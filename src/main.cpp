// 区间扫描线z缓冲器
#include "global.h"
#include "wavefront.h"
#include "show.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT){
  	switch(key){
  	case GLFW_KEY_ESCAPE:exit(0);
  	case GLFW_KEY_W:lat-=0.0625;break;
  	case GLFW_KEY_S:lat+=0.0625;break;
  	case GLFW_KEY_A:lng-=0.0625;break;
  	case GLFW_KEY_D:lng+=0.0625;break;
  	case GLFW_KEY_Q:if(scale < 128)scale *= 2;break;
  	case GLFW_KEY_E:if(scale > 1/128)scale /= 2;break;
    case GLFW_KEY_R:mode=static_cast<Mode>((mode+1)%(numOfMode));break;
  	}
    update = true;
    cout<<"scale:"<<scale<<" lng:"<<lng
    <<" lat:"<<lat<<" mode:"<<mode<<endl;
  }
}

void monitorFPS(){
  static double lastTime = glfwGetTime();
  static int frameCount = 0;
  double currentTime = glfwGetTime();
  ++frameCount;
  if(currentTime-lastTime >= 1){
    cout << "fps: " << frameCount/(currentTime-lastTime) << endl;
    frameCount = 0;
    lastTime = currentTime;
  }
}

int main(int argc, char** argv) {
  if(argc != 2)return printf("usage: %s model.obj\n", argv[0]);
  Mesh mesh(argv[1]);
  normalize(mesh.pts);

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
  glfwSetKeyCallback(window, key_callback);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    monitorFPS();

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
