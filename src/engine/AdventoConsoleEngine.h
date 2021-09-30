#if !defined (ADVENTO_CONSOLE_ENGINE_H)
#define ADVENTO_CONSOLE_ENGINE_H

#include <string>
#include <thread>
#include <Windows.h>

namespace engine
{
	typedef WORD color;

	namespace default_colors
	{
		constexpr WORD WHITE = 0x000F;
		constexpr WORD BLACK = 0x0000;
		constexpr WORD BLUE = 0x0001;
		constexpr WORD GREEN = 0x0002;
		constexpr WORD CYAN = 0x0003;
		constexpr WORD RED = 0x0004;
		constexpr WORD PURPLE = 0x0005;
		constexpr WORD YELLOW = 0x0006;
	}

	// Основний клас двигуна
	class AdventoConsoleEngine
	{
	private:
		HANDLE m_hConsoleHandle;
		HANDLE m_hConsoleHandleIn;
		SMALL_RECT m_windowRect;

	protected:
		// Ітформація про вікно
		std::string m_sAppName = "Default";
		uint16_t m_nWidth, m_nHeight;
		CHAR_INFO* m_ScreenBuffer= nullptr;

		bool m_bRunning = false;

	public: // фенкції для рендеру
		void Clear(engine::color clear_color);

	public:
		AdventoConsoleEngine();
		~AdventoConsoleEngine();
		
		bool Construct(uint16_t nWidth, uint16_t nHeight, std::string sAppName);
		void Start(); // починає обробку подій
		
		// Функції для наслідників класу
		virtual void Update(float fDeltaTime) = 0; // оновлення
		virtual void Render() = 0; // рендер (користувач користується лише особливими функціями)

	private:
		void GameLoop(); // ігровий потік
		void NativeRender(); // охоплює малювання та оновлення m_ScreenBuffer (нативно)

		void Error(const char* sMessage);
	};	
}

#endif
