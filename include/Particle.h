#pragma once

#include <glm/ext.hpp>

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

class Particle
{
	//Current life of the particle
	float m_currentLife;

	//The life span of the particle
	float m_life;

	//Position of the particle
	glm::vec4 m_position;
	//Direction of the particle
	glm::vec4 m_particleDir;

	//Speed of the particle
	int m_speed;

	//Rotation of the particle
	//float m_rotation;
	//Rotation velocity of the particle
	//float m_rotationVelocity;

	//Start size of the particle
	//glm::vec2 m_size;
	//End size of the particle
	//glm::vec2 m_sizeChange;

	//Start colour of the particle
	glm::vec4 m_colour;
	//real start colour
	//glm::vec4 m_startColour;
	//End colour of the particle
	//glm::vec4 m_colourChange;

	//Textuure of the particle
	unsigned int m_textureID;

	int padding;

public:
	//Empty constructor
	Particle();

	//Constructor
	Particle(const float & a_lifeSpan, const glm::vec4 & a_postion, const glm::vec4 & a_particleDir,
		const unsigned int& a_textureID, const int& a_speed);

	//Destructor
	virtual ~Particle();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Functions
	///////////////////////////////////////////////////////////////////////////////////////////////////

	//Update the particle 
	bool UpdateAndIsAlive(const float& a_deltaTime);

	//Update the particle and interpolate between two colours pass though
	bool Particle::UpdateAndIsAliveC(const float& a_deltaTime, glm::vec4& a_startC, glm::vec4& a_endC);

	//Get the position of the particle
	glm::vec4 GetPosition() const;

	//Get the colour of the particle
	glm::vec4 GetColour() const;
};
#endif // !__PARTICLE_H__
