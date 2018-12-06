#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <string>
#include <vector>

#include <learnopengl\shader_m.h>

#ifdef   USE_MY
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	 // Tangent
    glm::vec3 Tangent;
    // Bitangent
    glm::vec3 Bitangent;

};
// ����ÿ����������
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};
	typedef std::vector<Vertex> Vertices;
	typedef std::vector<unsigned int> Indices; 
	typedef std::vector<Texture> Textures;

class Mesh
{

	

public:

	~Mesh(void);


	/* ��������*/
	 Vertices _vertices;
	 Indices _indices;
	 Textures _textures;

	/* function*/
	Mesh(Vertices vertices, Indices indices, Textures textures);
	void Draw(Shader shader);
private:
	/* ��Ⱦ����*/
	unsigned int _VAO, _VBO,_EBO;

	/* ����*/
	void setupMesh();

};

#endif 
