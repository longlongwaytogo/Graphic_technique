#include "StdAfx.h"
#include "Mesh.h"

#include <string>

#ifdef USE_MY
Mesh::Mesh(Vertices vertices, Indices indices, Textures textures)
	:_vertices(vertices),
	_indices(indices),
	_textures(textures)
{

	setupMesh();
}


void Mesh::setupMesh()
{

	glGenVertexArrays(1,&_VAO);
	glGenBuffers(1,&_VBO);
	glGenBuffers(1,&_EBO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER,_VBO);
	glBufferData(GL_ARRAY_BUFFER,_vertices.size()*sizeof(Vertex),&_vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indices.size()*sizeof(unsigned int),&_indices[0],GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex, Normal));

	// texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);

}

Mesh::~Mesh(void)
{
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for(unsigned int i = 0; i < _textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = _textures[i].type;
		if(name == "texture_diffuse")
			number = std::to_string((unsigned __int64)diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string((unsigned __int64)specularNr++);

		shader.setFloat(("material." + name  + number).c_str(),i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].id);
	}

	// »æÖÆÍø¸ñ
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,NULL);
	glBindVertexArray(0);

}

#endif 