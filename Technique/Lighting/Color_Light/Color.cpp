// Color.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "stdafx.h"


#define  USE_GLEW 1

//#include <GL/GL.h>

#include <ExampleBase.h>
#include <learnopengl/shader.h>
#include <glm/glm.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string shader_vs =  
	"#version 330 core \n "
	"layout (location = 0) in vec3 position; \n "
	"layout (location = 1) in vec3 normal; \n"
"uniform mat4 model; \n "
"uniform mat4 view; \n "
"uniform mat4 projection; \n "
"out vec3 Normal; "
" out vec3 FramePos; "
"void main() \n "
"{ \n "
"	gl_Position = projection * view * model * vec4(position, 1.0f); \n "
" FramePos =vec3(model * vec4(position,1.0f)); "
" Normal = normal; "
"} \n ";




std::string shader_fs = 
"#version 330 core \n "
"out vec4 color; \n "
"in vec3 Normal; \n"
" in vec3 FramePos; \n"
"uniform vec3 lightPos; "
"uniform vec3 viewPos; "
"uniform vec3 objectColor; \n "
"uniform vec3 lightColor; \n "

"void main() \n"
"{  \n"
//"	color = vec4(lightColor * objectColor, 1.0f); \n"
" vec3 norm = normalize(Normal); "
" vec3 lightDir = normalize(lightPos - FramePos); "
"float diff = max(dot(norm, lightDir), 0.0); "
" vec3 diffuse = diff * lightColor;  "
" float ambientStrength = 0.1f; \n"
" vec3 ambient = ambientStrength * lightColor;  "
" vec3 result = (ambient+ diffuse) * objectColor ;  "
" color = vec4(result, 1.0f);  " 
"} \n ";

std::string shader_fs2 = 	"#version 330 core\n "
"	out vec4 color; \n "

"void main() \n "
"{"
	"color = vec4(1.0f);\n " // Set alle 4 vector values to 1.0f
"}\n ";




const GLuint WIDTH = 800, HEIGHT = 600;

// Camera

GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame





class ColorExample :public ExampleBase
{
public:

	ColorExample()
	{
		m_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		m_model = glm::mat4();
		m_model = glm::translate(m_model, m_lightPos);
		m_model = glm::scale(m_model, glm::vec3(0.2f)); 
	}
	~ColorExample()
	{
		glDeleteBuffers(1,&m_VBO);
		glDeleteVertexArrays(1,&m_containerVAO);
		glDeleteVertexArrays(1,&m_lightVAO);
	}


	virtual bool initExample()
	{
		glEnable(GL_DEPTH_TEST);

		m_lightingShader.setShaderSource(shader_vs,shader_fs);
		m_lamp.setShaderSource(shader_vs,shader_fs2);


		GLfloat vertices[] = {
			// back
			-0.5f, -0.5f, -0.5f,  0.0f,0.0f,-1.0f,
			0.5f, -0.5f, -0.5f,   0.0f,0.0f,-1.0f,
			0.5f,  0.5f, -0.5f,   0.0f,0.0f,-1.0f,
			0.5f,  0.5f, -0.5f,   0.0f,0.0f,-1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,0.0f,-1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,0.0f,-1.0f,

			// front 
			-0.5f, -0.5f,  0.5f,     0.0f,0.0f,1.0f,
			0.5f, -0.5f,  0.5f,      0.0f,0.0f,1.0f,
			0.5f,  0.5f,  0.5f,      0.0f,0.0f,1.0f,
			0.5f,  0.5f,  0.5f,		0.0f,0.0f,1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,0.0f,1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f,0.0f,1.0f,

			// left
			-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,0.0f,
			-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,0.0f,
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,0.0f,
			-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,0.0f,
			-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,0.0f,
			-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,0.0f,

			// right
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,

			// bottom
			-0.5f, -0.5f, -0.5f,0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,0.0f, -1.0f,  0.0f,

			// top 
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
			0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,
			0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
		};
		// First, set the container's VAO (and VBO)
	
		glGenVertexArrays(1, &m_containerVAO);
		glGenBuffers(1, &m_VBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_containerVAO);
		// Position attribute
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid * )0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);

		// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
		
		glGenVertexArrays(1, &m_lightVAO);
		glBindVertexArray(m_lightVAO);
		// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		// Set the vertex attributes (only position data for the lamp))
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		return true;
	}

	virtual bool update()
	{
		return 	 __super::update();
		

	}
	virtual bool render()
	{
		
		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		m_lightingShader.use();
		// Don't forget to 'use' the corresponding shader program first (to set the uniform)
		GLint objectColorLoc = glGetUniformLocation(m_lightingShader.Program(), "objectColor");
		GLint lightColorLoc  = glGetUniformLocation(m_lightingShader.Program(), "lightColor");
		GLint lightPosLoc = glGetUniformLocation(m_lightingShader.Program(),"lightPos");
		GLint viewPosLoc = glGetUniformLocation(m_lightingShader.Program(),"viewPos");


		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f); // Also set light's color (white)
		glUniform3f(lightPosLoc,m_lightPos.x,m_lightPos.y,m_lightPos.z);
	    glUniform3f(viewPosLoc,     _camera->Position.x, _camera->Position.y, _camera->Position.z);


		// Create camera transformations
		glm::mat4 view;
		view = _camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(_camera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(m_lightingShader.Program(), "model");
		GLint viewLoc  = glGetUniformLocation(m_lightingShader.Program(),  "view");
		GLint projLoc  = glGetUniformLocation(m_lightingShader.Program(),  "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the container (using container's vertex attributes)
		glBindVertexArray(m_containerVAO);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		m_lamp.use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(m_lamp.Program(), "model");
		viewLoc  = glGetUniformLocation(m_lamp.Program(), "view");
		projLoc  = glGetUniformLocation(m_lamp.Program(), "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, m_lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(m_lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		return true;

	}

private:

	


private:
	glm::vec3 m_lightPos;
	glm::mat4 m_model;

	GLuint m_VBO;
	GLuint m_containerVAO;
	GLuint m_lightVAO;

	Shader m_lightingShader;
	Shader m_lamp;

};



Init_Static();
int _tmain(int argc, _TCHAR* argv[])
{
	ColorExample eb;
	Example_Start(eb,"ColorTest");
	return 0;
}
