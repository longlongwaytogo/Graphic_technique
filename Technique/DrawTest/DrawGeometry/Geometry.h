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

// OpenGL ����ģʽ
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
//// ��ʾ�б�
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
//// ��������
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
