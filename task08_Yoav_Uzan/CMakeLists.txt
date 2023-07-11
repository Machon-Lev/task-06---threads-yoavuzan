# CMakeList.txt : CMake project for task08_Yoav_Uzan, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (task08_Yoav_Uzan  "src/main.cpp" "src/message.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET task08_Yoav_Uzan PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
add_subdirectory(include)
add_subdirectory(src)