#include "Emitter.h"
#include "Gizmos.h"

#include "GLFW\glfw3.h"

#include <thread>
#include <iostream>
#include "emmintrin.h"

//Empty constructor
Emitter::Emitter()
{
}

//Default constructor. Use array initialization
Emitter::Emitter(const glm::vec4& a_position, const glm::vec4& a_positionVeriance, const glm::vec4& a_direction,
	const glm::vec4& a_spread, const glm::vec4& a_secDir, const float& a_particleHealth,
	const float& a_particleHealthVariance, const int& a_emitRatePerFrame,
	const glm::vec4& a_particleSpeed /*= glm::vec4(1,1,1,1)*/)
				:
				m_position(a_position), m_positionVariance(a_positionVeriance), m_direction(a_direction),
				m_particleSpread(a_spread), m_secondDir(a_secDir), m_particleHealth(a_particleHealth), 
				m_particleHealthVariance(a_particleHealthVariance), m_emitPerFrame(a_emitRatePerFrame), 
				m_particleSpeed(a_particleSpeed)
{
}

//Destructor
Emitter::~Emitter()
{
}

void UpdatePartilce(__m256* a_particleItem, __m256* a_move, __m256* a_sDir, const float& a_sDAcc, const float& a_speed, const float& a_deltaTime)
{
	//Get the delta time for separate threads, this would allow each thread to be detached 
	//and the update with there own delta time
	//double m_previuosTime = 0;
	//double m_curretnTime = glfwGetTime();
	//double deltaTime = m_curretnTime - m_previuosTime;
	//m_previuosTime = m_curretnTime;
	//std::cout << deltaTime << std::endl;

	//Creating the max number of particles
	int maxParticles = MAX_NUM_PARTICLES / 8;
	float m_speedDeltaTime = a_speed * a_deltaTime;

	//Create union to hold temp values in
	union
	{
		__m256 items;
		float f[8];
	};

	union
	{
		__m256 sDAcc;
		float s[8];
	};

	//Update the union values to be equal to a_deltaTime
	for (int i = 0; i < 8; ++i)
	{
		f[i] = m_speedDeltaTime;
		s[i] = a_sDAcc;
	}
	__m256 newDir[MAX_NUM_PARTICLES / 8];
	for (int i = 0; i < maxParticles; ++i)
	{
		//Create a new direction from the old direction and the added direction
		newDir[i] = _mm256_add_ps(a_move[i], a_sDir[i]);
		//Get all the move values and multiply them by a_deltaTime
		//m_pos = m_direction * a_deltaTime;
		__m256 values = _mm256_mul_ps(newDir[i], items);
		//Add the movement value on to the positon of the particle
		a_particleItem[i] = _mm256_add_ps(a_particleItem[i], values);
		a_sDir[i] = _mm256_add_ps(a_sDir[i], sDAcc);
	}
}

void ReduceLife(__m256* health, const float& a_deltaTime)
{
	//Creating the max number of particles
	int maxParticles = MAX_NUM_PARTICLES / 8;

	//Create a temp __m256 to hold the delta time
	union
	{
		__m256 delta;
		float f[8];
	};

	//Assign the temp variables to delta time, this is needed as we are going to subtract
	//8 float at once
	for (int i = 0; i < 8; i++)
	{
		f[i] = a_deltaTime;
	}
	//Subtract all the values on health, do 8 per instruction
	for (int i = 0; i < maxParticles; ++i)
	{
		health[i] = _mm256_sub_ps(health[i], delta);
	}
}

//This function would be used to multi thread the draw calls
void DrawParticlesT(float* x, float* y, float* z, float* a_life, float* a_startLife, int a_start, int a_end, glm::vec4 a_nVx)
{
	int m_particleCount = 0;

	for (int i = a_start; i < a_end; ++i)
	{
		if (a_life[i] > 0)
		{
			Gizmos::addBillBoard(
				glm::vec4(x[i], y[i], z[i], 1),
				a_nVx,
				glm::vec2(0.1f, 0.1f),
				glm::mix(
					glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
					glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
					a_life[i] / a_startLife[i]));

			++m_particleCount;
		}
	}

	std::cout << m_particleCount << std::endl;
}

//Update function, check if the particle is Alive, if it is update it, else
//create a new particle
void Emitter::Update(float a_deltaTime, glm::mat4 a_camera)
{
	//Comment to stop threading 
	//Create 4 threads to update the x, y, z and life of each particle, one thread will do x, another
	//y, another z and the last will do the life.
	//std::thread thread1(UpdatePartilce, particles.x8, particles.xMove8, particles.XDrag8, m_secondDir.x, m_particleSpeed.x, a_deltaTime);
	//std::thread thread2(UpdatePartilce, particles.y8, particles.yMove8, particles.YDrag8, m_secondDir.y, m_particleSpeed.y, a_deltaTime);
	//std::thread thread3(UpdatePartilce, particles.z8, particles.zMove8, particles.ZDrag8, m_secondDir.z, m_particleSpeed.z, a_deltaTime);
	//std::thread thread4(ReduceLife, particles.life8, a_deltaTime);

	//Uncomment to update all the particles without using threading
	UpdatePartilce(particles.x8, particles.xMove8, particles.XDrag8, m_secondDir.x, m_particleSpeed.x, a_deltaTime);
	UpdatePartilce(particles.y8, particles.yMove8, particles.YDrag8, m_secondDir.y, m_particleSpeed.y, a_deltaTime);
	UpdatePartilce(particles.z8, particles.zMove8, particles.ZDrag8, m_secondDir.z, m_particleSpeed.z, a_deltaTime);
	ReduceLife(particles.life8, a_deltaTime);

	m_particlesSpawned = 0;
	m_particleCount = 0;

	////Go though all the particles 
	for (int i = 0; i < MAX_NUM_PARTICLES && m_particlesSpawned < m_emitPerFrame; ++i)
	{
		//if the particle is a nullptr then create a new particle
		if (particles.life[i] <= 0)
		{
	
			particles.life[i] = m_particleHealth + glm::linearRand(-m_particleHealthVariance, m_particleHealthVariance);
			particles.startLife[i] = m_particleHealth;
	
			//Create random start position
			glm::vec4 m_pPosition = glm::vec4(
				m_position.x + glm::linearRand(-m_positionVariance.x, m_positionVariance.x),
				m_position.y + glm::linearRand(-m_positionVariance.y, m_positionVariance.y),
				m_position.z + glm::linearRand(-m_positionVariance.z, m_positionVariance.z),
				m_positionVariance.w);
	
			//Create a direction vec3 for the particle
			glm::vec3 dir = glm::vec3(
				m_direction.x + glm::linearRand(-m_particleSpread.x, m_particleSpread.x),
				m_direction.y + glm::linearRand(-m_particleSpread.y, m_particleSpread.y),
				m_direction.z + glm::linearRand(-m_particleSpread.z, m_particleSpread.z));
	
			//Normolise the direction vector
			dir = glm::normalize(dir);
	
			//Assign new values to the particle
			particles.xMove[i] = dir.x;
			particles.yMove[i] = dir.y;
			particles.zMove[i] = dir.z;
	
			particles.XDrag[i] = 0.0f;
			particles.YDrag[i] = 0.0f;
			particles.ZDrag[i] = 0.0f;
	
			particles.x[i] = m_pPosition.x;
			particles.y[i] = m_pPosition.y;
			particles.z[i] = m_pPosition.z;
	
			++m_particlesSpawned;
		}
	}

	//Create two new threads, and have each of them drawing half the particles, not in use as it 
	//does not give any more performance as you have to wait for the first thread to finish and join,
	//this acts like if the code was using just the main thread or 1 thread
	//This is not in use as you can not draw from two threads as artifacting appears 
	//std::thread d1(DrawParticlesT, particles.x, particles.y, particles.z, particles.life, particles.startLife, 0, m_maxNumParticles * 0.5f, nVx);
	//std::thread d2(DrawParticlesT, particles.x, particles.y, particles.z, particles.life, particles.startLife, m_maxNumParticles * 0.5f, m_maxNumParticles, nVx);
	//d1.join();
	//d2.join();

	//Create a vector which faces the camera, do this here and not when creating the
	//billboard as then we don't have to times the camera matrix by position of the billboard
	//each time we make a billboard
	glm::vec3 vX(0.1f, 0, 0);
	glm::vec4 nVx = glm::vec4(vX, 0.f);
	nVx = a_camera * nVx;

	//Go though all the particles
	for (int i = 0; i < MAX_NUM_PARTICLES; ++i)
	{
		//If the particle life is greater than 0 then draw it,
		//this stops us from drawing not needed particles
		//this does however stop drawing particles if there life is 0
		//this reduces the particles count form 32768 to around 27000 - 28000
		//and gives around 1 frames per second in performance 
		if (particles.life[i] > 0)
		{
			//Draw a billboard
			//Gizmos::addBillBoard(
			//	glm::vec4(particles.x[i], particles.y[i], particles.z[i], 1),
			//	nVx,
			//	glm::vec2(0.1f, 0.1f),
			//	glm::mix(
			//		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			//		glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			//		particles.life[i] / particles.startLife[i]));

			//Use this instead of addBillBoard if the geometry code is in Gizmos.cpp.
			//The code can be found in the root dir
			Gizmos::addPoint(glm::vec4(particles.x[i], particles.y[i], particles.z[i], 1),
				glm::mix(
					glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
					glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
					particles.life[i] / particles.startLife[i]));
			
			//Increment m_particleCount
			++m_particleCount;
			
		}
	}
	
	//Print out the FPS and the particle count
	std::cout << "FPS:	" << 1 / a_deltaTime << "	ParticleCount:	" << m_particleCount << std::endl;


	//join all the update and reduce life thread back to the main thread
	//You can use .detach() to not have the threads join back to the main thread 
	//have the threads continue updating on there own
	//thread1.join();
	//thread2.join();
	//thread3.join();
	//thread4.join();
}

void Emitter::LoadTexture(unsigned int a_texture)
{
	m_texture = a_texture;
}

//Get the position of the emitter and return a vec3
glm::vec4 Emitter::GetPos() const
{
	return m_position;
}

//Get the maximum number of particles and return an int
int Emitter::GetMaxParticles() const
{
	return MAX_NUM_PARTICLES;
}

//Get the number if Alive particles and return an int
int Emitter::GetPatcileCount() const
{
	return m_particleCount;
}