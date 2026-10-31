#include <jni.h>
#include <string>
#include <sstream>
#include <memory>
#include <cstring>
#include <android/log.h>

// هدرهای ضروری لایبرری رسمی KenLM
#include "lm/model.hh"
#include "lm/virtual_interface.hh"
#include "util/mmap.hh"

#define LOG_TAG "KenLmNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static lm::base::Model* g_model = nullptr;

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_logcat_texttospeech_engine_stt_KenLmNativeEngine_nativeLoadModel(
        JNIEnv *env,
        jobject /* this */,
        jstring j_model_path,
        jstring /* j_vocab_path */) {

    const char *model_path = env->GetStringUTFChars(j_model_path, nullptr);
    LOGI("Loading KenLM Binary via mmap from: %s", model_path);

    try {
        lm::ngram::Config config;
        config.load_method = util::POPULATE_OR_READ;

        if (g_model) {
            delete g_model;
            g_model = nullptr;
        }

        // لود مستقیم مدل باینری کوانتایز شده Trie
        g_model = lm::ngram::LoadVirtual(model_path, config);
        env->ReleaseStringUTFChars(j_model_path, model_path);

        if (g_model != nullptr) {
            LOGI("✅ KenLM Binary successfully loaded into memory via mmap!");
            return JNI_TRUE;
        }
    } catch (const std::exception& e) {
        LOGE("🔴 Exception loading KenLM binary: %s", e.what());
    } catch (...) {
        LOGE("🔴 Unknown exception loading KenLM binary!");
    }

    env->ReleaseStringUTFChars(j_model_path, model_path);
    return JNI_FALSE;
}

JNIEXPORT jfloat JNICALL
Java_com_logcat_texttospeech_engine_stt_KenLmNativeEngine_nativeScoreSentence(
        JNIEnv *env,
        jobject /* this */,
        jstring j_sentence) {

    if (!g_model) {
        return -999.0f;
    }

    const char *sentence = env->GetStringUTFChars(j_sentence, nullptr);
    std::string text(sentence);
    env->ReleaseStringUTFChars(j_sentence, sentence);

    std::unique_ptr<uint8_t[]> state_buf(new uint8_t[g_model->StateSize()]);
    std::unique_ptr<uint8_t[]> out_state_buf(new uint8_t[g_model->StateSize()]);

    void* state = state_buf.get();
    void* out_state = out_state_buf.get();

    g_model->BeginSentenceWrite(state);

    std::istringstream iss(text);
    std::string token;
    float total_score = 0.0f;

    const lm::base::Vocabulary& vocab = g_model->BaseVocabulary();

    while (iss >> token) {
        lm::WordIndex word_idx = vocab.Index(token);
        total_score += g_model->BaseScore(state, word_idx, out_state);
        std::memcpy(state, out_state, g_model->StateSize());
    }

    total_score += g_model->BaseScore(state, vocab.EndSentence(), out_state);
    return total_score;
}

JNIEXPORT jboolean JNICALL
Java_com_logcat_texttospeech_engine_stt_KenLmNativeEngine_nativeIsLoaded(
        JNIEnv *env,
        jobject /* this */) {
    return (g_model != nullptr) ? JNI_TRUE : JNI_FALSE;
}

}
