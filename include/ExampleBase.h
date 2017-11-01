#ifndef __GT_EXAMPLE_BASE_INC_H__
#define __GT_EXAMPLE_BASE_INC_H__


#include <iostream>
#if USE_GLEW 
#include <GL/glew.h>

#else 
#include <glad/glad.h>
#endif 
#include <glfw/glfw3.h>

#include <learnopengl/camera.h>
#include <glm/glm.hpp>
class ExampleBase
{
public:
	ExampleBase():_window(NULL),_camera(NULL)
	{
		
		for(int i = 0; i < 1024; ++i)
			_keys[i] = false;
	}
	~ExampleBase()
	{
		if(_camera)
			delete _camera;
	}
	virtual bool start(const char* windowName = "LearnOpenGL")
	{
		_windowName = windowName;
		init();
		run();
		shutdown();
		return true;
	}

	virtual bool initExample()
	{
		return true;
	}
	virtual bool init()
	{
		setup();
		createWindow();
		setviewPort();
		setEvent();
		initExample();
		return true;
	}

	virtual bool setup()
	{
		// 1.set property 
		int ret = glfwInit();
		if(ret) return false;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
		return true;
	}
	virtual bool createWindow()
	{
		// 2.create window 
		_window = glfwCreateWindow(800, 600, _windowName,nullptr, nullptr);
		glfwMakeContextCurrent(_window);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}


		// 4.init opengl 
#if    USE_GLEW
		glewExperimental = GL_TRUE;

		if(glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return -1;
		}
#endif 

		return true;
	}

	virtual bool setviewPort()
	{

		_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		// 5.set viewport
		glViewport(0, 0, 800, 600);
		return true;

	}
	virtual void setEvent()
	{
		// 3. setup eventcallback
		glfwSetKeyCallback(_window, key_callback);
		glfwSetCursorPosCallback(_window, mouse_callback);
		glfwSetScrollCallback(_window, scroll_callback);
	}

	virtual bool update()
	{
		// Camera controls
		if (_keys[GLFW_KEY_W])
			_camera->ProcessKeyboard(FORWARD, _deltaTime);
		if (_keys[GLFW_KEY_S])
			_camera->ProcessKeyboard(BACKWARD, _deltaTime);
		if (_keys[GLFW_KEY_A])
			_camera->ProcessKeyboard(LEFT, _deltaTime);
		if (_keys[GLFW_KEY_D])
			_camera->ProcessKeyboard(RIGHT, _deltaTime);

		return true;

	}
	virtual bool render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		return true;

	}


	

	

	virtual void run()
	{
		// 6.run loop 
		while(!glfwWindowShouldClose(_window))
		{

			GLfloat currentFrame = glfwGetTime();
			_deltaTime = currentFrame - _lastTime;
			_lastTime = currentFrame;

			glfwPollEvents();
			// 7. render
			update();
			render();
			glfwSwapBuffers(_window);
			
		}
	}

	virtual bool shutdown()
	{
		// 8.terminate
		glfwTerminate();

		return true;

	}

	virtual void keyEvent(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				_keys[key] = true;
			else if (action == GLFW_RELEASE)
				_keys[key] = false;
		}
		// 当用户按下ESC, 我们就把WindowShouldClose设置为true, 关闭应用
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		

	}

	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		

		if(_base)
			_base->keyEvent(window,key,scancode,action,mode);
	}

	virtual void mouseEvent(GLFWwindow* window, double xpos, double ypos)
	{
		static bool firstMouse = true;
		if (firstMouse)
		{
			_lastX = xpos;
			_lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - _lastX;
		GLfloat yoffset = _lastY - ypos;  // Reversed since y-coordinates go from bottom to left

		_lastX = xpos;
		_lastY = ypos;

		_camera->ProcessMouseMovement(xoffset, yoffset);
	}
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if(_base)
			_base->mouseEvent(window,xpos,ypos);
	}

	virtual void scrollEvent(GLFWwindow* window, double xoffset, double yoffset)
	{
			_camera->ProcessMouseScroll(yoffset);
	}
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if(_base)
			_base->scrollEvent(window,xoffset,yoffset);
	}

	static int Base_main( int argc, char** argv) 
	{  
		ExampleBase eb;
		eb.start();
		return 0;
	}  

protected:

	GLFWwindow * _window;
	const char* _windowName;
	Camera* _camera;
	float _deltaTime;
	float _lastTime;

	double _lastX;
	double _lastY;

	bool    _keys[1024];
public:
	static ExampleBase* _base;
	
};

#define  Init_Static() ExampleBase* ExampleBase::_base = NULL;
#define  Example_Start(eb,windowName) { \
		ExampleBase::_base = &eb; \
		if(windowName)    \
		 eb.start(windowName); \
		else  \
		  eb.start(); \
	}
	
#endif 