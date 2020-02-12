#include "Shape.h"


 std::vector<glm::vec3> GenCylinder(std::vector<glm::vec3>& triangle_points,std::vector<unsigned int>& indexs,float radius, float height, int x_dimesion,int y_dimesion)
{
	std::vector<glm::vec3> all_points;
	std::vector<unsigned int> all_indexs;

	for(int j = 0; j < y_dimesion; ++j)
	{
		float h = height / y_dimesion * j; 
		for(int i = 0; i < x_dimesion; ++i)
		{
#if 1
			float rad = PI * 2.0 * i / x_dimesion;
			float x = radius * sin(rad);
			float y = radius * cos(rad);
#else 
			float x = i * 2 * radius/ x_dimesion;
			float y = j * 2 * radius / y_dimesion;
			h = 0;
#endif 
			all_points.push_back(glm::vec3(x,h,y));
		}
	}
	
	for(int j = 1; j < y_dimesion; ++j)
	{
		/*
		c ______ d
		 |\	    |
		 | \    |
		 |  \ 	|
		 |   \  |
		 |    \ | 
		a|_____\|b
		*/
		for(int i = 0; i < x_dimesion ; ++i)
		{
			int a = y_dimesion * (j -1) + i;
			int b = y_dimesion * (j -1) + (i + 1)%x_dimesion;
			int c = y_dimesion * j + i;
			int d = y_dimesion * j + (i + 1)%x_dimesion;
			all_indexs.push_back(a);
			all_indexs.push_back(b);
			all_indexs.push_back(c);
			
			all_indexs.push_back(b);
			all_indexs.push_back(d);
			all_indexs.push_back(c);

			triangle_points.push_back(all_points[a]);
			triangle_points.push_back(all_points[b]);
			triangle_points.push_back(all_points[c]);

			triangle_points.push_back(all_points[b]);
			triangle_points.push_back(all_points[d]);
			triangle_points.push_back(all_points[c]);
		}
	}
	indexs = all_indexs;

	return all_points;
}
