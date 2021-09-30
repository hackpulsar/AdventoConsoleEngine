#include "engine/AdventoConsoleEngine.h"
#include <iostream>

class Game :
	public engine::AdventoConsoleEngine
{
public:
	Game()
	{
		
	}

	~Game()
	{  }

	void Update(float fDeltaTime) override
	{

	}

	void Render() override
	{
		Clear(engine::default_colors::CYAN);
	}

private:

};

int main(void)
{
	Game game;
	if (game.Construct(128, 120, "Example application"))
		game.Start();
	return 0;
}
