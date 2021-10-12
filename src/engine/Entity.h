#if !defined (ENTITY_H)
#define  ENTITY_H

#include "AdventoConsoleEngine.h"

namespace engine
{
	class Entity
	{
	protected:
		AdventoConsoleEngine* m_Window = nullptr;

		Vector_f2d m_vfPosition;
		Vector_f2d m_vfVelocity;
		Vector_i2d m_viSize;

	public:
		Entity() = default;
		Entity(AdventoConsoleEngine* window, Vector_f2d vfPosition, Vector_f2d vfVelocity, Vector_i2d viSize);
		virtual ~Entity();
		
		Vector_f2d& getPosition();
		Vector_f2d& getVelocity();

		Vector_i2d getSize() const;
		
		virtual void handleInput(float fDeltaTime);
		virtual void update(float fDeltaTime) = 0;
		virtual void render() const;
	};
}

#endif // ENTITY_H
