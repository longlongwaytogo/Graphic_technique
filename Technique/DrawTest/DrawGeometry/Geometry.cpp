#include "Geometry.h"


Geometry::Geometry(void)
{
}


Geometry::~Geometry(void)
{
}

//--------------------------------------------------------------------------------------
// Á¢¼´Ä£Ê½
 
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

 