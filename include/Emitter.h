#pragma once

#include <glm\ext.hpp>
#include <intrin.h>

#ifndef __EMITTER_H__
#define __EMITTER_H__

#define MAX_NUM_PARTICLES 32768

//	Power of 2 List
//	2	||	4		||	8		||	16		||	32		||	64		||	128		||	256
//	512	||	1024	||	2048	||	4096	||	8192	||	16384	||	32768	||	65534
//update

class Emitter
{
	//Variables for the emitter
	glm::vec4 m_position;
	glm::vec4 m_positionVariance;
	glm::vec4 m_direction;
	glm::vec4 m_particleSpread;
	glm::vec4 m_secondDir;
	glm::vec4 m_particleSpeed;

	//Store the current number of particles
	int m_particleCount;

	//Store the number of particles to be emitted each frame
	int m_emitPerFrame;
	//Store the number of particles which have been emitted
	int m_particlesSpawned;

	//Store the base health of each particle
	float m_particleHealth;
	//Store the variance of the health for each particle
	float m_particleHealthVariance;

	unsigned int m_texture = 0;

	//This struct will be used in SIMD (Single instruction, multiple data) operations
	//Particle struct
	struct Particles
	{
		//X position
		union
		{
			float x[MAX_NUM_PARTICLES];
			__m256 x8[MAX_NUM_PARTICLES / 8];
		};

		//X Direction position
		union
		{
			float xMove[MAX_NUM_PARTICLES];
			__m256 xMove8[MAX_NUM_PARTICLES / 8];
		};

		//Y position
		union
		{
			float y[MAX_NUM_PARTICLES];
			__m256 y8[MAX_NUM_PARTICLES / 8];
		};

		//Y Direction position
		union
		{
			float yMove[MAX_NUM_PARTICLES];
			__m256 yMove8[MAX_NUM_PARTICLES / 8];
		};

		//Z position
		union
		{
			float z[MAX_NUM_PARTICLES];
			__m256 z8[MAX_NUM_PARTICLES / 8];
		};

		//Z Direction position
		union
		{
			float zMove[MAX_NUM_PARTICLES];
			__m256 zMove8[MAX_NUM_PARTICLES / 8];
		};

		//Life of the particle 
		union
		{
			float life[MAX_NUM_PARTICLES];
			__m256 life8[MAX_NUM_PARTICLES / 8];
		};

		//Start life of the particles
		union
		{
			float startLife[MAX_NUM_PARTICLES];
			__m256 StartLife8[MAX_NUM_PARTICLES / 8];
		};

		//X second Direction (Force)
		union
		{
			float XDrag[MAX_NUM_PARTICLES];
			__m256 XDrag8[MAX_NUM_PARTICLES / 8];
		};

		//Y second Direction (Force)
		union
		{
			float YDrag[MAX_NUM_PARTICLES];
			__m256 YDrag8[MAX_NUM_PARTICLES / 8];
		};

		//Z second Direction (Force)
		union
		{
			float ZDrag[MAX_NUM_PARTICLES];
			__m256 ZDrag8[MAX_NUM_PARTICLES / 8];
		};
	} particles;

public:

	//Default constructor
	Emitter();

	//Constructor for the emitter. This constructor uses a default argument. This allows 
	//nothing to inputed and for a value which has already been set to take its place. The
	//default value here is for the speed of the particle. The value is set to 1 on all axis
	Emitter(const glm::vec4& a_position, const glm::vec4& a_positionVeriance, const glm::vec4& a_direction,
			const glm::vec4& a_spread, const glm::vec4& a_secDir, const float& a_particleHealth, 
			const float& a_particleHealthVariance, const int& a_emitRatePerFrame,
			const glm::vec4& a_particleSpeed = glm::vec4(1, 1, 1, 0));

	//Destructor
	virtual				~Emitter();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Functions
	///////////////////////////////////////////////////////////////////////////////////////////////////

	//Update function to check if all the particle are alive and to update them 
	virtual void		Update(float a_deltaTime, glm::mat4 a_camera);
	virtual void		LoadTexture(unsigned int a_texture);

	//Get the position of the emitter
	glm::vec4			GetPos() const;

	//Get the minimax number of particles 
	int					GetMaxParticles() const;

	//Get current number of alive particles
	int					GetPatcileCount() const;
};
#endif // !__EMITTER_H__
