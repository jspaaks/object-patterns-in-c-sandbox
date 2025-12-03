add_library(tgt_lib_mbm SHARED)

set_property(TARGET tgt_lib_mbm PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET tgt_lib_mbm PROPERTY OUTPUT_NAME mbm)

generate_export_header(
    tgt_lib_mbm
    BASE_NAME mbm
    EXPORT_MACRO_NAME MBM_ABI
    NO_EXPORT_MACRO_NAME MBM_NO_ABI
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/mbm/abi.h
)

target_compile_definitions(
    tgt_lib_mbm
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_lib_mbm
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_lib_mbm
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIC
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<BOOL:${MBM_LIB_WITH_ASAN}>:-fsanitize=address>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_lib_mbm
    PRIVATE
        ../../include
        ../../third_party/SDL/include
)

target_link_libraries(
    tgt_lib_mbm
    PRIVATE
)

target_link_options(
    tgt_lib_mbm
    PRIVATE
        $<$<BOOL:${MBM_LIB_WITH_ASAN}>:-fsanitize=address>
)

target_sources(
    tgt_lib_mbm
    PRIVATE
        background.c
        game.c
        timings.c
        world.c
    PUBLIC
        FILE_SET fset_lib_mbm_headers
            TYPE HEADERS
            BASE_DIRS
                ../../include
                ${CMAKE_BINARY_DIR}/include  # contains cmake-generated file
            FILES
                ../../include/mbm/background.h
                ../../include/mbm/game.h
                ../../include/mbm/timings.h
                ../../include/mbm/world.h
                ${CMAKE_BINARY_DIR}/include/mbm/abi.h  # cmake-generated file
)

install(TARGETS tgt_lib_mbm
    LIBRARY
    FILE_SET fset_lib_mbm_headers
)
