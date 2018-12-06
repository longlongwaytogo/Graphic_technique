// ModelLoading.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl\shader_m.h>
#include <learnopengl\camera.h>
#if !USE_MY_MODEL 
#include <learnopengl\model.h>
#else

#include "model.h"
#endif 
#include <learnopengl\filesystem.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// setttings 
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;





int _tmain(int argc, _TCHAR* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation 
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL",NULL,NULL);
	if(NULL == window)
	{
		std::cout << "Failed to create GLFW window." <<std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window,scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// glad laod all OpenGL function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load OpenGL Function !" << std::endl;
		return -1;

	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	std::string resoucePath = FileSystem::getPath("resources/shaders/3.model_loading/");

	Shader ourShader((resoucePath + "1.model_loading.vs").c_str(), (resoucePath + "1.model_loading.fs").c_str());

	// load models

	Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));	

	// draw in wireFrame
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// rend loop

	while(!glfwWindowShouldClose(window))
	{
		// pre-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

		lastFrame = currentFrame;

		// intput 

		processInput(window);

		// render
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		
		// view/ porjection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH /(float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection",projection);
		ourShader.setMat4("view",view);

		// render the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model,glm::vec3(0.2f,0.2f,0.2f));
		ourShader.setMat4("model",model);
		ourModel.Draw(ourShader);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();


	return 0;
}



void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
	if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD,deltaTime);
	if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD,deltaTime);
	if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT,deltaTime);
	if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT,deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
