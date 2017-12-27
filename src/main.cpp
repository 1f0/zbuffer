// 区间扫描线z缓冲器
#include "global.h"
#include "wavefront.h"
#include "show.h"

void initGlobalColorVars(){
  white << 255, 255, 255;
  black << 0, 0, 0;
  red << 255, 0, 0;
  green << 0, 255, 0;
  blue << 0, 0, 255;
}

int main(int argc, char** argv)
{
  if(argc != 2)return printf("usage: %s model.obj\n", argv[0]);
  Mesh mesh;
  readObj(argv[1], mesh);
  initGlobalColorVars();

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    show(mesh);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
