#include "MyApplication.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include "TextureManager.h"

#include <iostream>


#define DEFAULT_SCREENWIDTH 1280
#define DEFAULT_SCREENHEIGHT 720

MyApplication::MyApplication()
{

}

MyApplication::~MyApplication()
{

}

bool MyApplication::onCreate(int a_argc, char* a_argv[])
{
	// initialise the Gizmos helper class
	Gizmos::create(65534, 65534 * 2);

	//initialise TextureManager class
	TextureManager* textureManager =  TextureManager::CreateInstance();
	
	unsigned int texIDJPG = textureManager->LoadTexture("./images/1.jpg");

	//Create a new emitter
	//This emitter's last parameter is for the speed 
	//of the particles on the  x, y and z axis. This is 1 on all axis
	//by default to change add a glm::vec4
	emitter = new Emitter(
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),		//Position
		glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),		//Position variance
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),		//Direction
		glm::vec4(0.5f, 0.0f, 0.5f, 0.0f),		//Spread
		glm::vec4(0.03f, 0.0f, 0.0f, 0.0f),		//Second direction (force/wind)
		6,										//Base life span
		0.5f,									//Life span variance
		256										//Emit rate per frame									
	/*glm::vec4(1,1,1,0)*/	);				//Particle speed (x, y, z));

	emitter->LoadTexture(texIDJPG);

	//textureManager->ReleaseTexture(texIDJPG);

	// create a world-space matrix for a camera
	m_cameraMatrix = glm::inverse( glm::lookAt(glm::vec3(10,10,10),glm::vec3(0,0,0), glm::vec3(0,1,0)) );
	
	// create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, DEFAULT_SCREENWIDTH/(float)DEFAULT_SCREENHEIGHT, 0.1f, 1000.0f);

	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f,0.25f,0.25f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void MyApplication::onUpdate(float a_deltaTime)
{
	// update our camera matrix using the keyboard/mouse
	Utility::freeMovement( m_cameraMatrix, a_deltaTime, 10 );

	// clear all gizmos from last frame
	Gizmos::clear();
	
	// add an identity matrix gizmo
	Gizmos::addTransform( glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1) );

	// add a 20x20 grid on the XZ-plane
	for ( int i = 0 ; i < 21 ; ++i )
	{
		Gizmos::addLine( glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
		
		Gizmos::addLine( glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
	}

	//Update emitter
	emitter->Update(a_deltaTime, m_cameraMatrix);

	// quit our application when escape is pressed
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		quit();
	}
}

void MyApplication::onDraw()
{
	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = glm::inverse( m_cameraMatrix );
	
	// draw the gizmos from this frame
	Gizmos::draw(viewMatrix, m_projectionMatrix, m_cameraMatrix);
}

void MyApplication::onDestroy()
{
	// clean up anything we created
	Gizmos::destroy();
	TextureManager::DestoryInstance();
}

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
	// explicitly control the creation of our application
	MyApplication* app = new MyApplication();
	
	if (app->create("Test Project",DEFAULT_SCREENWIDTH,DEFAULT_SCREENHEIGHT,argc,argv) == true)
		app->run();
		
	// explicitly control the destruction of our application
	delete app;

	return 0;
}