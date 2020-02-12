#pragma once

#define  USE_GLEW 1

#include <string>
//#if USE_GLEW 
#include <GL/glew.h>

//#else 
//#include <glad/glad.h>
//#endif 

#include "Shape.h"

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
	std::vector<unsigned int>		_indexs;

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

// ��ʾ�б�
class DisplayListGeometry :public Geometry
{

public:
	virtual void Init();
	virtual void Draw();
	virtual void Destroy();

private:
	GLuint _listId;
};
//
//// ��������
// ʹ�ö��������õ���API��
// ��ͼ�ӿ� glDrawArrays��glDrawElements 
// �ڴ����ýӿڣ� glVertexPointer,���ڽ�cpu�ڴ洫�͸�GPU
// ���ö������� glEnabeClientState

class VertexArraysGeometry :public Geometry
{
public:
	virtual void Init();
	virtual void Draw();
	virtual void Destroy();

};


// VBO 
class VBOArrayGeometry :public Geometry
{

public:

	virtual void Init();
	virtual void Draw();
	virtual void Destroy();
private:
	GLuint _vbo_Id;
};
//
// VAO + VBO
class VAOGeometry :public Geometry
{

public:

	virtual void Init();
	virtual void Draw();
	virtual void Destroy();

private:
	GLuint _vao_id;
	GLuint _vbo_id;
};

// VAO + Array
class VAOArrayGeometry :public Geometry
{

public:

	virtual void Init();
	virtual void Draw();
	virtual void Destroy();

private:
	GLuint _vao_id;
	 
};

//
// VAO + VBO + EBO
class EBOGeometry :public Geometry
{
public:
	virtual void Init();
	virtual void Draw();
	virtual void Destroy();
	
private:
	GLuint _vao_id;
	GLuint _vbo_id;
	GLuint _ebo_id;
};


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


// reference: http://www.360doc.com/content/14/1028/10/19175681_420522404.shtml