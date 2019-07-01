#ifndef __Application_H_
#define __Application_H_

#include "Application.h"
#include <glm/glm.hpp>
#include "Emitter.h"
#include <thread>
#include <emmintrin.h>

#define MAX_NUMBER_PARTICLE 32767

// Derived application class that wraps up all globals neatly
class MyApplication : public Application
{
public:

	MyApplication();
	virtual ~MyApplication();

	Emitter* emitter;

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;

	int particlesSpawned = 0;
	int particlesStartHealth = 10;
	int emitPerFrame = 128;

	//std::thread thread1;
	//std::thread thread2;
	//std::thread thread3;
	//std::thread thread4;
	//
	//struct Particles
	//{
	//	union 
	//	{
	//		float x[MAX_NUMBER_PARTICLE];
	//		__m256 x8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//
	//	union
	//	{
	//		float xMove[MAX_NUMBER_PARTICLE];
	//		__m256 xMove8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//
	//	union
	//	{
	//		float y[MAX_NUMBER_PARTICLE];
	//		__m256 y8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//
	//	union
	//	{
	//		float yMove[MAX_NUMBER_PARTICLE];
	//		__m256 yMove8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//
	//	union
	//	{
	//		float z[MAX_NUMBER_PARTICLE];
	//		__m256 z8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//
	//	union
	//	{
	//		float zMove[MAX_NUMBER_PARTICLE];
	//		__m256 zMove8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//
	//	union
	//	{
	//		float life[MAX_NUMBER_PARTICLE];
	//		__m256 life8[MAX_NUMBER_PARTICLE / 8];
	//	};
	//} particles;
};

#endif // __Application_H_