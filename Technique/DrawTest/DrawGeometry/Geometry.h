#pragma once

#define  USE_GLEW 1

#include <string>
//#if USE_GLEW 
#include <GL/glew.h>

//#else 
//#include <glad/glad.h>
//#endif 

#include <Shape.h>

class Geometry
{
public:
	Geometry(void);
	virtual ~Geometry(void);

	virtual void Init() =0;
	virtual void Draw() =0;
	virtual void Destroy() =0;

	void SetName(const char* name)
	{
		_name = name;
	}

protected:
	std::string _name;
	std::vector<glm::vec3>	_points;
	std::vector<int>		_indexs;

};

// OpenGL 立即模式
class ImmediateGeometry :public Geometry
{
public:
	ImmediateGeometry();
	~ImmediateGeometry();
	virtual void Init();
	virtual void Draw();
	virtual void Destroy();

	
};
//
//// 显示列表
//class DisplayListGeometry :public Geometry
//{
//
//public:
//	virtual void Init();
//	virtual void Draw();
//	virtual void Destroy();
//
//};
//
//// 顶点数组
//class ArraysGeometry :public Geometry
//{
//
//public:
//
//	virtual void Init();
//	virtual void Draw();
//	virtual void Destroy();
//
//};
//
//
////
//class VBOArrayGeometry :public Geometry
//{
//
//public:
//
//	virtual void Init();
//	virtual void Draw();
//	virtual void Destroy();
//
//};
//
//// VAO + VBO
//class VAOGeometry :public Geometry
//{
//
//public:
//
//	virtual void Init();
//	virtual void Draw();
//	virtual void Destroy();
//
//};
//
//// VAO + VBO + EBO
//class DisplayListGeometry :public Geometry
//{
//
//public:
//
//	virtual void Init();
//	virtual void Draw();
//	virtual void Destroy();
//
//}
//
//
//// VAO + VBO + EBO + Shader 
//class DisplayListGeometry :public Geometry
//{
//
//public:
//
//	virtual void Init();
//	virtual void Draw();
//	virtual void Destroy();
//
//};
