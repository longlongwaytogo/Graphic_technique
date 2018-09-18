#include "Geometry.h"


Geometry::Geometry(void)
{
}


Geometry::~Geometry(void)
{
}

//--------------------------------------------------------------------------------------
// 立即模式
 
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

void VertexArraysGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);
}
void VertexArraysGeometry::Draw()
{

	glTranslatef(2.2f,0.0f,0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.5f,0.0f,0.0f);
	// 传递顶点数据
	glVertexPointer(
		3, // 每个顶点的维度
		GL_FLOAT, // 每个顶点的数据类型
		0, // 顶点之间的间隙
		_points.data()); // 顶点指针

	glDrawArrays(GL_TRIANGLES,0,_points.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	
}
void VertexArraysGeometry::Destroy()
{

}

 

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

 

	void VAOGeometry::Init()
	{
		GenCylinder(_points,_indexs,1.0f,2.0f,20,20);

		glGenVertexArrays(1,&_vao_id);
		glGenBuffers(1,&_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER,_vbo_id);
		glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*_points.size(),_points.data(),GL_STATIC_DRAW);
		glBindVertexArray(_vao_id);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0); // 绑定属性到vao
		glEnableVertexAttribArray(0); // 启用0号id的属性作为输入数据
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
 
 

void EBOGeometry::Init()
{
	GenCylinder(_points,_indexs,1.0f,2.0f,20,20);

		
	glGenBuffers(1,&_vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo_id);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*3*_points.size(),_points.data(),GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&_ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*_indexs.size(),_indexs.data(),GL_STATIC_DRAW);
	

	glGenVertexArrays(1,&_vao_id);
	glBindVertexArray(_vao_id);
	glEnableVertexAttribArray(0); // 启用0号id的属性作为输入数据
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0); // 绑定vbo属性到vao
	
	//glEnableVertexAttribArray(1); // 启用1号id的属性作为输入数据
	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(GLuint),(GLvoid*)0); // 绑定ebo属性到vao 1号属性上
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	



}
void EBOGeometry::Draw()
{
		glTranslatef(5.0f,0.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		//glEnableVertexAttribArray(0);
		//glEnableClientState( GL_VERTEX_ARRAY );

		glBindVertexArray(_vao_id);
		//glDrawArrays(GL_TRIANGLES,0,_points.size());
		glDrawElements(GL_TRIANGLES,_indexs.size(),GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		//glDisableClientState( GL_VERTEX_ARRAY);

}
void EBOGeometry::Destroy()
{
		glDeleteBuffers(1,&_vbo_id);
		glDeleteBuffers(1,&_ebo_id);
		glDeleteVertexArrays(1,&_vao_id);
}
	
 