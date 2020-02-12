// DrawGeometry.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#define  USE_GLEW 1


//#include <GL/GL.h>

#include <ExampleBase.h> 
#include "Geometry.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GEOMETRY_COUNT 6
class DrawGeometry: public ExampleBase
{
public:
	DrawGeometry()
	{
	
	}

	~DrawGeometry()
	{
		 for(auto itr = _geometrys.begin(); itr != _geometrys.end(); ++itr)
		{
			(*itr)->Destroy();
		}
	}

	virtual bool initExample()
	{
		ExampleBase::initExample();
		// 0 立即模式
		std::shared_ptr<Geometry> pImmediateGeometry(new ImmediateGeometry());
		pImmediateGeometry->SetName("ImmediateGeometry");
		_geometrys.push_back(pImmediateGeometry);

		// 1 显示列表模式	
		std::shared_ptr<Geometry> pListGeometry(new DisplayListGeometry());
		pListGeometry->SetName("DisplayListGeometry");
		_geometrys.push_back(pListGeometry);

		// 2 顶点数组模式
		std::shared_ptr<Geometry> pVertexArraysGeometry(new VertexArraysGeometry());
		pVertexArraysGeometry->SetName("VertexArraysGeometry");
		_geometrys.push_back(pVertexArraysGeometry);


		// 3 VBO 
		std::shared_ptr<Geometry> pVBOGeometry(new VBOArrayGeometry());
		pVBOGeometry->SetName("VBOArrayGeometry");
		_geometrys.push_back(pVBOGeometry);

		// 4 VBO + VAO
		std::shared_ptr<Geometry> pVAOGeometry(new VAOGeometry());
		pVAOGeometry->SetName("VAOGeometry");
		_geometrys.push_back(pVAOGeometry);

		// 5 VAO +Array
		std::shared_ptr<Geometry> pVAOArray(new VAOArrayGeometry());
		pVAOArray->SetName("VAOArrayGeometry");
		_geometrys.push_back(pVAOArray);

		// 6 VBO + VAO +EBO 
		std::shared_ptr<Geometry> pEBOGeometry(new EBOGeometry());
		pEBOGeometry->SetName("EBOGeometry");
		_geometrys.push_back(pEBOGeometry);


		for(auto itr = _geometrys.begin(); itr != _geometrys.end(); ++itr)
		{
			(*itr)->Init();
		}


		return true;
	}
	//virtual bool setup()
	//{
	//	// 1.set property 
	//	int ret = glfwInit();
	//	if(ret) return false;
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	//	return true;
	//}

	virtual bool update()
	{
		ExampleBase::update();

		glm::mat4 view;
		view = _camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(_camera->Zoom, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

		float m[16] = {0}; //用来保存当前矩阵数据  
		glMatrixMode(GL_MODELVIEW);  
		glLoadIdentity();  
		//glGetFloatv(GL_MODELVIEW_MATRIX, m);   
		////相机设置，View 变换  
		//gluLookAt(0.0, 0.0, 5.0,  
		//0.0, 0.0, 0.0,  
		//0.0, 1.0, 0.0);  
		//glGetFloatv(GL_MODELVIEW_MATRIX, m);  
		glLoadMatrixf(glm::value_ptr(view));
		//投影设置  
		glMatrixMode(GL_PROJECTION);  
		glLoadIdentity();  
		glLoadMatrixf(glm::value_ptr(projection));

	
		glMatrixMode(GL_MODELVIEW);   

		glEnable(GL_DEPTH_TEST);

	/*	for(auto itr = _geometrys.begin(); itr != _geometrys.end(); ++itr)
		{
			(*itr)->Update();
		}*/
		return true;
	}
	virtual bool render()
	{
		ExampleBase::render();
		for(auto itr = _geometrys.begin(); itr != _geometrys.end(); ++itr)
		{
			(*itr)->Draw();
		}
		return true;
	}

private:

	std::vector<std::shared_ptr<Geometry>> _geometrys;
};

Init_Static();
int _tmain(int argc, _TCHAR* argv[])
{
	DrawGeometry eb;
	Example_Start(eb,"DrawGeometry");
	return 0;
}



