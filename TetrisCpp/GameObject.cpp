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

void GameObject::DrawObject() {
	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH - 1; j++)
		{
			if (shapeSprite[_type - 1][_pos][i][j] != u' ') {
				_wData->vBuf[_y + i][_x + j] = shapeSprite[_type - 1][_pos][i][j] | (_color << 8);
			}
		}
	}
}

void GameObject::EraseObject()
 {
	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH - 1; j++)
		{
			if (shapeSprite[_type - 1][_pos][i][j] != u' ') {
				_wData->vBuf[_y + i][_x + j] = ' ';
			}
		}
	}
}

bool Shape::ShapeIsDown()
{
	return _alreadyDown;
}

void Shape::MoveShape()
{
	FillCoord();

	if (shapesCoord.back().second == ROWS - 1) {
		_alreadyDown = true;
	}

	if (!_alreadyDown) {

		EraseObject();


		if (GetAsyncKeyState(VK_RIGHT)) {
			if (_x <= COLS - 5) {
				_x += _speed;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			if (_x > 2) {
				_x -= _speed;
			}
		}

		if (GetAsyncKeyState(VK_SPACE)) {
			RotateShape();
		}

		_y += _speed;
	}
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
