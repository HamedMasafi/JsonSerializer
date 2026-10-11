add_executable(tst_basic
    tst_basic.cpp
)

target_include_directories(tst_basic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_link_libraries(tst_basic PRIVATE
    JsonSerializer
    Qt6::Test
    Qt6::Core
    Qt6::Gui
)

add_test(NAME BasicTest COMMAND tst_basic)