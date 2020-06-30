//
// Created by liulizhang on 2020/6/18.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "html/html.h"
#include <vector>
// 设置窗口尺寸
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
float vertices[4][12]={0.5f, 0.5f, 0.0f,   // 右上角
					   0.5f, -0.5f, 0.0f,  // 右下角
					   -0.5f, -0.5f, 0.0f, // 左下角
					   -0.5f, 0.5f, 0.0f,  // 左上角

						0.4f, 0.4f, 0.0f,   // 右上角
						0.4f, -0.4f, 0.0f,  // 右下角
						-0.4f, -0.4f, 0.0f, // 左下角
						-0.4f, 0.4f, 0.0f,  // 左上角

						0.3f, 0.3f, 0.0f,   // 右上角
						0.3f, -0.3f, 0.0f,  // 右下角
						-0.3f, -0.3f, 0.0f, // 左下角
						-0.3f, 0.3f, 0.0f,  // 左上角

						0.1f, 0.1f, 0.0f,   // 右上角
						0.1f, -0.1f, 0.0f,  // 右下角
						-0.1f, -0.1f, 0.0f, // 左下角
						-0.1f, 0.1f, 0.0f   // 左上角
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
node* root;
HtmlContent* html_ctx;
class gl
{
 public:
	node* cssNode;
	void render()
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	gl(float vertices[], const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		this->vertexShaderSource = vertexShaderSource;
		this->fragmentShaderSource = fragmentShaderSource;
		for (int i = 0; i <= 11; i++)this->vertices[i] = vertices[i];
		Shader(shaderProgram);
	}
	unsigned int shaderProgram;
 private:
	unsigned int VAO;
	unsigned int VBO, EBO;
	float vertices[12];
	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	void Shader(unsigned& shaderProgram)
	{
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器对象;
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
		glUseProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
};
std::map<int, gl*> TREE;
std::set<node*> changeNode;
int flag = 0;
void render()
{
	for (auto i:TREE)
	{
		glUseProgram(i.second->shaderProgram);
		int vertexColorLocation = glGetUniformLocation(i.second->shaderProgram, "ourColor");
#ifdef DEBUG_ALL
		printf("render node is:%s\ncolor:\n",i.second->cssNode->real_name.c_str());
#endif
		css_color color_shade;
		css_computed_color(i.second->cssNode->_final_style, &color_shade);
		char buf[8];
		sprintf(buf, "%x", color_shade);
		float color[4];
		for (int j = 0; j < 4; j++)
		{
			uint32_t x;
			sscanf(buf + 2 * j, "%2x", &x);
#ifdef DEBUG_ALL
			printf("%d%c", x,j==3?'\n':',');
#endif
			color[j] = x * 1.0 / 255;
		}
		glUniform4f(vertexColorLocation, color[1], color[2], color[3], color[0]);
	}
}
void changeCss(node* dom, std::string& key, std::string& value)
{
	dom->change(propsMap[key], value);
	changeNode = dom->update();
	render();
}

void processInput(GLFWwindow* window)
{
	std::string vv="color:#";
	char *x;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		//cout<<vv<<endl;
		TREE[0]->cssNode->_inline_style=vv;
		html_ctx->node_style_updata(TREE[0]->cssNode,CSS_NODE_MODIFIED);
		html_ctx->get_tree_style(root);
		render();
		return;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		//cout<<vv<<endl;
		TREE[1]->cssNode->_inline_style=vv;
		html_ctx->node_style_updata(TREE[1]->cssNode,CSS_NODE_MODIFIED);
		html_ctx->get_tree_style(root);
		render();
		return;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		//cout<<vv<<endl;
		TREE[2]->cssNode->_inline_style=vv;
		html_ctx->node_style_updata(TREE[2]->cssNode,CSS_NODE_MODIFIED);
		html_ctx->get_tree_style(root);
		render();
		return;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		itoa(rand()%240+16,x,16);
		vv+=x;
		//cout<<vv<<endl;
		TREE[3]->cssNode->_inline_style=vv;
		html_ctx->node_style_updata(TREE[3]->cssNode,CSS_NODE_MODIFIED);
		html_ctx->get_tree_style(root);
		render();
		return;
	}
	flag = 0;
}
GLFWwindow* window;
void glfw_init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DEMO", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;
}
void glfw_run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		for (auto i:TREE)
		{
			i.second->render();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
int cnt=0;
void for_tree(node *root){
	if(root->real_name!="root"){
		TREE[cnt]=new gl(vertices[cnt], vertexShaderSource, fragmentShaderSource);
		TREE[cnt]->cssNode=root;
		++cnt;
	}
	for(auto i:root->_children){
		for_tree(i);
	}
}
void html_init()
{
	root = html_init(R"(F:\opengl\glfw\test.json)");
	html_ctx = new HtmlContent();
	html_ctx->html_css_new_stylesheets();
	html_ctx->html_css_append_stylesheets(R"(F:\opengl\glfw\test.css)");
	html_ctx->html_css_new_selection_context();
	html_ctx->get_tree_style(root);
	for_tree(root);
}
void css_init()
{
	render();
}
int main()
{
	glfw_init();
	html_init();
	css_init();
	glfw_run();
	return 0;
}