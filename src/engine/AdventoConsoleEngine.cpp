#include "AdventoConsoleEngine.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <math.h>

namespace engine
{
	AdventoConsoleEngine::AdventoConsoleEngine()
	{
		m_hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // get console handle
		m_hConsoleHandleIn = GetStdHandle(STD_INPUT_HANDLE); // get console handle

		std::memset(m_KeysData, 0, 256 * sizeof(KeyData));
	}

	bool AdventoConsoleEngine::Construct(int nWidth, int nHeight, int nPixelScale, std::wstring sAppName)
	{	
		if (m_hConsoleHandleIn == INVALID_HANDLE_VALUE)
			Error(L"Bad handle");

		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_sAppName = sAppName;
		m_WindowName = GetForegroundWindow();

		m_windowRect = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(m_hConsoleHandle, TRUE, &m_windowRect);

		// ініціцалізація консолі
		COORD screenSize = { (short)m_nWidth, (short)m_nHeight };
		if (!SetConsoleScreenBufferSize(m_hConsoleHandle, screenSize)) // розмір
			Error(L"SetConsoleScreenBufferSize");
		if (!SetConsoleActiveScreenBuffer(m_hConsoleHandle)) // встановити активне вікно
			Error(L"SetConsoleActiveScreenBuffer");
		
		// налаштування шрифта
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = nPixelScale;
		cfi.dwFontSize.Y = nPixelScale;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		
		wcscpy_s(cfi.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(m_hConsoleHandle, false, &cfi)) // встановити налаштування шрифту
			Error(L"SetCurrentConsoleFontEx");

		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(m_hConsoleHandle, &info);	

		m_windowRect = { 0, 0, short(m_nWidth - 1), short(m_nHeight - 1) };
		if (!SetConsoleWindowInfo(m_hConsoleHandle, TRUE, &m_windowRect)) // встановити позицію та розмів вікна
			Error(L"SetConsoleWindowInfo");

		if (!SetConsoleMode(m_hConsoleHandleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
			Error(L"SetConsoleMode");

		m_ScreenBuffer = new CHAR_INFO[nWidth * nHeight];
		std::memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * nWidth * nHeight);

		SetConsoleCtrlHandler((PHANDLER_ROUTINE)FALSE, TRUE);

		return true;
	}

	AdventoConsoleEngine::~AdventoConsoleEngine()
	{
		delete[] m_ScreenBuffer;
	}

	void AdventoConsoleEngine::Error(const wchar_t* sMessage)
	{
		wchar_t buf[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		MessageBoxW(nullptr, buf, sMessage, MB_OK);
	}

	void AdventoConsoleEngine::Clear(WCHAR c, engine::color clr)
	{
		Fill(0, 0, GetWindowSizeX(), GetWindowSizeY(), c, clr);
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

			for (int i = 0; i < 256; ++i)
			{
				short nState = GetAsyncKeyState(i);

				m_KeysData[i].bPressed = false;
				m_KeysData[i].bReleased = false;

				if (nState & 0x8000)
				{
					m_KeysData[i].bPressed = !m_KeysData[i].bHeld;
					m_KeysData[i].bHeld = true;
				}
				else
				{
					m_KeysData[i].bReleased = true;
					m_KeysData[i].bHeld = false;
				}
			}

			// події
			DWORD events = 0;
			GetNumberOfConsoleInputEvents(m_hConsoleHandleIn, &events);
			if (events > 0)
			{
				if (!ReadConsoleInput(m_hConsoleHandleIn, m_InputRecordBuf, 128, &m_dwNumRead))
					Error(L"ReadConsoleInput");
			}

			for (int i = 0; i < (int)m_dwNumRead; ++i)
			{
				switch (m_InputRecordBuf[i].EventType)
				{
					case MOUSE_EVENT:
						if (m_InputRecordBuf[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
						{
							m_vMousePosition = { 
								int(m_InputRecordBuf[i].Event.MouseEvent.dwMousePosition.X),
								int(m_InputRecordBuf[i].Event.MouseEvent.dwMousePosition.Y)
							};
						}	
						break;
				}
			}
			
			this->HandleInput(delta.count());
			this->Update(delta.count());
			this->Render();
			this->NativeRender();

			// update title
			std::wstring sTitle = m_sAppName + L" - FPS: " + std::to_wstring(int(1.0f / delta.count()));
			SetConsoleTitleW(sTitle.c_str());
		}
	}

	Vector_i2d AdventoConsoleEngine::GetMousePosition() const { return m_vMousePosition; }
	int AdventoConsoleEngine::GetMouseX() const { return m_vMousePosition.x; }
	int AdventoConsoleEngine::GetMouseY() const { return m_vMousePosition.y; }

	Vector_i2d AdventoConsoleEngine::GetWindowSize() const { return Vector_i2d{ int(m_nWidth), int(m_nHeight) }; }
	int AdventoConsoleEngine::GetWindowSizeX() const { return m_nWidth; }
	int AdventoConsoleEngine::GetWindowSizeY() const { return m_nHeight; }

	KeyData AdventoConsoleEngine::IsButton(const int button) const { return m_KeysData[button]; }
	bool AdventoConsoleEngine::IsWindowFocused() const { return GetForegroundWindow() == m_WindowName; }

	void AdventoConsoleEngine::DrawPoint(Vector_i2d vPosition, WCHAR c, engine::color renderColor)
	{
		if (vPosition.x >= 0 && vPosition.x < (int)m_nWidth &&
				vPosition.y >= 0 && vPosition.y < (int)m_nHeight)
		{
			m_ScreenBuffer[vPosition.y * m_nWidth + vPosition.x].Char.UnicodeChar = c;
			m_ScreenBuffer[vPosition.y * m_nWidth + vPosition.x].Attributes = renderColor;
		}
	}

	void AdventoConsoleEngine::DrawPoint(int x, int y, WCHAR c, engine::color renderColor)
	{
		this->DrawPoint({ x, y }, c, renderColor);
	}

	void AdventoConsoleEngine::Fill(int x1, int y1, int x2, int y2, WCHAR c, engine::color renderColor)
	{
		Wrap(x1, y1);
		Wrap(x2, y2);
		for (int x = x1; x < x2; ++x)
			for (int y = y1; y < y2; ++y)
				DrawPoint(x, y, c, renderColor);
	}

	void AdventoConsoleEngine::DrawString(Vector_i2d nPosition, const std::string &sData)
	{
		if (nPosition.x >= 0 && nPosition.x < int(m_nWidth - sData.length()) &&
				nPosition.y >= 0 && nPosition.y < m_nHeight)
		{
			for (int i = 0; i < (int)sData.length(); ++i)
			{
				m_ScreenBuffer[(nPosition.y * m_nWidth + nPosition.x) + i].Char.UnicodeChar = sData[i];
				m_ScreenBuffer[(nPosition.y * m_nWidth + nPosition.x) + i].Attributes = engine::default_colors::WHITE;
			}
		}
	}

	void AdventoConsoleEngine::DrawCircle(Vector_i2d viPosition, int nRadius, WCHAR c, engine::color renderColor)
	{
		if (viPosition.x >= 0 && viPosition.x < m_nWidth &&
				viPosition.y >= 0 && viPosition.y < m_nHeight)
		{
			int X = viPosition.x, Y = viPosition.y;
			double deg = 0;

			// draw a circle using sin( ) and cos( )
			do {
				X = (int) (nRadius * cos(deg));
				Y = (int) (nRadius * sin(deg));
				DrawPoint(X + viPosition.x, Y + viPosition.y, c, renderColor);
				deg +=  0.005;
			} while (deg <= 6.4);
		}
	}

	void AdventoConsoleEngine::FillCircle(Vector_i2d viPosition, int nRadius, WCHAR c, engine::color renderColor)
	{
		if (viPosition.x >= 0 && viPosition.x < m_nWidth &&
				viPosition.y >= 0 && viPosition.y < m_nHeight)
		{
			for (int i = 0; i < nRadius * 2; i++)
			{
				int dx = i - nRadius;
				int x = viPosition.x + dx;

				int h = (int)round((nRadius * 2) * sqrt((nRadius * 2) * (nRadius * 2) / 4.0 - dx * dx ) / (nRadius * 2));
				for (int dy = 1; dy <= h; dy++)
				{
					DrawPoint({ x, viPosition.y + dy }, c, renderColor);
					DrawPoint({ x, viPosition.y - dy }, c, renderColor);
				}

				if (h >= 0)
				{
					DrawPoint({ x, viPosition.y }, c, renderColor);
				}
			}
		}	
	}

	void AdventoConsoleEngine::Wrap(int &x, int &y)
	{
		if (x < 0) x = 0;
		else if (x > m_nWidth) x = m_nWidth;

		if (y < 0) y = 0;
		else if (y > m_nHeight) y = m_nHeight;

	}

	void AdventoConsoleEngine::Start()
	{
		m_bRunning = true;
		AppInit();
		std::thread gameLoop(&AdventoConsoleEngine::GameLoop, this);

		gameLoop.join();
	}
}
