#if !defined (ADVENTO_CONSOLE_ENGINE_H)
#define ADVENTO_CONSOLE_ENGINE_H

#include <string>
#include <thread>
#include <Windows.h>

// Як використовувати?
//
// Якщо ви використовуєте Visual Studio, то все, що вам потрібно зробити 
// - це помістити папку engine у корінний каталог вашого проекту
//
// Якщо ви використовуєте MinGW компілятор, 
// то команда для компіляції буде мати наступний вигляд
// g++ -o main.exe main.cpp engine/*.cpp -static-libgcc -static-libstdc++
//
// ВАЖЛИВО: ДВИГУН ВИКОРИСТОВУЄ WinAPI, ТОМУ ПРАЦЮЄ ЛИШЕ НА WINDOWS!!!

namespace engine
{
	typedef WORD color;

	namespace default_colors
	{	
		constexpr WORD BLACK = 0x0000;
		constexpr WORD DARK_BLUE = 0x0001;
		constexpr WORD DARK_GREEN = 0x0002;
		constexpr WORD DARK_CYAN = 0x0003;
		constexpr WORD DARK_RED = 0x0004;
		constexpr WORD DARK_PURPLE = 0x0005;
		constexpr WORD DARK_YELLOW = 0x0006;
		constexpr WORD LIGHT_GREY = 0x0007;
		constexpr WORD GREY = 0x0008;
		constexpr WORD BLUE = 0x0009;
		constexpr WORD GREEN = 0x000A;
		constexpr WORD CYAN = 0x000B;
		constexpr WORD RED = 0x000C;
		constexpr WORD PURPLE = 0x000D;
		constexpr WORD YELLOW = 0x000E;
		constexpr WORD WHITE = 0x000F;
	}

	namespace pixel_types
	{
		constexpr WCHAR EMPTY = 0x0000;
		constexpr WCHAR SOLID = 0xDB;
	}

	struct KeyData
	{
		bool bPressed, bHeld, bReleased;
	};

	template<class type>
	struct Vector_2d
	{
		type x, y;
	};

	typedef Vector_2d<int> Vector_i2d;
	typedef Vector_2d<float> Vector_f2d;

	// Основний клас двигуна
	class AdventoConsoleEngine
	{
	private:
		HANDLE m_hConsoleHandle;
		HANDLE m_hConsoleHandleIn;
		SMALL_RECT m_windowRect;

		HWND m_WindowName;

	protected:
		// Ітформація про вікно
		std::string m_sAppName = "Default";
		uint16_t m_nWidth, m_nHeight;
		CHAR_INFO* m_ScreenBuffer = nullptr;

		DWORD m_dwNumRead;
		INPUT_RECORD m_InputRecordBuf[128];

		// клавіші/кнопки
		KeyData m_KeysData[256];

		// позиція курсора миші
		Vector_i2d m_vMousePosition;

		bool m_bRunning = false;

	public:
		// Очищає екран
		void Clear(WCHAR c = 0xDB, engine::color clr = 0x000F);	

		Vector_i2d GetMousePosition() const;
		int GetMouseX() const;
		int GetMouseY() const;

		Vector_i2d GetWindowSize() const;
		int GetWindowSizeX() const;
		int GetWindowSizeY() const;

		KeyData IsButton(const int button) const;
		
		// ПОвертає true якщо вікно сфокусоване
		bool IsWindowFocused() const;

		// Малює точку в заданих коорденитах
		void DrawPoint(Vector_i2d vPosition, WCHAR c = 0xDB, engine::color renderColor = 0x000F);

		// Малює точку в заданих координатах
		void DrawPoint(int x, int y, WCHAR c = 0xDB, engine::color renderColor = 0x000F);

		// Заповнює задану площину
		void Fill(int x1, int y1, int x2, int y2, WCHAR c = 0xDB, engine::color renderColor = 0x000F);

		// Виводить строку на екран
		void DrawString(Vector_i2d viPosition, const std::string& sData);

		// Малює форму кола
		void DrawCircle(Vector_i2d viPosition, int nRadius, WCHAR c = 0xDB, engine::color renderColor = 0x000F);
		void FillCircle(Vector_i2d viPosition, int nRadius, WCHAR c = 0xDB, engine::color renderColor = 0x000F);

	public:
		AdventoConsoleEngine();
		~AdventoConsoleEngine();
		
		bool Construct(uint16_t nWidth, uint16_t nHeight, uint16_t nPixelScale, std::string sAppName);
		void Start(); // починає обробку подій
		
		// Функції для наслідників класу
		virtual void HandleInput(float fDeltaTime) = 0;
		virtual void Update(float fDeltaTime) = 0; // оновлення
		virtual void Render() = 0; // рендер (користувач користується лише особливими функціями)

	private:
		void GameLoop(); // ігровий потік
		void NativeRender(); // охоплює малювання та оновлення m_ScreenBuffer (нативно)

		void Error(const char* sMessage);

		void Wrap(int &x, int &y);
	};	
}

#endif
