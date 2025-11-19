add_executable(tgt_exe_mbm)

set_property(TARGET tgt_exe_mbm PROPERTY OUTPUT_NAME mbm)

target_compile_definitions(
    tgt_exe_mbm
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_mbm
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_mbm
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<BOOL:${MBM_APP_WITH_ASAN}>:-fsanitize=address>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_mbm
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_mbm
    PRIVATE
        m
        tgt_lib_mbm
)

target_link_options(
    tgt_exe_mbm
    PRIVATE
        -pie
        $<$<BOOL:${MBM_APP_WITH_ASAN}>:-fsanitize=address>
)

target_sources(
    tgt_exe_mbm
    PRIVATE
        main.c
)

install(TARGETS tgt_exe_mbm)
