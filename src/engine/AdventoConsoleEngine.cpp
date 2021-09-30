#include "AdventoConsoleEngine.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>

namespace engine
{
	AdventoConsoleEngine::AdventoConsoleEngine()
	{
		m_hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // get console handle
		m_hConsoleHandleIn = GetStdHandle(STD_INPUT_HANDLE); // get console handle
	}

	bool AdventoConsoleEngine::Construct(uint16_t nWidth, uint16_t nHeight, std::string sAppName)
	{	
		if (m_hConsoleHandleIn == INVALID_HANDLE_VALUE)
			Error("Bad handle");

		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_sAppName = sAppName;

		// ініціцалізація консолі
		COORD screenSize = { (short)m_nWidth, (short)m_nHeight };
		if (!SetConsoleScreenBufferSize(m_hConsoleHandle, screenSize)) // розмір
			Error("SetConsoleScreenBufferSize");
		if (!SetConsoleActiveScreenBuffer(m_hConsoleHandle)) // встановити активне вікно
			Error("SetConsoleActiveScreenBuffer");
		
		// налаштування шрифта
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 8;
		cfi.dwFontSize.Y = 8;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		
		wcscpy_s(cfi.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(m_hConsoleHandle, false, &cfi)) // встановити налаштування шрифту
			Error("SetCurrentConsoleFontEx");

		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(m_hConsoleHandle, &info);	

		m_windowRect = { 0, 0, short(m_nWidth - 1), short(m_nHeight - 1) };
		if (!SetConsoleWindowInfo(m_hConsoleHandle, TRUE, &m_windowRect)) // встановити позицію та розмів вікна
			Error("SetConsoleWindowInfo");

		if (!SetConsoleMode(m_hConsoleHandleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
			Error("SetConsoleMode");

		m_ScreenBuffer = new CHAR_INFO[nWidth * nHeight];
		std::memset(m_ScreenBuffer, 0, nWidth * nHeight);

		SetConsoleCtrlHandler((PHANDLER_ROUTINE)FALSE, TRUE);

		return true;
	}

	AdventoConsoleEngine::~AdventoConsoleEngine()
	{
		delete[] m_ScreenBuffer;
	}

	void AdventoConsoleEngine::Error(const char* sMessage)
	{
		char buf[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		MessageBox(nullptr, TEXT(buf), TEXT(sMessage), MB_OK);
	}

	void AdventoConsoleEngine::Clear(engine::color clear_color)
	{
		for (int i = 0; i < int(m_nWidth * m_nHeight); ++i)
		{
			m_ScreenBuffer[i].Char.UnicodeChar = 0xDB;
			m_ScreenBuffer[i].Attributes = clear_color;
		}
	}

	void AdventoConsoleEngine::NativeRender()
	{	
		WriteConsoleOutput(m_hConsoleHandle, m_ScreenBuffer, { short(m_nWidth), short(m_nHeight) }, { 0, 0 }, &m_windowRect);
	}

	void AdventoConsoleEngine::GameLoop()
	{
		using namespace std::chrono_literals;

		auto t1 = std::chrono::system_clock::now();
		auto t2 = std::chrono::system_clock::now();

		while (m_bRunning)
		{
			t2 = std::chrono::system_clock::now();
			std::chrono::duration<float> delta = t2 - t1;
			t1 = t2;

			this->Update(delta.count());
			this->Render();
			this->NativeRender();

			// update title
			std::string sTitle = m_sAppName + " - FPS: " + std::to_string(1.0f / delta.count());
			SetConsoleTitle(sTitle.c_str());
		}
	}
	

	void AdventoConsoleEngine::Start()
	{
		m_bRunning = true;
		std::thread gameLoop(&AdventoConsoleEngine::GameLoop, this);

		//m_bRunning = false;
		gameLoop.join();
		//exit(0);
	}
}
