#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// 查询GL_MAX_VERTEX_ATTRIBS具体的上限
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	return 0;
}