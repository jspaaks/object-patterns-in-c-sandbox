add_executable(tgt_exe_test_mbm)

set_property(TARGET tgt_exe_test_mbm PROPERTY OUTPUT_NAME test_mbm)

target_compile_definitions(
    tgt_exe_test_mbm
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_test_mbm
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_test_mbm
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_test_mbm
    PRIVATE
        ../include
)

target_link_libraries(
    tgt_exe_test_mbm
    PRIVATE
        criterion
        tgt_lib_mbm
)

target_link_options(
    tgt_exe_test_mbm
    PRIVATE
        -pie
)

target_sources(
    tgt_exe_test_mbm
    PRIVATE
        test_balloon.c
)

add_test(
    NAME
        test_mbm
    COMMAND
        ${CMAKE_INSTALL_BINDIR}/test_mbm
)

install(TARGETS tgt_exe_test_mbm)
