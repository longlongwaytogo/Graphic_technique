#ifndef __GT_CYLINDER_INC_H__
#define __GT_CYLINDER_INC_H__

#include <vector>
#include <glm/glm.hpp>
#define PI 3.141592654

class Sphere
{
public:
	Sphere(float radius,int x_dimesion, int y_dimesion)
	{
	}
	void GenMesh()
	{

	}

private:
	float _radius; 
	int _xDimession; // ά�ȷ���ϸ����Ŀ
	int _yDimession; // ���ȷ���ϸ����Ŀ
};



 void GenCylinder(std::vector<glm::vec3>& triangle_points,std::vector< unsigned int>& indexs,float radius, float height, int x_dimesion,int y_dimesion);

#endif 