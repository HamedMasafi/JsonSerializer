add_library(JsonSerializer
  jsonserialize.h jsonserialize.cpp
  qttypes.h qttypes.cpp
  object.h object.cpp
  variant.h variant.cpp
  qtcontainers.h qtcontainers.cpp
  traits.h
  jsonserialize_def.h
)
target_link_libraries(JsonSerializer PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui)
