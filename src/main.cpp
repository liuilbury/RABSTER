//
// Created by liulizhang on 2020/6/18.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include "html/node.h"
#include "lex/CSSLex.h"
#include <stack>
#include <vector>
// 设置窗口尺寸
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
float vertices1[12] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};
float vertices2[12] = {
	0.4f, 0.4f, 0.0f,   // 右上角
	0.4f, -0.4f, 0.0f,  // 右下角
	-0.4f, -0.4f, 0.0f, // 左下角
	-0.4f, 0.4f, 0.0f   // 左上角
};
float vertices3[12] = {
	0.3f, 0.3f, 0.0f,   // 右上角
	0.3f, -0.3f, 0.0f,  // 右下角
	-0.3f, -0.3f, 0.0f, // 左下角
	-0.3f, 0.3f, 0.0f   // 左上角
};
float vertices4[12] = {
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
std::map<std::string, gl*> TREE;
std::set<node*> changeNode;
int flag = 0;
std::string css;
void getHtml()
{
	std::ifstream in("F:\\opengl\\glfw\\cssDemo");
	std::ostringstream buf;
	buf << in.rdbuf();
	css = buf.str();
	std::cout << buf.str() << std::endl;
}

void render()
{
	for (auto i:TREE)
	{
		if (changeNode.count(i.second->cssNode))
		{
			glUseProgram(i.second->shaderProgram);
			int vertexColorLocation = glGetUniformLocation(i.second->shaderProgram, "ourColor");
			float *color=i.second->cssNode->_final_style.getColor();
			printf("%f %f %f\n",i.second->cssNode->_final_style.color[0],i.second->cssNode->_final_style.color[1],i.second->cssNode->_final_style.color[2]);
			glUniform4f(vertexColorLocation, color[0], color[1], color[2], 1.0f);
		}
		i.second->render();
	}
}
node* getNode()
{
	return new node;
}
void changeCss(node* dom, std::string& key, std::string& value)
{
	dom->change(propsMap[key], value);
	changeNode = dom->update();
	render();
}
void pushdown(node* dom)
{
	if (TREE.find(dom->name) != TREE.end())
	{
		TREE[dom->name]->cssNode = dom;
		changeNode.insert(dom);
	}
	for (auto i:dom->_children)
	{
		pushdown(i);
	}
	if (!dom->_changeSet.empty())
	{
		dom->update();
	}
}

void parser(std::string css, node* root)
{
	CSSLex* lex = new CSSLex();
	lex->SetBufferString(css);
	CSSToken* token;
	std::stack<node*> st;
	node* nowNode = root;
	int prop;
	while ((token = lex->GetToken())->data != "EOF")
	{
		if (propsMap.find(token->data) != propsMap.end())
		{
			prop = propsMap[token->data];
		}
		else if (token->type == CSS_TOKEN_IDENT)
		{
			node* newNode = getNode();
			newNode->name = token->data;
			nowNode->add_child(newNode);
			st.push(nowNode);
			nowNode = newNode;
		}
		else if (token->data == ":")
		{
			token = lex->GetToken();
			nowNode->change(prop, token->data);
		}
		else if (token->data == "\n")
		{
			if (!st.empty())
			{
				st.pop();
				nowNode = st.top();
			}
		}
		else
		{
			continue;
		}
	}
	root->change(propsMap["color"],"000000000");
	pushdown(root);
	render();
}
node* root;
void parserHtml()
{
	root = getNode();
	root->name = "root";
	parser(css, root);
}


int RGB[3][3];
void processInput(GLFWwindow* window)
{
	std::string vv;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		RGB[0][0]++;
		if (flag)return;
		flag = 1;
		for(int i=0;i<3;i++){
			vv+=(RGB[0][i]/100%10+'0');
			vv+=(RGB[0][i]/10%10+'0');
			vv+=(RGB[0][i]/1%10+'0');
		}
		std::string c = "color";
		changeCss(TREE["no"]->cssNode, c, vv);
		return;
	}else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
		RGB[1][1]++;
		if (flag)return;
		flag = 1;
		for(int i=0;i<3;i++){
			vv+=(RGB[1][i]/100%10+'0');
			vv+=(RGB[1][i]/10%10+'0');
			vv+=(RGB[1][i]/1%10+'0');
		}
		std::string c = "color";
		changeCss(TREE["nod"]->cssNode, c, vv);
		return;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		RGB[2][2]++;
		if (flag)return;
		flag = 1;
		std::string c = "color";
		for(int i=0;i<3;i++){
			vv+=(RGB[2][i]/100%10+'0');
			vv+=(RGB[2][i]/10%10+'0');
			vv+=(RGB[2][i]/1%10+'0');
		}
		changeCss(TREE["node"]->cssNode, c, vv);
		return;
	}
	flag = 0;
}
int main()
{
	// glfw: 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CssParser", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// render loop
	TREE["no"] = new gl(vertices1, vertexShaderSource, fragmentShaderSource);
	TREE["nod"] = new gl(vertices2, vertexShaderSource, fragmentShaderSource);
	TREE["node"] = new gl(vertices3, vertexShaderSource, fragmentShaderSource);
	TREE["nodee"] = new gl(vertices4, vertexShaderSource, fragmentShaderSource);
	getCss();
	parserCss();
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
	return 0;
}