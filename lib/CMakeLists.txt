add_library(JsonSerializer STATIC
  jsonserialize.h
  qttypes.h
  qttypes.cpp
  object.h
  object.cpp
  variant.h
  variant.cpp
  qtcontainers.h
  traits.h
  jsonserialize_def.h
  enum.h
  enum.cpp
  options.h
  jsonzerializer_export.h
)

target_link_libraries(JsonSerializer
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
)

target_include_directories(JsonSerializer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(JsonSerializer
    PRIVATE
        JSONSERIALIZER_LIBRARY
)