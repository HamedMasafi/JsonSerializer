add_executable(tst_variant
    tst_variant.cpp
    ../testcommon.h ../testcommon.cpp
)

target_include_directories(tst_variant PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_link_libraries(tst_variant PRIVATE
    JsonSerializer
    Qt6::Test
    Qt6::Core
    Qt6::Gui
)

add_test(NAME VariantTest COMMAND tst_variant)