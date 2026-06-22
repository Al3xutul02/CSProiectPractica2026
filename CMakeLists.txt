cmake_minimum_required(VERSION 3.25)

project(CSProiectPractica2026 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------
# Dependencies (provided by vcpkg, see vcpkg.json)
#
# The vcpkg toolchain file is supplied via CMakePresets.json
# (CMAKE_TOOLCHAIN_FILE -> $VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake),
# so every find_package below resolves against the vcpkg install tree.
# ------------------------------------------------------------

find_package(CURL CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

# netcdf-cxx4 pulls in netcdf-c and HDF5 transitively.
find_package(netCDFCxx CONFIG REQUIRED)

# ------------------------------------------------------------
# Application
# ------------------------------------------------------------

add_executable(CSProiectPractica2026
        main.cpp
        src/DataDownloader.cpp
)

target_include_directories(CSProiectPractica2026
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CSProiectPractica2026
        PRIVATE
        CURL::libcurl
        ZLIB::ZLIB
        netCDF::netcdf-cxx4
)
