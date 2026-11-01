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

# ۱. واکشی خودکار فایل‌های double-conversion موجود
file(GLOB DOUBLE_CONV_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../util/double-conversion/*.cc")

# ۲. واکشی فایل‌های util (فقط لایه اصلی بدون ورود به ساب‌فولدرها)
file(GLOB UTIL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../util/*.cc")
list(FILTER UTIL_SRCS EXCLUDE REGEX ".*main\\.cc$")
list(FILTER UTIL_SRCS EXCLUDE REGEX ".*test\\.cc$")
list(FILTER UTIL_SRCS EXCLUDE REGEX ".*benchmark\\.cc$")

# ۳. واکشی فایل‌های lm (فقط لایه اصلی بدون ورود به builder و wrappers)
file(GLOB LM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../lm/*.cc")
list(FILTER LM_SRCS EXCLUDE REGEX ".*main\\.cc$")
list(FILTER LM_SRCS EXCLUDE REGEX ".*test\\.cc$")
list(FILTER LM_SRCS EXCLUDE REGEX ".*benchmark\\.cc$")

add_library(
    kenlm_native
    SHARED
    kenlm_jni.cpp
    ${DOUBLE_CONV_SRCS}
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
