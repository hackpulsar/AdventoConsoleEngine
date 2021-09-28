#include "engine/AdventoConsoleEngine.h"
#include <iostream>

class Game :
	public engine::AdventoConsoleEngine
{
public:
	Game()
		: engine::AdventoConsoleEngine(128, 128, "Example application")
	{
		
	}

	~Game()
	{  }

	void Update() override
	{

	}

	void Render() const override
	{
		//std::cout << "Running..." << std::endl;
	}

private:

};

int main(void)
{
	Game game;
	game.Start();
	return 0;
}
