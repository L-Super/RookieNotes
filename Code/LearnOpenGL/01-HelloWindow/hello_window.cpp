#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>

// 每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "window size changed\n";
}

// 检查用户是否按下了Esc 如果没有按下，glfwGetKey将会返回GLFW_RELEASE。
// 如果用户按下了，将通过glfwSetwindowShouldClose把WindowShouldClose属性设置为true关闭GLFW
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		std::cout << "ESC key pressed\n";
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	// 初始化GLFW
	glfwInit();
	// 将主版本号(Major)和次版本号(Minor)都设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 使用的是核心模式(Core-profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建一个窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);
	// 注册函数，告诉GLFW每当窗口调整大小的时候调用这个函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// 设置窗口的维度(Dimension)
	// 前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
	glViewport(0, 0, 800, 600);


	// 渲染循环(Render Loop)
	// 每次循环的开始前检查一次GLFW是否被要求退出，如果是的话，该函数返回true，渲染循环便结束了
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// 设置清空屏幕所用的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// 清空屏幕的颜色缓冲 接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		// 检查有没有触发事件（比如键盘输入、鼠标移动等）、更新窗口状态，
		// 并调用对应的回调函数（可以通过回调方法手动设置）。
		glfwPollEvents();
		// 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
		// 它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
	}

	// 正确释放/删除之前的分配的所有资源
	glfwTerminate();
	return 0;
}