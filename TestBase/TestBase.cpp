// TestBase.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define  USE_GLEW 1

//#include <GL/GL.h>

#include <ExampleBase.h>





Init_Static();
int _tmain(int argc, _TCHAR* argv[])
{
	ExampleBase eb;
	Example_Start(eb,"testBase");
	return 0;
}

