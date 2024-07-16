//
// Created by LMR on 24-7-16.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#include "shader_s.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char **argv)
{
  // glfw：初始化和配置
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // GLFW 窗口创建
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad：加载所有 OpenGL 函数指针
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 构建和编译我们的着色器程序
  Shader ourShader("transform.vs", "transform.fs");

  // 设置顶点数据（和缓冲区）并配置顶点属性
  float vertices[] = {
      // positions         // texture coords
      0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
      0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
      -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position 属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // 纹理坐标属性
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 加载并创建纹理
  unsigned int texture1;
  // 生成纹理1
  glGenTextures(1, &texture1);
  // 绑定纹理1
  glBindTexture(GL_TEXTURE_2D, texture1);

  // 纹理环绕方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	//纹理环绕方式 GL_REPEAT (默认)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // 设置纹理过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 加载并生成纹理1
  int width, height, nrChannels;
  // 在 y 轴上翻转加载的纹理
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("images/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    // 使用载入的图片数据生成一个纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cerr << "Failed to load texture1" << std::endl;
  }
  stbi_image_free(data);

  // 加载并创建纹理2
  unsigned int texture2;
  // 生成纹理2
  glGenTextures(1, &texture2);
  // 绑定纹理2
  glBindTexture(GL_TEXTURE_2D, texture2);
  // 纹理环绕方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	//纹理环绕方式 GL_REPEAT (默认)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // 设置纹理过滤
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 加载并生成纹理2
  data = stbi_load("images/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    // 使用载入的图片数据生成一个纹理
    // 注意，awesomeface.png具有透明度，因此具有alpha通道，因此OpenGL数据类型为GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // 为当前绑定的纹理自动生成所有需要的多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cerr << "Failed to load texture2" << std::endl;
  }
  stbi_image_free(data);

  // 告诉每个OpenGL 的采样器它属于哪个纹理单元（只需执行一次）
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  // 渲染循环
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 将纹理绑定到相应的纹理单元上
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 创建变换
    // 确保先将 matrix 初始化为 identity matrix
    glm::mat4 transform = glm::mat4(1.0f);
    // 第一个变换
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));

    // 获取矩阵uniform变量的地址，设置矩阵
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // 渲染容器
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // 第二个变换
    // ---------------------
    transform = glm::mat4(1.0f); // reset it to identity matrix
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scaleAmount = static_cast<float>(sin(glfwGetTime()));
    transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
    // 这一次，将矩阵值数组的第一个元素作为其内存指针值
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

    // 现在，随着均匀矩阵被新的变换替换，再次绘制它。
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glfw：交换缓冲区和轮询 IO 事件（按下/释放键、移动鼠标等）
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 可选：一旦所有资源超出其用途，就取消分配它们：
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw：终止，清除所有以前分配的 GLFW 资源。
  glfwTerminate();
  return 0;
}

// 处理所有输入：查询GLFW是否按下相关键释放此帧并做出相应反应
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小发生变化（通过操作系统或用户调整大小）时，都会执行此回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // 确保视口与新窗口尺寸匹配;请注意，宽度和高度将明显大于视网膜显示屏上指定的尺寸。
  glViewport(0, 0, width, height);
}

