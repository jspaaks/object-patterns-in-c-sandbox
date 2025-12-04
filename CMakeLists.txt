cmake_minimum_required(VERSION 3.23...3.28 FATAL_ERROR)

project(mbm LANGUAGES C)

option(MBM_APP_WITH_ASAN "Whether to enable address sanitizing for executable 'mbm'" OFF)
option(MBM_LIB_WITH_ASAN "Whether to enable address sanitizing for library 'mbm'" OFF)
option(MBM_USE_VENDORED_SDL3 "Whether to use SDL3 from vendored or system" ON)

include(CTest)
include(GenerateExportHeader)
include(GNUInstallDirs)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

if (APPLE)
    list(APPEND CMAKE_INSTALL_RPATH @loader_path/../lib)
elseif(UNIX)
    list(APPEND CMAKE_INSTALL_RPATH $ORIGIN/../lib)
endif()

if (PROJECT_IS_TOP_LEVEL)
    # I'm the top-most project, set MBM_BUILD_TESTING like BUILD_TESTING by default
    option(MBM_BUILD_TESTING "Whether to build the libmbm tests" ${BUILD_TESTING})
else()
    # I'm not the top-most project, set MBM_BUILD_TESTING default OFF
    option(MBM_BUILD_TESTING "Whether to build the libmbm tests" OFF)
endif()

if (MBM_USE_VENDORED_SDL3)
    add_subdirectory(third_party/SDL EXCLUDE_FROM_ALL)
    install(TARGETS SDL3-shared)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_subdirectory(third_party/libidx EXCLUDE_FROM_ALL)
install(TARGETS tgt_lib_idx)

add_subdirectory(assets)
add_subdirectory(src/app)
add_subdirectory(src/mbm)
if (MBM_BUILD_TESTING)
    #add_subdirectory(test/mbm)
endif()
