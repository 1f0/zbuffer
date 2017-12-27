add_executable(zbuffer main.cpp)
target_link_libraries(zbuffer
  ${GLFW_LIBRARIES}
  GL
  )
