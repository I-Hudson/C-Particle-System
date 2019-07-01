#include "Particle.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//Empty constructor 
Particle::Particle()
{
}

//Constructor
Particle::Particle(const float & a_lifeSpan, const glm::vec4 & a_postion, const glm::vec4 & a_particleDir,
				  const unsigned int& a_textureID, const int& a_speed)
				  :
				  m_life(a_lifeSpan), m_position(a_postion), m_particleDir(a_particleDir),
				  m_textureID(a_textureID), m_speed(a_speed), m_currentLife(0)
{
}

//Destructor
Particle::~Particle()
{
}

//Uppdate all the needed values NOT USED 
bool Particle::UpdateAndIsAlive(const float& a_deltaTime)
{
	if (m_currentLife < m_life)
	{
		//Update the current life value
		m_currentLife += a_deltaTime;

		//Update the position of the particle
		m_position += m_particleDir * m_speed * a_deltaTime;

		//Update the size of the particle
		//m_size += m_sizeChange;

		//Change the colour of the particle depending on the life
		//m_colour = glm::mix(m_startColour, m_colourChange, 1 / (m_life / m_currentLife));

		return true;
	}
	else
	{
		m_currentLife = 0;
		return false;
	}
}

bool Particle::UpdateAndIsAliveC(const float& a_deltaTime, glm::vec4& a_startC, glm::vec4& a_endC)
{
	if (m_currentLife < m_life)
	{
		//Update the current life value
		m_currentLife += a_deltaTime;

		//Update the position of the particle
		m_position += m_particleDir * m_speed * a_deltaTime;

		//Update the size of the particle
		//m_size += m_sizeChange;

		//Change the colour of the particle depending on the life
		m_colour = glm::mix(a_startC, a_endC, 1 / (m_life / m_currentLife));
		//Change the alpha of the particle
		m_colour.a = glm::mix(1.0f, 0.0f, 1 / (m_life / m_currentLife));

		return true;
	}
	else
	{
		m_currentLife = 0;
		return false;
	}
}

//Get the position of the particle by returning a vec3
glm::vec4 Particle::GetPosition() const
{
	//Return m_position 
	return m_position;
}

//Get the colour of the particle by returning a vec4
glm::vec4 Particle::GetColour() const
{
	//return m_colour
	return m_colour;
}