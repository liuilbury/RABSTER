//
// Created by liulizhang on 2020/6/17.
//

#include <sstream>
#include "style.h"
template <class Type>
Type stringToNum(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
void calcolor(float*cl,const std::string &value){
	cl[0]=cl[1]=cl[2]=0;
	/*for(int i0=0;i0<=2;i0++)
	{
		for (int i = i0*2; i < i0*2+2; i++)
		{
			cl[i0] = cl[i0] * 16 + hex[value[i]];
		}
	}*/
	for(int i0=0;i0<=2;i0++)
	{
		for (int i = i0*3; i < i0*3+3; i++)
		{
			cl[i0] = cl[i0] * 10 + value[i]-'0';
		}
	}
}
void style::set(int key,const std::string &value)
{
	float cl[3];
	calcolor(cl,value);
	if(props[key]=="color"){
		color[0]=cl[0]/255;
		color[1]=cl[1]/255;
		color[2]=cl[2]/255;
	}else if(props[key]=="border"){
	}
}
float* style::getColor()
{
	return color;
}
std::string style::getBorder()
{
	return border;
}