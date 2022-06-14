#include "GameObject.h"

int GameObject::GetX() {
	return _x;
}

int GameObject::GetY() {
	return _y;
}

void GameObject::SetX(int x) {
	_x = x;
}

void GameObject::SetY(int y) {
	_y = y;
}

void GameObject::DrawObject() 
{
	for (int i = 0; i < shapesCoord.size(); i++)
	{
		_wData->vBuf[shapesCoord[i].second][shapesCoord[i].first] = u'#' | (_color << 8);
	}
}

void GameObject::EraseObject()
{
	for (int i = 0; i < shapesCoord.size(); i++)
	{
		_wData->vBuf[shapesCoord[i].second][shapesCoord[i].first] = u' ';
	}
}

vector<char16_t> GameObject::GetType(int type)
{
	vector<char16_t> sprite;

	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH - 1; j++)
		{
			sprite.push_back(shapeSprite[type][0][i][j]);
		}
	}
	
	return sprite;
}

bool Shape::ShapeIsDown()
{
	return collisionBot;
}

void Shape::MoveShape(bool collisionLeft, bool collisionRight, int lvl, bool ready)
{
	if (shapesCoord.back().second == ROWS - 1) {
		collisionBot = true;
	}

	EraseObject();

	if (!collisionBot) {
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			speedY = 1;
		}
		else {
			speedY = 6;
			if (lvl >= 3) {
				speedY = 3;
			}
		}

		if (tick % speedY == 0) {
			_y += _speed;
		}

		if (tick % 3 == 0) {
			if (GetAsyncKeyState(VK_SPACE)) {
				if (GetLeft() >= (SHAPE_WIDTH - 2) && GetRight() <= COLS - (SHAPE_WIDTH - 2)) RotateShape();

				if (GetRight() >= COLS - 2) {
					do
					{
						_x--;
					} while (_x >= COLS - (SHAPE_WIDTH - 1));
				};
			}
		}
	}

	if (!ready) {

		if (GetAsyncKeyState(VK_RIGHT)) {
			if ((GetRight() <= COLS - 3) && (!collisionRight)) {
				_x += _speed;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			if ((GetLeft() >= 3) && (!collisionLeft)) {
				_x -= _speed;
			}
		}
	}
	
	FillCoord();

	tick++;
}

void Shape::RotateShape()
{
	if (_pos <= 2) {
		_pos++;
	}
	else if (_pos >= 3) {
		_pos = 0;
	}
}

int Shape::GetRight()
{
	int rightX = _x;

	for (int i = 0; i < shapesCoord.size(); i++)
	{
		if (shapesCoord[i].first > rightX) {
			rightX = shapesCoord[i].first;
		}
	}

	return rightX;
}

int Shape::GetLeft()
{
	int LeftX = shapesCoord.front().first;

	for (int i = 0; i < shapesCoord.size(); i++)
	{
		if (shapesCoord[i].first < LeftX) {
			LeftX = shapesCoord[i].first;
		}
	}

	return LeftX;
}

void Shape::FillCoord()
{
	shapesCoord.clear();

	for (int height = 0; height < SHAPE_HEIGHT; height++)
	{
		for (int width = 0; width < SHAPE_WIDTH; width++)
		{
			if (shapeSprite[_type - 1][_pos][height][width] == u'#') {
				shapesCoord.push_back(make_pair(_x + width, _y + height));
			}
		}
	}
}

void Shape::DeleteCoord()
{
	bool restart = true;

	for (int i = 0; i < shapesCoord.size();)
	{
		if (restart) i = 0;
		else i++;

		if (i == shapesCoord.size()) break;
		
		if (_wData->vBuf[shapesCoord[i].second][shapesCoord[i].first] == u' ') {
			shapesCoord.erase(shapesCoord.begin() + i);

			restart = true;
		}
		else restart = false;
	}

	if (shapesCoord.size() == 0)
	{
		deleteShape = true;
	}
}
