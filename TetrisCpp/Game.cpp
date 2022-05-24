#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys()
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		if (GetKeyState(0x50)) {
			pause = !pause;
		}
		this_thread::sleep_for(milliseconds(10));
	}
}

void Game::DrawInfo()
{
	SetPos(17, 52);
	cout << score;
	SetPos(17, 53);
	cout << level + 1;
}

//void Game::DrawTitle() {
//
//	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
//
//	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT2), L"TEXT");
//
//	if (hResource)
//	{
//		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);
//
//		if (hLoadedResource)
//		{
//			LPCSTR title = (LPCSTR)LockResource(hLoadedResource);
//
//			if (title)
//			{
//				DWORD dwResourceSize = SizeofResource(hInstance, hResource);
//
//				if (0 != dwResourceSize)
//				{
//					int j = 0;
//					int k = 1;
//					for (int i = 0; i < 55; i++)
//					{
//						for (; j < 150.95 * k; j++)
//						{
//							cout << title[j];
//						}
//						k++;
//						Sleep(20);
//					}
//				}
//			}
//		}
//	}
//
//	while (!GetAsyncKeyState(VK_RETURN)) {
//		SetPos(67, 42);
//		cout << "PRESS ENTER TO START";
//		Sleep(650);
//		SetPos(67, 42);
//		cout << "                    ";
//		Sleep(650);
//	}
//
//	PlaySound(NULL, 0, 0);
//
//	system("cls");
//}

void Game::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					for (int i = 0; i < strnlen(area, 7500); i++) {
						cout << area[i];
					}
				}
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	// DrawTitle();
	DrawArea();
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	thread hotKeys([&]
		{ HotKeys(); }
	);

	Shape* shape = new Shape(&wData, COLS / 2, 3, FOREGROUND_RED, 1, 1);
	shapeList.push_back(shape);
	allGameObjects.push_back(shape);

	level = 0;
	score = 0;

	while (worldIsRun) {
		if (pause) {
			SetPos(COLS / 2, ROWS / 2);
			cout << "PAUSED";
			while (pause) {

			}
			SetPos(COLS / 2, ROWS / 2);
			cout << "      ";
		}

		if (shapeList.back()->_y >= ROWS - 4) {
			shape = new Shape(&wData, COLS/2, 3, FOREGROUND_RED, 1, 1);
			shapeList.push_back(shape);
			allGameObjects.push_back(shape);
		}
	
		for (int i = 0; i < shapeList.size() - 1; i++)
		{
			for (int height = 0; height < SHAPE_HEIGHT; height++)
			{
				for (int width = 0; width < SHAPE_WIDTH; width++)
				{
					if ((shapeList.back()->_x + width == shapeList[i]->_x + width) && (shapeList.back()->_y == shapeList[i]->_y)) {
						shape = new Shape(&wData, COLS / 2, 3, FOREGROUND_RED, 1, 1);
						shapeList.push_back(shape);
						allGameObjects.push_back(shape);
					}
				}
			}
		}

		shapeList.back()->MoveShape();

		for (int i = 0; i < allGameObjects.size(); i++)
		{
			allGameObjects[i]->DrawObject();
		}

		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				if (prevBuf[y][x] == wData.vBuf[y][x]) {
					continue;
				}
				else {
					prevBuf[y][x] = wData.vBuf[y][x];

					SetPos(x, y);

					if ((prevBuf[y][x] >> 8) == FOREGROUND_RED) {
						printf(CSI "22;31m");
					}
					else if ((prevBuf[y][x] >> 8) == FOREGROUND_GREEN) {
						printf(CSI "22;32m");
					}
					else if ((prevBuf[y][x] >> 8) == FOREGROUND_BLUE) {
						printf(CSI "22;34m");
					}
					else if ((prevBuf[y][x] >> 8) == (FOREGROUND_RED | FOREGROUND_GREEN)) {
						printf(CSI "22;33m");
					}
					else if ((prevBuf[y][x] >> 8) == (FOREGROUND_RED | FOREGROUND_BLUE)) {
						printf(CSI "22;35m");
					}
					else if ((prevBuf[y][x] >> 8) == (FOREGROUND_GREEN | FOREGROUND_BLUE)) {
						printf(CSI "22;36m");
					}
					else {
						printf(CSI "22;37m");
					}

					cout << char(prevBuf[y][x]);

					printf(CSI "1;0m");
				}
			}
		}

		DrawInfo();

		Sleep(60);

		if (score == 1000) {
			worldIsRun = 0;
		}

	}



}
