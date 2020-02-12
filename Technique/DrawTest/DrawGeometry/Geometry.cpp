#include "Geometry.h"


Geometry::Geometry(void)
{
}


Geometry::~Geometry(void)
{
}

//--------------------------------------------------------------------------------------
// ����ģʽ
 
	ImmediateGeometry::ImmediateGeometry()
	{
	}

	ImmediateGeometry::~ImmediateGeometry()
	{

	}
void ImmediateGeometry::Init()
{
		GenCylinder(_points,_indexs,1.0f,2.0f,20,20);
}
void ImmediateGeometry::Draw()
{
	glBegin(GL_TRIANGLES);
			
		 for(int i = 0; i < _points.size(); ++i)
		 {
			 glVertex3f(_points[i].x,_points[i].y,_points[i].z);
			 if(i%2)
				glColor3f(0.0f,1.0f,0.0f);
			 else
				glColor3f(1.0f,1.0f,0.0f);
		 }
		glEnd();

	//glBegin(GL_TRIANGLES);
	//		 glVertex3f(0.0,1.0,0.0);
	//		 glVertex3f(-1.0,-1.0,0.0);
	//		 glVertex3f(1.0,-1.0,0.0);
 //   glEnd();



}
void ImmediateGeometry::Destroy()
{

}

 //��ʾ�б�
//-------------------------------------------------------------------------------

void DisplayListGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);
	
	_listId = glGenLists(1);
	 glNewList(_listId,GL_COMPILE);

	if(!glIsList(_listId))
	{
	  assert(0);
	}

	glTranslatef(2.10f,0.0f,0.0f);
	glBegin(GL_TRIANGLES);
		 for(int i = 0; i < _points.size(); ++i)
		 {
			 glVertex3f(_points[i].x,_points[i].y,_points[i].z);
			 if(i%2)
				glColor3f(1.0f,0.0f,0.0f);
			 else
				glColor3f(0.0f,1.0f,0.0f);
		 }
	glEnd();
	
	glEndList();

	
}
void DisplayListGeometry::Draw()
{
	glCallList(_listId);
}
void DisplayListGeometry::Destroy()
{
	glDeleteLists(_listId,1);

}

// ��������
//--------------------------------------------------------------------------------
void VertexArraysGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);
}
void VertexArraysGeometry::Draw()
{

	glTranslatef(2.2f,0.0f,0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.5f,0.0f,0.0f);
	// ���ݶ�������
	glVertexPointer(
		3, // ÿ�������ά��
		GL_FLOAT, // ÿ���������������
		0, // ����֮��ļ�϶
		_points.data()); // ����ָ��

	glDrawArrays(GL_TRIANGLES,0,_points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	
}
void VertexArraysGeometry::Destroy()
{

}

 
//VBO
//----------------------------------------------------------------------
void VBOArrayGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);
	
	// create vbo 
	glGenBuffers(1,&_vbo_Id);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo_Id);
	// copy data
	glBufferData(GL_ARRAY_BUFFER,3*sizeof(GLfloat)*_points.size(),_points.data(),GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

}
void VBOArrayGeometry::Draw()
{
	glTranslatef(3.0f,0.0f,0.0f);
	glColor3f(0.0f,0.0f,0.34f);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER,_vbo_Id);
	glVertexPointer(3,GL_FLOAT,0,0);
	glDrawArrays(GL_TRIANGLES,0,_points.size());
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glDisableClientState(GL_VERTEX_ARRAY);

}
void VBOArrayGeometry::Destroy()
{
	glDeleteBuffers(1,&_vbo_Id);
}

 //VAO +VBO
//-----------------------------------------------------------------------------------------

void VAOGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);

	glGenVertexArrays(1,&_vao_id);
	glGenBuffers(1,&_vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo_id);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*_points.size(),_points.data(),GL_STATIC_DRAW);
	glBindVertexArray(_vao_id);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0); // �����Ե�vao
	//glVertexPointer(3,GL_FLOAT,0,0);
	glEnableVertexAttribArray(0); // ����0��id��������Ϊ��������
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

}
void VAOGeometry::Draw()
{

	glTranslatef(4.0f,0.0f,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	//glEnableVertexAttribArray(0);
	glBindVertexArray(_vao_id);
	glDrawArrays(GL_TRIANGLES,0,_points.size());
	glBindVertexArray(0);


}
void VAOGeometry::Destroy()
{
	glDeleteBuffers(1,&_vbo_id);
	glDeleteVertexArrays(1,&_vao_id);
		

}
 
// VAO+Array����
//------------------------------------------------

 
void VAOArrayGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);

	glGenVertexArrays(1,&_vao_id);
	glBindVertexArray(_vao_id);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),_points.data()); // �����Ե�vao
	//glVertexPointer(3,GL_FLOAT,0,0);
	glEnableVertexAttribArray(0); // ����0��id��������Ϊ��������
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

}
void VAOArrayGeometry::Draw()
{

	glTranslatef(6.0f,0.0f,0.0f);
	glColor3f(0.5f,1.0f,0.0f);
	//glEnableVertexAttribArray(0);
	glBindVertexArray(_vao_id);
	glDrawArrays(GL_TRIANGLES,0,_points.size());
	glBindVertexArray(0);


}
void VAOArrayGeometry::Destroy()
{
	glDeleteVertexArrays(1,&_vao_id);
}

// VAO+VBO+EBO
// -----------------------------------------------------------------------------
/*
�ܽ᣺һ���glEnableClientState/glDisableClientState,glVertexPointer 
Ϊ��Shader״̬�´���vboʹ�ã�ʹ��glvertexAttribPointer/glEnableVertexAttribArray
���ڴ���shader��Ĳ�������;��Ȼ��ʹ�ø���API��Ҳ�����滻����Shader����ʹ��vbo(�������Ϊ����˾ɵĽӿڣ� 
*/
#define USE_GLVERTEXATTRIBPOINTER 1

void EBOGeometry::Init()
{
	std::vector<glm::vec3> indexPoint = GenCylinder(_points,_indexs,1.0f,2.0f,20,20);

		
	glGenBuffers(1,&_vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo_id);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*indexPoint.size(),indexPoint.data(),GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&_ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*_indexs.size(),_indexs.data(),GL_STATIC_DRAW);
	

	glGenVertexArrays(1,&_vao_id);
	glBindVertexArray(_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo_id);
	//glEnableClientState(GL_VERTEX_ARRAY);
	
#if USE_GLVERTEXATTRIBPOINTER
	glEnableVertexAttribArray(0); // ����0��id��������Ϊ��������
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0); // ��vbo���Ե�vao
#else
	glVertexPointer(3,GL_FLOAT,0,(GLvoid*)0);
#endif 
	//glEnableVertexAttribArray(1); // ����1��id��������Ϊ��������
	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(GLuint),(GLvoid*)0); // ��ebo���Ե�vao 1��������
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ebo_id);
	glVertexPointer(3,GL_UNSIGNED_INT,0,(GLvoid*)0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	



}
void EBOGeometry::Draw()
{
		glTranslatef(5.0f,0.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		//glEnableVertexAttribArray(0);
		

		glBindVertexArray(_vao_id);
#if USE_GLVERTEXATTRIBPOINTER
	 
		glDrawElements(GL_TRIANGLES,_indexs.size(),GL_UNSIGNED_INT,0);
		glDisableClientState( GL_VERTEX_ARRAY);
		 
#else
	    glEnableClientState( GL_VERTEX_ARRAY );
		//glDrawArrays(GL_TRIANGLES,0,_points.size());
		glDrawElements(GL_TRIANGLES,_indexs.size(),GL_UNSIGNED_INT,0);
		glDisableClientState( GL_VERTEX_ARRAY);
		
#endif 
	glBindVertexArray(0);
		

}
void EBOGeometry::Destroy()
{
		glDeleteBuffers(1,&_vbo_id);
		glDeleteBuffers(1,&_ebo_id);
		glDeleteVertexArrays(1,&_vao_id);
}
	
 