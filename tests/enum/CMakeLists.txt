add_executable(tst_enum
    tst_enum.cpp
)

target_include_directories(tst_enum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_link_libraries(tst_enum PRIVATE
    JsonSerializer
    Qt6::Test
    Qt6::Core
    Qt6::Gui
)

add_test(NAME EnumTest COMMAND tst_enum)