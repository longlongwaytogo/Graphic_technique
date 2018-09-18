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
	int _xDimession; // 维度方向细分数目
	int _yDimession; // 精度方向细分数目
};



 void GenCylinder(std::vector<glm::vec3>& triangle_points,std::vector< unsigned int>& indexs,float radius, float height, int x_dimesion,int y_dimesion);

#endif 