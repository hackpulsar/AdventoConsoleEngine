#if !defined (ADVENTO_CONSOLE_ENGINE_H)
#define ADVENTO_CONSOLE_ENGINE_H

#include <string>
#include <chrono>
#include <thread>
#include <Windows.h>

namespace engine
{
	// Основний клас двигуна
	class AdventoConsoleEngine
	{
	private:
		HANDLE m_hConsoleHandle;
		SMALL_RECT m_windowRect;

	protected:
		// Ітформація про вікно
		std::string m_sAppName = "Default";
		uint16_t m_nWidth, m_nHeight;
		CHAR_INFO* m_ScreenBuffer= nullptr;

		bool m_bRunning = false;

	public:
		AdventoConsoleEngine(uint16_t nWidth, uint16_t nHeight, std::string sAppName);
		~AdventoConsoleEngine();

		void Start(); // починає обробку подій
		
		// Функції для наслідників класу
		virtual void Update() = 0; // оновлення
		virtual void Render() const = 0; // рендер (користувач користується лише особливими функціями)

	private:
		void GameLoop(); // ігровий потік
		void NativeRender(); // охоплює малювання та оновлення m_ScreenBuffer (нативно)
	};	
}

#endif
