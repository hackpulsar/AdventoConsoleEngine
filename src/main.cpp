#include "engine/AdventoConsoleEngine.h"

class Demo
	: public engine::AdventoConsoleEngine
{
public:
	Demo()
	{  }

	~Demo()
	{  }

	void HandleInput(float fDeltaTime) override
	{

	}

	void Update(float fDeltaTime) override
	{

	}

	void Render() override
	{
		Clear(engine::pixel_types::SOLID, engine::default_colors::YELLOW);
	}

private:

};

int main(void)
{
	Demo demo;
	if (demo.Construct(128, 120, "Demo application"))
		demo.Start();

	return 0;
}
