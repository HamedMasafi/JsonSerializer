add_executable(tst_inheristance
    tst_inheristance.cpp
    animalobject.h animalobject.cpp
    tigerobject.h tigerobject.cpp
    lionobject.h lionobject.cpp
)

target_include_directories(tst_inheristance PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

target_link_libraries(tst_inheristance PRIVATE
    JsonSerializer
    Qt6::Test
    Qt6::Core
    Qt6::Gui
)

add_test(NAME InheristanceTest COMMAND tst_inheristance)