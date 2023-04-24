#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "Scene.h"
#include "Camera.h"
#include "AssimpModel.h"

////////////////////////////////////////////////////////////////////////////////

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Objects to render
	static Scene* gameScene;
	static GLuint shaderAssimp;
	static AssimpModel* assimpModel;

	// Act as Constructors and desctructors 
	static bool initializeProgram(GLFWwindow* window);
	static bool initializeObjects();
	static bool initializeObjectsFromAssimp(const char* path);
	
	static void cleanObjects();
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback(float deltaTime);
	static void displayCallback(GLFWwindow*);

	// helper to reset the camera
	static void resetCamera();

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);
};

////////////////////////////////////////////////////////////////////////////////

#endif