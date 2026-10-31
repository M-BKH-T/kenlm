cmake_minimum_required(VERSION 3.22.1)
project(kenlm_native CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DKENLM_MAX_ORDER=6)
add_definitions(-DHAVE_ZLIB)
add_compile_options(-fexceptions -frtti)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../util/double-conversion
)

# واکشی فایل‌های سورس محاسباتی KenLM
file(GLOB_RECURSE KENLM_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../util/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../lm/*.cc
)

# حذف ابزارهای آموزش مدل، پردازش استریم و تست‌ها
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*main\\.cc$")
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*test\\.cc$")
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*benchmark\\.cc$")
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*/filter/.*")
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*/interpolate/.*")
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*/builder/.*")
list(FILTER KENLM_SRCS EXCLUDE REGEX ".*/stream/.*")

add_library(
    kenlm_native
    SHARED
    kenlm_jni.cpp
    ${KENLM_SRCS}
)

find_library(log-lib log)
find_library(z-lib z)

target_link_libraries(
    kenlm_native
    ${log-lib}
    ${z-lib}
)
