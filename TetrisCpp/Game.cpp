#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys(bool &pause)
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		else if (GetAsyncKeyState(0x50) & 0x8000) {
			pause = !pause;

			this_thread::sleep_for(milliseconds(1000));
		}
	}
}

void Game::ShapePreview()
{
	GameObject* gmObj = new GameObject;

	nextShape = 1 + rand() % 5;

	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH - 1; j++)
		{
			SetPos(COLS + 23 + j, 2 + i);
			cout << ' ';
		}
	}

	vector<char16_t> sprite = gmObj->GetType(nextShape - 1);

	int n = 0;

	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH - 1; j++)
		{
			SetPos(COLS + 23 + j, 2 + i);

			if (sprite[n] == u'#') printf(CSI "47;37m");
			cout << char(sprite[n]);
			printf(CSI "0m");
			n++;
		}
	}
}

void Game::GoNewShape(Shape* shape)
{
	shape = new Shape(&wData, 10 + rand() % (COLS - 20), 2, 1 + rand() % 7, 1, nextShape);

	shapeList.push_back(shape);
	allGameObjects.push_back(shape);

	ch = true;

	ShapePreview();

	ready = false;
}

bool Game::GameOver()
{
	while (worldIsRun) {
		if ((score % 260 == 0) && (score > 0)) {
			if (level <= 4) {
				level++;
				score += 20;
			}
		}
		if (level == 5) worldIsRun = false;

		for (int shape = 0; shape < shapeList.size(); shape++)
		{
			if ((shapeList[shape]->collisionBot) && (shapeList[shape]->GetY() <= 5)) {
				worldIsRun = false;
			}
		}
		
		this_thread::sleep_for(milliseconds(1));
	}

	return worldIsRun;
}

void Game::ShapeIsReady()
{
	checkCollision = false;

	for (int i = 0; i < 1; i++)
	{
		this_thread::sleep_for(milliseconds(250));
	}

	shapeList.back()->collisionBot ? ready = true : ready = false;

	checkCollision = true;
}

void Game::MergeLine(vector <int>& lineErase)
{
	bool restart = true;

	for (int shape = 0; shape < shapeList.size();)
	{
		if (restart) shape = 0;
		else shape++;

		if (shape == shapeList.size()) break;

		if (shapeList[shape]->deleteShape) {
			shapeList.erase(shapeList.begin() + shape);
			allGameObjects.erase(allGameObjects.begin() + shape);

			restart = true;
		}
		else restart = false;
	}

	int line;

	for (int i = 0; i < lineErase.size(); i++)
	{
		auto minEl = min_element(lineErase.begin(), lineErase.end());
		line = *minEl;

		for (int shape = 0; shape < shapeList.size() - 1; shape++)
		{
			for (int coord = 0; coord < shapeList[shape]->shapesCoord.size(); coord++)
			{
				if (shapeList[shape]->shapesCoord[coord].second < line) {
					wData.vBuf[shapeList[shape]->shapesCoord[coord].second][shapeList[shape]->shapesCoord[coord].first] = u' ';
					shapeList[shape]->shapesCoord[coord].second++;
				}
			}
		}
		if (lineErase.size() > 1) {
			lineErase.erase(minEl);
			i = -1;
		}
		
		score += 20;
	}
}

void Game::DrawEndInfo(bool &restart)
{
	if (level == 5) {
		SetPos(31, 17);
		cout << "CONGRATULATIONS! YOU WIN";
	}
	else {
		SetPos(31, 17);
		cout << "GAME OVER!";
		SetPos(31, 20);
		cout << "LEVEL: " << level + 1 << "/5";
	}
	SetPos(32, 19);
	cout << "SCORE: " << score;
	SetPos(25, 22);
	cout << "PRESS ENTER TO RESTART";
	SetPos(27, 23);
	cout << "PRESS ESC TO EXIT";

	pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	}
}

void Game::DrawInfo()
{
	SetPos(COLS + 10, 2);
	cout << score;
	SetPos(COLS + 10, 4);
	cout << level + 1;
}

void Game::ClearLine()
{
	vector<int> lineCounter(ROWS);

	vector<int> lineErase;
	lineErase.reserve(ROWS);

	int lineCnt = 0;

	for (int i = 0; i < ROWS; i++)
	{
		lineCounter[i] = 0;
	}

	for (int y = 2; y < ROWS; y++)
	{
		for (int x = 2; x <= COLS - 2; x++)
		{
			if ((wData.vBuf[y][x] != u' ') && (wData.vBuf[y][x] != 0)) {
				lineCounter[y]++;
			}
		}
	}

	for (int i = 0; i < lineCounter.size(); i++)
	{
		if (lineCounter[i] >= COLS - 3) {
			lineCnt++;
			for (int x = 2; x <= COLS - 2; x++)
			{
				wData.vBuf[i][x] = u' ';
			}
			for (int shape = 0; shape < shapeList.size(); shape++)
			{
				shapeList[shape]->DeleteCoord();
			}
			lineErase.push_back(i);
		}
	}

	if (lineCnt > 0) {
		MergeLine(lineErase);
	}
}

void Game::CollisionSide(bool& collisionRight, bool& collisionLeft)
{
	for (int i = 0; i < shapeList.size() - 1; i++)
	{
		bool finded = false;

		for (int j = 0; j < shapeList[i]->shapesCoord.size(); j++) {


			for (int size = 0; size < shapeList.back()->shapesCoord.size(); size++)
			{
				if (!collisionRight) {
					if (
						(((shapeList.back()->shapesCoord[size].first == shapeList[i]->shapesCoord[j].first - 1) &&
							(shapeList.back()->shapesCoord[size].second + 1 == shapeList[i]->shapesCoord[j].second)) ||
							((shapeList.back()->shapesCoord[size].first == shapeList[i]->shapesCoord[j].first - 1) &&
								(shapeList.back()->shapesCoord[size].second == shapeList[i]->shapesCoord[j].second)))
						)
					{
						collisionRight = true;
					}
				}
				
				if (!collisionLeft) {
					if (((shapeList.back()->shapesCoord[size].first == shapeList[i]->shapesCoord[j].first + 1) &&
						(shapeList.back()->shapesCoord[size].second + 1 == shapeList[i]->shapesCoord[j].second)) ||
						((shapeList.back()->shapesCoord[size].first == shapeList[i]->shapesCoord[j].first + 1) &&
							(shapeList.back()->shapesCoord[size].second == shapeList[i]->shapesCoord[j].second)))
					{
						collisionLeft = true;
					}
				}
			}
		}
	}

	// Side collision with objects
}

void Game::CollisionBot()
{

	for (int i = 0; i < shapeList.size() - 1; i++)
	{

		for (int j = 0; j < shapeList[i]->shapesCoord.size(); j++)
		{

			for (int size = 0; size < shapeList.back()->shapesCoord.size(); size++)
			{
				if ((shapeList[i]->shapesCoord[j].first == shapeList.back()->shapesCoord[size].first) &&
					(shapeList[i]->shapesCoord[j].second - 1 == shapeList.back()->shapesCoord[size].second)) {

					shapeList.back()->collisionBot = true;
					

					if (checkCollision) {
						thread readyShape([&]
							{ ShapeIsReady(); }
						);
						readyShape.detach();
					}

					clearLine = true;
				}
			}
		}
	}
}

void Game::DrawToMem()
{
	for (int i = 0; i < allGameObjects.size(); i++)
	{
		if (allGameObjects[i]->deleteShape) {
			allGameObjects.erase(allGameObjects.begin() + i);
		}
	}

	for (int i = 0; i < allGameObjects.size(); i++)
	{
		allGameObjects[i]->DrawObject();
	}
}

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
					for (int i = 0; i < strnlen(area, 2192); i++) {
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

	bool pause = false;

	thread hotKeys([&]
		{ HotKeys(pause); }
	);

	thread gameOver([&]
		{ GameOver(); }
	);

	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

	Shape* shape = new Shape(&wData, 10 + rand() % (COLS - 20), 2, 1 + rand() % 7, 1, 1 + rand() % 5);
	shapeList.push_back(shape);
	allGameObjects.push_back(shape);
	ShapePreview();

	level = 0;
	score = 0;

	bool collisionLeft = false;
	bool collisionRight = false;

	while (worldIsRun) {

		if (pause) {

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "PAUSED";

			while (pause) {

			}

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "      ";

		}

		shapeList.back()->MoveShape(collisionLeft, collisionRight, level, ready);
		// -> MoveShape function

		if (shapeList.back()->ShapeIsDown()) {

			if (checkCollision) {
				thread readyShape([&]
					{ ShapeIsReady(); }
				);
				readyShape.detach();
			}

			shapeList.back()->collisionBot = true;

			clearLine = true;
		}

		collisionRight = false;
		collisionLeft = false;

		CollisionSide(collisionRight, collisionLeft);

		CollisionBot();

		if (ready) GoNewShape(shape);

		DrawToMem();

		if (clearLine && ch) {
			ClearLine();

			clearLine = false;
			ch = false;
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

					if ((prevBuf[y][x] >> 8) == Red) {
						printf(CSI "41;31m");
					}
					else if ((prevBuf[y][x] >> 8) == Green) {
						printf(CSI "42;32m");
					}
					else if ((prevBuf[y][x] >> 8) == Blue) {
						printf(CSI "44;34m");
					}
					else if ((prevBuf[y][x] >> 8) == Yellow) {
						printf(CSI "43;33m");
					}
					else if ((prevBuf[y][x] >> 8) == Purple) {
						printf(CSI "45;35m");
					}
					else if ((prevBuf[y][x] >> 8) == Cyan) {
						printf(CSI "46;36m");
					}
					else if ((prevBuf[y][x] >> 8) == White) {
						printf(CSI "47;37m");
					}
					else printf(CSI "0;0m");

					cout << char(prevBuf[y][x]);

					printf(CSI "1;0m");
				}
			}
		}

		DrawInfo();

		Sleep(40);
	}

	DrawEndInfo(restart);

	hotKeys.join();
	gameOver.join();

	printf(CSI "?1049l");
}

