cmake_minimum_required(VERSION 4.2)
project(CSProiectPractica2026)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

include(FetchContent)

FetchContent_Declare(
        zlib
        URL https://github.com/madler/zlib/archive/refs/tags/v1.3.1.zip
)
FetchContent_MakeAvailable(zlib)

set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
set(CURL_USE_LIBPSL OFF CACHE INTERNAL "")
set(CURL_STATICLIB ON CACHE INTERNAL "")

FetchContent_Declare(
        curl
        URL https://github.com/curl/curl/archive/refs/tags/curl-8_7_1.zip
)
FetchContent_MakeAvailable(curl)

set(SOURCES
        main.cpp
        src/DataDownloader.cpp
)

add_executable(CSProiectPractica2026 ${SOURCES})


target_compile_definitions(CSProiectPractica2026 PRIVATE CURL_STATICLIB)

target_include_directories(CSProiectPractica2026 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(CSProiectPractica2026 PRIVATE libcurl zlibstatic ws2_32 wldap32 crypt32)