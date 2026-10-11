add_executable(tst_object
    tst_object.cpp
    myobject.h myobject.cpp
    mygadget.h mygadget.cpp
)

target_include_directories(tst_object PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_link_libraries(tst_object PRIVATE
    JsonSerializer
    Qt6::Test
    Qt6::Core
    Qt6::Gui
)

add_test(NAME ObjectTest COMMAND tst_object)