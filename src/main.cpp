// 区间扫描线z缓冲器
#include <GLFW/glfw3.h>
#include "display.h"
#include "mesh.h"

int main(int argc, char** argv) {
  if (argc != 2) return printf("usage: %s model.obj\n", argv[0]);
  if (!glfwInit()) return printf("glfw init fail\n");

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow* window = glfwCreateWindow(Display::width, Display::height, "Scanning Line", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return printf("cannot create window\n");
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, Display::keyCallback);

  Mesh mesh(argv[1], true);
  while (!glfwWindowShouldClose(window)) {
    Display::monitorFPS();
    glClear(GL_COLOR_BUFFER_BIT);
    Display::display(mesh);
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}