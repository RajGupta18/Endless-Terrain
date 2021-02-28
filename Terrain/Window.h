#ifndef __TGEN_WINDOW_H__
#define __TGEN_WINDOW_H__
#include<iostream>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint w, GLint h);
	int Init();
	bool WindowClose() { return glfwWindowShouldClose(mainWindow); }
	void SwapBuffers() { glfwSwapBuffers(mainWindow); }
	void ChangePloygonMode();

	GLint GetBufferWidth() { return bufferWidth; }
	GLint GetBufferHeight() { return bufferHeight; }

	bool* getsKeys() { return Keys; }
	GLfloat getXchange();
	GLfloat getYchange();

	~Window();
private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//keys state store...
	bool Keys[1024];

	GLfloat lastX, lastY;
	GLfloat xChange, yChange;
	bool Firstmove;

	void CreateKeyCallbacks();
	void CreateCursorCallbacks();
	static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void HandleCursor(GLFWwindow *window, double xPos, double yPos);
	static void framebuffer_size_callback(GLFWwindow* window, int w, int h);
};
#endif