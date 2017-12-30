file(GLOB source *.cpp)
add_executable(scanline ${source})
target_link_libraries(scanline
  ${GLFW_LIBRARIES}
  GL
  )
