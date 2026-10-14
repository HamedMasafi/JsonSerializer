add_executable(tst_containers
    tst_containers.cpp
    ../testcommon.h ../testcommon.cpp
)

target_include_directories(tst_containers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_link_libraries(tst_containers PRIVATE
    JsonSerializer
    Qt6::Test
    Qt6::Core
    Qt6::Gui
)

add_test(NAME ContainersTest COMMAND tst_containers)