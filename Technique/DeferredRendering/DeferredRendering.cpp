// DeferredRendering.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <ExampleBase.h>
//
//Init_Static();
//
//
//class DeferredRender : public ExampleBase
//{
//public:
//	virtual bool render()
//	{
//		__super::render();
//		return true;
//
//	}
//protected:
//private:
//};
//int _tmain(int argc, _TCHAR* argv[])
//{
//	DeferredRender df;
//	Example_Start(df);
//	return 0;
//}


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>


#include <iostream>
#include <string>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path, bool gammaCorrection);
void renderQuad();
void renderCube();
void  renderDebugQuad();
int g_debugFbo = -1;
int g_debugFbo_tex = -1;
bool g_use_volumeLight = false;
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

unsigned int gPosition, gNormal, gAlbedoSpec;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;



int _tmain(int argc, _TCHAR* argv[])
{
	glfwInit();
	// Initialize GLEW to setup the OpenGL Function pointers
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Deffered shading",NULL,NULL);
	glfwMakeContextCurrent(window);
	if(window == NULL)
	{
		std::cout<<"Failed to create Glfw window"<<std::endl;
		glfwTerminate();
		return -1;

	}

	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetCursorPosCallback(window,mouse_callback);
	glfwSetScrollCallback(window,scroll_callback);

	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"fail to init glad"<<std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);


	// build and compile shaders
	std::string resoucePath = FileSystem::getPath("resources/deferred_shading/");
	std::string lightvs = resoucePath + "8.1.g_buffer.vs";
	std::string lightfs = resoucePath + "8.1.g_buffer.fs";
	std::string deferred_shadingvs = resoucePath + "8.1.deferred_shading.vs";
	std::string deferred_shadingfs = resoucePath + "8.1.deferred_shading.fs";
	std::string shaderLightBoxvs = resoucePath + "8.1.deferred_light_box.vs";
	std::string shaderLightBoxfs = resoucePath + "8.1.deferred_light_box.fs";
	std::string fboDebugvs = resoucePath +"8.1.fbo_debug.vs";
	std::string fboDebugfs = resoucePath + "8.1.fbo_debug.fs";
	std::string deferred_shader_volumevs = resoucePath + "8.2.deferred_shading.vs";
	std::string deferred_shader_volumefs = resoucePath + "8.2.deferred_shading.fs";

	/*Shader shaderGeometryPass("8.1.g_buffer.vs", "8.1.g_buffer.fs");
	Shader shaderLightingPass("8.1.deferred_shading.vs", "8.1.deferred_shading.fs");
	Shader shaderLightBox("8.1.deferred_light_box.vs", "8.1.deferred_light_box.fs");*/



	Shader shaderGeomtryPass(lightvs.c_str(),lightfs.c_str());
	Shader shaderLightingPass(deferred_shadingvs.c_str(),deferred_shadingfs.c_str());
	Shader shaderLightBox(shaderLightBoxvs.c_str(),shaderLightBoxfs.c_str());
	Shader shaderFbo(fboDebugvs.c_str(),fboDebugfs.c_str());
	Shader shaderLightingPass_volume(deferred_shader_volumevs.c_str(),deferred_shader_volumefs.c_str());
	Shader* pShaderLighting = &shaderLightingPass_volume;
	

	// load models
	// -----------
	Model nanosuit(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	std::vector<glm::vec3> objectPositions;
	objectPositions.push_back(glm::vec3(-3.0,  -3.0, -3.0));
	objectPositions.push_back(glm::vec3( 0.0,  -3.0, -3.0));
	objectPositions.push_back(glm::vec3( 3.0,  -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0,  -3.0,  0.0));
	objectPositions.push_back(glm::vec3( 0.0,  -3.0,  0.0));
	objectPositions.push_back(glm::vec3( 3.0,  -3.0,  0.0));
	objectPositions.push_back(glm::vec3(-3.0,  -3.0,  3.0));
	objectPositions.push_back(glm::vec3( 0.0,  -3.0,  3.0));
	objectPositions.push_back(glm::vec3( 3.0,  -3.0,  3.0));

	// configure g-buffer framebuffer
	// ------------------------------
	unsigned int gBuffer;
	glGenFramebuffers(1,&gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);


	 // position color buffer
	glGenTextures(1,&gPosition);
	glBindTexture(GL_TEXTURE_2D,gPosition);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,SCR_WIDTH,SCR_HEIGHT,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gPosition,0);

	// gNormal 
	glGenTextures(1,&gNormal);
	glBindTexture(GL_TEXTURE_2D,gNormal);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,SCR_WIDTH,SCR_HEIGHT,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,gNormal,0);

	// gAlbedoSpec
	// gNormal 
	glGenTextures(1,&gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D,gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,SCR_WIDTH,SCR_HEIGHT,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,gAlbedoSpec,0);


	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	
	// create and attach depth buffer (renderbuffer)
	 unsigned int rboDepth;
	 glGenRenderbuffers(1,&rboDepth);
	 glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
	 glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,SCR_WIDTH,SCR_HEIGHT);
	 glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboDepth);

		 // finally check if framebuffer is complete
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			 std::cout << "Framebuffer not complete!" << std::endl;
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 // lighting info
	 // -------------
	 const unsigned int NR_LIGHTS = 32;
	 std::vector<glm::vec3> lightPositions;
	 std::vector<glm::vec3> lightColors;
	 srand(13);

	 for (unsigned int i = 0; i < NR_LIGHTS; i++)
	 {
		 // calculate slightly random offsets
		 float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		 float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		 float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		 lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		 // also calculate random color
		 float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		 float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		 float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		 lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	 }


	 // shader configuration
	 // --------------------
	 shaderLightingPass_volume.use();
	 shaderLightingPass_volume.setInt("gPosition", 0);
	 shaderLightingPass_volume.setInt("gNormal", 1);
	 shaderLightingPass_volume.setInt("gAlbedoSpec", 2);

	 shaderLightingPass.use();
	 shaderLightingPass.setInt("gPosition", 0);
	 shaderLightingPass.setInt("gNormal", 1);
	 shaderLightingPass.setInt("gAlbedoSpec", 2);


	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		
		if(g_use_volumeLight)
			pShaderLighting = &shaderLightingPass_volume;
		else
			pShaderLighting = &shaderLightingPass;

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. geometry pass: render scene's geometry/color data into gbuffer
		// -----------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;
		
		shaderGeomtryPass.use();
		shaderGeomtryPass.setMat4("projection", projection);
		shaderGeomtryPass.setMat4("view", view);
		for (unsigned int i = 0; i < objectPositions.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, objectPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			shaderGeomtryPass.setMat4("model", model);
			nanosuit.Draw(shaderGeomtryPass);
		}

		 glBindFramebuffer(GL_FRAMEBUFFER, 0);


		 // 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
		 // -----------------------------------------------------------------------------------------------------------------------
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		 pShaderLighting->use();
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, gPosition);
		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_2D, gNormal);
		 glActiveTexture(GL_TEXTURE2);
		 glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

		 // send light relevant uniforms
		 for (_ULonglong i = 0; i < lightPositions.size(); i++)
		 {
			 pShaderLighting->setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			 pShaderLighting->setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
			 // update attenuation parameters and calculate radius
			 const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			 const float linear = 0.7;
			 const float quadratic = 1.8;
			 pShaderLighting->setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			 pShaderLighting->setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);

			 if(g_use_volumeLight)
			 {
				 // then calculate radius of light volume/sphere
				 const float maxBrightness = std::max(std::max(lightColors[i].r, lightColors[i].g), lightColors[i].b);
				 float radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
				 pShaderLighting->setFloat("lights[" + std::to_string(i) + "].Radius", radius);
			 }
		 }

		 pShaderLighting->setVec3("viewPos", camera.Position);
		 // finally render quad
		 renderQuad();


		 // 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
		 // ----------------------------------------------------------------------------------
		 glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		 glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer

		 // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
		 // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
		 // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		 glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST); // 将之前渲染结果的的深度缓存拷贝到当前深度缓存
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);



		 // 3. render lights on top of scene
		 // --------------------------------

		 shaderLightBox.use();
		 shaderLightBox.setMat4("projection", projection);
		 shaderLightBox.setMat4("view", view);
		 for (unsigned int i = 0; i < lightPositions.size(); i++)
		 {
			 model = glm::mat4();
			 model = glm::translate(model, lightPositions[i]);
			 model = glm::scale(model, glm::vec3(0.125f));
			 shaderLightBox.setMat4("model", model);
			 shaderLightBox.setVec3("lightColor", lightColors[i]);
			 renderCube();
		 }

		 // debug 
		 if(g_debugFbo != -1)
		 {
			
			 shaderFbo.use();
			 glActiveTexture(GL_TEXTURE0);
			 glBindTexture(GL_TEXTURE_2D,g_debugFbo_tex);
			 renderDebugQuad();
			 glBindTexture(GL_TEXTURE2,0);
			
		 }
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}




// glfw: whenever the mouse scroll wheel scrolls, this callback is called

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;

void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1,&quadVAO);
		glGenBuffers(1,&quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices), &quadVertices,GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*) (3* sizeof(float)));

		
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int debugVAO = 0;
unsigned int debugVBO;
void renderDebugQuad()
{
	if (debugVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  0.5f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			.50f,  .50f, 0.0f, 1.0f, 1.0f,
			0.50f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1,&debugVAO);
		glGenBuffers(1,&debugVBO);
		glBindVertexArray(debugVAO);
		glBindBuffer(GL_ARRAY_BUFFER,debugVBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices), &quadVertices,GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*) (3* sizeof(float)));


	}
	glBindVertexArray(debugVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if(glfwGetKey(window,GLFW_KEY_F2) == GLFW_PRESS)
	{
		g_debugFbo++;
		if(g_debugFbo >= 3)
		{
			g_debugFbo = -1;
		}
		else if(g_debugFbo == 0)
			g_debugFbo_tex = gPosition;
		else if(g_debugFbo == 1)
			g_debugFbo_tex = gNormal;
		else if(g_debugFbo == 2)
			g_debugFbo_tex = gAlbedoSpec;
	}
	if(glfwGetKey(window,GLFW_KEY_F3) == GLFW_PRESS)
	{
		g_use_volumeLight  = !g_use_volumeLight;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}