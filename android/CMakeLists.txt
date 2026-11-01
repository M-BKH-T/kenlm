cmake_minimum_required(VERSION 3.22.1)
project(kenlm_native CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DKENLM_MAX_ORDER=6)
add_definitions(-DHAVE_ZLIB)
add_definitions(-D_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
add_compile_options(-fexceptions -frtti -O3)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../util/double-conversion
)

# دقیقاً فایل‌های پایه مورد نیاز لود و ارزیابی باینری بدون پوشه‌های wrappers, builder, stream
set(UTIL_SRCS
    ../util/bit_packing.cc
    ../util/ersatz_progress.cc
    ../util/exception.cc
    ../util/file.cc
    ../util/file_piece.cc
    ../util/float_to_string.cc
    ../util/integer_to_string.cc
    ../util/mmap.cc
    ../util/murmur_hash.cc
    ../util/parallel_read.cc
    ../util/pool.cc
    ../util/read_compressed.cc
    ../util/scoped.cc
    ../util/string_piece.cc
    ../util/usage.cc
    ../util/double-conversion/bignum.cc
    ../util/double-conversion/bignum-dtoa.cc
    ../util/double-conversion/cached-powers.cc
    ../util/double-conversion/diy-fp.cc
    ../util/double-conversion/double-conversion.cc
    ../util/double-conversion/fast-dtoa.cc
    ../util/double-conversion/fixed-dtoa.cc
    ../util/double-conversion/strtod.cc
)

set(LM_SRCS
    ../lm/bhiksha.cc
    ../lm/binary_format.cc
    ../lm/config.cc
    ../lm/lm_exception.cc
    ../lm/model.cc
    ../lm/quantize.cc
    ../lm/read_arpa.cc
    ../lm/search_hashed.cc
    ../lm/search_trie.cc
    ../lm/sizes.cc
    ../lm/trie.cc
    ../lm/trie_sort.cc
    ../lm/value_build.cc
    ../lm/virtual_interface.cc
    ../lm/vocab.cc
)

add_library(
    kenlm_native
    SHARED
    kenlm_jni.cpp
    ${UTIL_SRCS}
    ${LM_SRCS}
)

find_library(log-lib log)
find_library(z-lib z)

target_link_libraries(
    kenlm_native
    ${log-lib}
    ${z-lib}
)
