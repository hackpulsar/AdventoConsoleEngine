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
		DrawString({10, 10}, "First advento console string");
	}

private:

};

int main(void)
{
	Demo demo;
	if (demo.Construct(48, 48, 16, "Demo application"))
		demo.Start();

	return 0;
}
