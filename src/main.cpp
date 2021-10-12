#include "engine/AdventoConsoleEngine.h"
#include "engine/Entity.h"

#include "Blocks.h"
#include <vector>

class Player
	: public engine::Entity
{
public:
	Player(engine::AdventoConsoleEngine* window)
		:engine::Entity(window, { 54.f, 100.f }, { 10.f, 10.f }, { 20, 5 })
	{  }

	~Player()
	{  }

	void handleInput(float fDeltaTime) override
	{
		if (m_Window->IsButton((int)engine::Leter_Keys::A).bHeld)
		{
			m_vfVelocity.x = -45.f;
		}
		else if (m_Window->IsButton((int)engine::Leter_Keys::D).bHeld)
		{
			m_vfVelocity.x = 45.f;
		}
		else
		{
			m_vfVelocity = { 0, 0 };
		}
	}

	void update(float fDeltaTime) override
	{
		if (m_vfPosition.x < 0) m_vfPosition.x = 0;
		else if ((int)m_vfPosition.x + m_viSize.x > m_Window->GetWindowSizeX())
		{ m_vfPosition.x = float(m_Window->GetWindowSizeX() - m_viSize.x); }

		m_vfPosition.x += m_vfVelocity.x * fDeltaTime;
		m_vfPosition.y += m_vfVelocity.y * fDeltaTime;
	}

	void render() const override
	{
		m_Window->Fill(m_vfPosition.x, m_vfPosition.y, m_vfPosition.x + m_viSize.x, m_vfPosition.y + m_viSize.y,
				engine::pixel_types::SOLID, engine::default_colors::BLUE);
	}

private:

};

class Demo
	: public engine::AdventoConsoleEngine
{
public:
	Demo()
		: m_Player(new Player(this))
	{
		GenerateNewLevel();
	}

	~Demo()
	{
		delete m_Player;

		for (auto& block : m_vBlocks)
			delete block;
		m_vBlocks.clear();
	}

	void HandleInput(float fDeltaTime) override
	{
		if (IsWindowFocused())
		{
			// stuff
		
			m_Player->handleInput(fDeltaTime);
		}
	}

	void Update(float fDeltaTime) override
	{
		m_Player->update(fDeltaTime);
	}

	void Render() override
	{
		Clear(engine::pixel_types::SOLID, engine::default_colors::YELLOW);
		
		for (const auto& block : m_vBlocks)
			block->render();

		m_Player->render();
	}
private:
	Player* m_Player;

	std::vector<engine::Entity*> m_vBlocks;

private:
	void GenerateNewLevel()
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 7; ++x)
			{
				m_vBlocks.push_back(new BasicBlock(this, { float(2 + x * (16 + 2)), float(5 + y * (8 + 2)) }));
			}
		}
	}

};

int main(void)
{
	Demo demo;
	if (demo.Construct(128, 112, 8, "Demo application"))
		demo.Start();

	return 0;
}

