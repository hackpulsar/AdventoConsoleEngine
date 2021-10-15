#include "Entity.h"

namespace engine
{
	Entity::Entity(AdventoConsoleEngine* window, Vector_f2d vfPosition, Vector_f2d vfVelocity, Vector_i2d viSize)
		: m_Window(window), m_vfPosition(vfPosition), m_vfVelocity(vfVelocity), m_viSize(viSize)
	{  }

	Entity::~Entity()
	{  }

	void Entity::init()
	{  }

	void Entity::setAlive(bool value) { this->m_bAlive = value; }

	Vector_f2d Entity::getPosition() { return m_vfPosition; }
	Vector_f2d Entity::getVelocity() { return m_vfVelocity; }

	void Entity::setPosition(Vector_f2d viPosition) { m_vfPosition = viPosition; }
	void Entity::setVelocity(Vector_f2d viVelocity) { m_vfVelocity = viVelocity; }

	Vector_i2d Entity::getSize() const { return m_viSize; }
	bool Entity::isAlive() const { return m_bAlive; }

	void Entity::handleInput(float fDeltaTime) 
	{  }

	void Entity::render() const
	{  }
}
