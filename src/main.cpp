#include "Blocks.h"
#include <vector>
#include <algorithm>

static bool AABB_Collision(const engine::Entity* e1, const engine::Entity* e2)
{
	auto e1_pos = e1->getPosition();
	auto e2_pos = e2->getPosition();
	
	auto e1_size = e1->getSize();
	auto e2_size = e2->getSize();

	if (e1_pos.x < e2_pos.x + e2_size.x &&
		e1_pos.x + e1_size.x > e2_pos.x &&
		e1_pos.y < e2_pos.y + e2_size.y &&
		e1_pos.y + e1_size.y > e2_pos.y)
	{
		return true;
	}
	return false;
}

class Ball
	: public engine::Entity
{
public:
	Ball(engine::AdventoConsoleEngine* window)
		: engine::Entity(window, { 0.f, 0.f }, { 25.f, 25.f }, { 4, 4 })
	{  }

	void init() override
	{
		this->setPosition({ float(m_Window->GetWindowSizeX() / 2), float(m_Window->GetWindowSizeY() / 2) });
	}

	~Ball()
	{  }

	void handleInput(float fDeltaTime) override
	{  }

	void update(float fDeltaTime) override
	{
		if (m_vfPosition.x < 0 || m_vfPosition.x + m_viSize.x > m_Window->GetWindowSizeX())
			m_vfVelocity.x = -m_vfVelocity.x;
		if (m_vfPosition.y < 0 || m_vfPosition.y + m_viSize.y > m_Window->GetWindowSizeY())
			m_vfVelocity.y = -m_vfVelocity.y;

		m_vfPosition.x += m_vfVelocity.x * fDeltaTime;
		m_vfPosition.y += m_vfVelocity.y * fDeltaTime;
	}

	void render() const override
	{
		m_Window->DrawCircle({ (int)m_vfPosition.x, (int)m_vfPosition.y }, 
				m_viSize.x, engine::pixel_types::SOLID, engine::default_colors::BLUE);
	}

private:
};

class Player
	: public engine::Entity
{
public:
	Player(engine::AdventoConsoleEngine* window)
		:engine::Entity(window, { 54.f, 100.f }, { 10.f, 10.f }, { 20, 5 })
	{  }

	~Player()
	{  }

	void init() override
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
		: engine::AdventoConsoleEngine(), m_Player(new Player(this)), m_Ball(new Ball(this))
	{
		
	}

	~Demo()
	{
		delete m_Player;
		delete m_Ball;

		for (auto& block : m_vBlocks)
			delete block;
		m_vBlocks.clear();
	}

	void AppInit() override
	{
		m_Ball->init();
		GenerateNewLevel();
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
		//TODO: delect ball collision with bricks
		
		if (m_Ball->getPosition().x >= m_Player->getPosition().x &&
				m_Ball->getPosition().x + m_Ball->getSize().x < m_Player->getPosition().x + m_Player->getSize().x)
		{
			if (m_Ball->getPosition().y + m_Ball->getSize().y >= m_Player->getPosition().y)
			{
				// player collision with ball
				if (m_Ball->getVelocity().y > 0)
					m_Ball->setVelocity({ m_Ball->getVelocity().x, -m_Ball->getVelocity().y });
			}
		}

		auto it = std::remove_if(m_vBlocks.begin(), m_vBlocks.end(),
				[&](engine::Entity* e) { return AABB_Collision(e, m_Ball); });

		if (it != m_vBlocks.end())
		{
			if (m_Ball->getPosition().x + m_Ball->getSize().x > (*it)->getPosition().x)
				m_Ball->setVelocity({ -m_Ball->getVelocity().x, m_Ball->getVelocity().y });
			else if (m_Ball->getPosition().x - m_Ball->getSize().x < (*it)->getPosition().x + (*it)->getSize().x)
				m_Ball->setVelocity({ -m_Ball->getVelocity().x, m_Ball->getVelocity().y });

			if (m_Ball->getPosition().y + m_Ball->getSize().y > (*it)->getPosition().y)
				m_Ball->setVelocity({ -m_Ball->getVelocity().x, -m_Ball->getVelocity().y });
			else if (m_Ball->getPosition().y - m_Ball->getSize().y < (*it)->getPosition().y + (*it)->getSize().y)
				m_Ball->setVelocity({ -m_Ball->getVelocity().x, -m_Ball->getVelocity().y });

			m_vBlocks.erase(it);
		}

		m_Player->update(fDeltaTime);
		m_Ball->update(fDeltaTime);
	}

	void Render() override
	{
		Clear(engine::pixel_types::SOLID, engine::default_colors::YELLOW);
		
		for (const auto& block : m_vBlocks)
			block->render();

		m_Player->render();
		m_Ball->render();
	}
private:
	Player* m_Player;
	Ball* m_Ball;

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

