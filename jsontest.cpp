//
// Created by liulizhang on 2020/6/24.
//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "src/html/html.h"
using namespace rapidjson;
void test(){
	// 1. Parse a JSON string into DOM.
	std::ifstream in(R"(F:\opengl\glfw\test.json)");
	std::ostringstream buf;
	buf << in.rdbuf();
	std::string ss=buf.str();
	const char* html = ss.c_str();
	const char* json = R"({ "hello" : "world", "t" : true , "f" : false, "n": null, "i":123, "pi": 3.1416, "a":[1, 2, 3, 4] })";
	Document d;
	d.Parse(html);
	Value& s = d["node"];
	// 2. Modify it by DOM.
	static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
	for (SizeType i = 0; i < s.Size(); i++)
	{
		printf("%d\n",i);
		printf("%s\n",s[i]["name"].GetString());
		for (auto& m : s[i].GetObject())
			printf("Type of member %s is %s\n", m.name.GetString(), kTypeNames[m.value.GetType()]);
	}
	//s.SetInt(s.GetInt() + 1);
	//ss=s.GetString();
	// 3. Stringify the DOM
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	// Output {"project":"rapidjson","stars":11}
	std::cout << buffer.GetString() << std::endl;
}
int main() {
	html_init(R"(F:\opengl\glfw\test.json)");
	return 0;
}