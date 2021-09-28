#include "AdventoConsoleEngine.h"

#include <cstring>
#include <iostream>
#include <thread>

namespace engine
{
	AdventoConsoleEngine::AdventoConsoleEngine(uint16_t nWidth, uint16_t nHeight, std::string sAppName)
		: m_nWidth(nWidth), m_nHeight(nHeight), m_sAppName(sAppName)
	{
		m_hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // get console handle
		m_ScreenBuffer = new CHAR_INFO[nWidth * nHeight];
		std::memset(m_ScreenBuffer, 0, nWidth * nHeight);

		// ініціцалізація консолі
		COORD screenSize = { (short)m_nWidth, (short)m_nHeight };
		SetConsoleScreenBufferSize(m_hConsoleHandle, screenSize); // розмір
		SetConsoleActiveScreenBuffer(m_hConsoleHandle); // встановити активне вікно
		
		// налаштування шрифта
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 8;
		cfi.dwFontSize.Y = 8;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(m_hConsoleHandle, &info);
		
		wcscpy_s(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(m_hConsoleHandle, false, &cfi); // встановити налаштування шрифту

		m_windowRect = { 0, 0, short(m_nWidth - 1), short(m_nHeight - 1) };
		SetConsoleWindowInfo(m_hConsoleHandle, TRUE, &m_windowRect); // встановити позицію та розмів вікна
	}

	AdventoConsoleEngine::~AdventoConsoleEngine()
	{
		delete[] m_ScreenBuffer;
	}

	void AdventoConsoleEngine::NativeRender()
	{	
		WriteConsoleOutput(m_hConsoleHandle, m_ScreenBuffer, { short(m_nWidth), short(m_nHeight) }, { 0, 0 }, &m_windowRect);
	}

	void AdventoConsoleEngine::GameLoop()
	{
		using namespace std::chrono_literals;

		while (m_bRunning)
		{
			// TODO: make it a method
			std::memset(m_ScreenBuffer, 0, m_nWidth * m_nHeight); // очистити екран

			this->Update();
			this->Render();

			m_ScreenBuffer[0].Char.UnicodeChar = 0xB2;
			m_ScreenBuffer[0].Attributes = 0x000F;

			this->NativeRender();
	
			std::this_thread::sleep_for(0.005s); // визначений період часу між кадрами
		}
	}
	

	void AdventoConsoleEngine::Start()
	{
		m_bRunning = true;
		std::thread gameLoop(&AdventoConsoleEngine::GameLoop, this);

		std::cin.get();

		m_bRunning = false;
		exit(0);
		//std::cout << "Finished!" << std::endl;
	}
}
