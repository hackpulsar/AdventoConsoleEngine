#include "engine/AdventoConsoleEngine.h"
#include <vector>
#include <algorithm>

class Game :
	public engine::AdventoConsoleEngine
{
private:
	struct Particle
	{
		engine::Vector_f2d vPos;
		engine::Vector_f2d vVel;
	};

public:
	Game()
	{
		
	}

	~Game()
	{  }

	void HandleInput(float fDeltaTime) override
	{
		if (IsWindowFocused())
		{
			if (IsButton(VK_LBUTTON).bPressed)
			{
				for (int i = 0; i < 50; ++i)
				{
					m_vParticles.push_back(Particle{ 
						{(float)GetMouseX(), (float)GetMouseY()}, 
						{float(rand() % 100 - 50), float(rand() % 100 - 50)}}
					);
				}
			}
		}	
	}

	void Update(float fDeltaTime) override
	{
		for (int i = 0; i < m_vParticles.size(); ++i)
		{
			Particle& p = m_vParticles[i];
			p.vPos.x += p.vVel.x * fDeltaTime;
			p.vPos.y += p.vVel.y * fDeltaTime;
		}

		auto it = std::remove_if(m_vParticles.begin(), m_vParticles.end(), 
			[&](Particle p)
			{ 
				return p.vPos.x < 0 || p.vPos.x > GetWindowSizeX() || 
					p.vPos.y < 0 || p.vPos.y > GetWindowSizeY();
			}
		);

		if (it != m_vParticles.end())
			m_vParticles.erase(it);
	}

	void Render() override
	{
		Clear(engine::pixel_types::EMPTY, engine::default_colors::CYAN);
		
		for (int i = 0; i < m_vParticles.size(); ++i)
		{
			Particle& p = m_vParticles[i];
			Fill(p.vPos.x, p.vPos.y, p.vPos.x + 1, p.vPos.y + 1, 
					engine::pixel_types::SOLID, engine::default_colors::YELLOW);
		}
	}

private:
	std::vector<Particle> m_vParticles;
};

int main(void)
{
	Game game;
	if (game.Construct(64, 64, 16, "Example application"))
		game.Start();
	return 0;
}
