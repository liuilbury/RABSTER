//
// Created by liulizhang on 2020/6/18.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
// 设置窗口尺寸
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};
float vertices2[] = {
	0.1f, 0.5f, 0.0f,   // 右上角
	0.1f, -0.5f, 0.0f,  // 右下角
	-0.1f, -0.5f, 0.0f, // 左下角
	-0.1f, 0.5f, 0.0f   // 左上角
};
unsigned int indices[] = { // 注意索引从0开始!
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};
const char* vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "\n"
								 "void main()\n"
								 "{\n"
								 "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}";
const char* fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
		                           "uniform vec4 ourColor;\n"
								   "\n"
								   "void main()\n"
								   "{\n"
								   "    FragColor = ourColor;\n"
								   "} ";
void render(unsigned int &VBO2){
	unsigned int VAO2,EBO;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}unsigned int shaderProgram;
void tt(){
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//传入源码
	glCompileShader(vertexShader);//编译
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//片段着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	shaderProgram = glCreateProgram();//创建
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
int main()
{
	// glfw: 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfw 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CssParser", NULL, NULL);
	glfwMakeContextCurrent(window);
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	tt();
	unsigned int VAO2;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		render(VAO2);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFWresources.
	glfwTerminate();
	return 0;
}
