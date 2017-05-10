// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// test opengl evnironment

//#include <GL/GL.h>
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <iostream>


void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

void render();
int main( int argc, char** argv ) {  

	// 1.set property 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

	
	// 2.create window 
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL",nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 3. setup eventcallback
	glfwSetKeyCallback(window, key_callback);

	// 4.init opengl 
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// 5.set viewport
	glViewport(0, 0, 800, 600);

	// 6.run loop 
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
		// 7. render
		render();
	}

	// 8.terminate
	glfwTerminate();


	return 0;
}  

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC, 我们就把WindowShouldClose设置为true, 关闭应用
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}