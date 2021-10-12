#include <string>
#if !defined (BLOCKS_H)
#define BLOCKS_H

#include "engine/Entity.h"

class BasicBlock
	: public engine::Entity
{
public:
	BasicBlock(engine::AdventoConsoleEngine* window, engine::Vector_f2d vfPosition)
		: Entity(window, vfPosition, { 0.f, 0.f }, { 16, 8 })
	{  }

	void update(float fDeltaTime) override
	{  }

	void render() const override
	{
		m_Window->Fill(m_vfPosition.x, m_vfPosition.y, m_vfPosition.x + m_viSize.x, m_vfPosition.y + m_viSize.y,
				engine::pixel_types::SOLID, engine::default_colors::RED);
	}
};

class CounterBlock
	: public engine::Entity
{
public:
	CounterBlock(engine::AdventoConsoleEngine* window, engine::Vector_f2d vfPosition, int nCount)
		: Entity(window, vfPosition, { 0.f, 0.f }, { 16, 8 }), m_nCount(nCount)
	{  }

	int& getCount() { return m_nCount; }

	void update(float fDeltaTime) override
	{  }

	void render() const override
	{
		m_Window->Fill(m_vfPosition.x, m_vfPosition.y, m_vfPosition.x + m_viSize.x, m_vfPosition.y + m_viSize.y,
				engine::pixel_types::SOLID, engine::default_colors::RED);

		m_Window->DrawString(
			{
				int(m_vfPosition.x + m_viSize.x / 2 - int(std::to_string(m_nCount).length() / 2)), 
				int(m_vfPosition.y + m_viSize.y / 2) 
			},
			std::to_string(m_nCount)
		); // render count
	}

private:
	int m_nCount;
};

#endif // BLOCKS_H
