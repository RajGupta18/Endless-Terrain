#include "Window.h"

Window::Window() {
	width = 800;
	height = 600;

	for (int i = 0; i < 1024; i++) Keys[i] = false;
	Firstmove = true;
	xChange = yChange = 0.f;
}

Window::Window(GLint w, GLint h) {
	width = w;
	height = h;

	for (int i = 0; i < 1024; i++) Keys[i] = false;
	Firstmove = true;
	xChange = yChange = 0.f;
}

int Window::Init() {
	if (!glfwInit()) {
		std::cout << "Failed to initialise GLFW!" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Terrain Gen", NULL, NULL);
	if (!mainWindow) {
		std::cout << "Window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Handle KeyBoard inputs...
	CreateKeyCallbacks();

	//Handle Cursor Position Input...
	CreateCursorCallbacks();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW Initialisation Failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	//Setup ViewPort Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//call this function on every window resize by registering it...
	//glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	//reference to this window class
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::CreateKeyCallbacks() {
	glfwSetKeyCallback(mainWindow, HandleKeys);
}

void Window::CreateCursorCallbacks() {
	glfwSetCursorPosCallback(mainWindow, HandleCursor);
}

void Window::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->Keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->Keys[key] = false;
		}
	}
}

void Window::HandleCursor(GLFWwindow *window, double xPos, double yPos) {
	Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->Firstmove) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->Firstmove = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

//callback function when window is resized by user or else...
void Window::framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
	Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->bufferWidth = w;
	theWindow->bufferHeight = h;
	std::cout << theWindow->bufferWidth<< " , " << theWindow->bufferHeight<< std::endl;
}

void Window::ChangePloygonMode() {
	if (glfwGetKey(mainWindow, GLFW_KEY_1) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(mainWindow, GLFW_KEY_2) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLfloat Window::getXchange() {
	GLfloat temp = xChange;
	xChange = 0.f;
	return temp;
}

GLfloat Window::getYchange() {
	GLfloat temp = yChange;
	yChange = 0.f;
	return temp;
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}