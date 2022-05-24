#include "GameObject.h"

void Shape::MoveShape()
{
	if (GetAsyncKeyState(VK_RIGHT)) {
		if (_x <= COLS - 6) {
			EraseObject();
			_x += _speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		if (_x > 2) {
			EraseObject();
			_x -= _speed;
		}
	}
	
	EraseObject();
	_y += _speed;
	
}

void GameObject::DrawObject()
{
	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH; j++)
		{
			_wData->vBuf[_y + i][_x + j] = shapeSprite[_type - 1][i][j] | (_color << 8);
		}
	}
}

void GameObject::EraseObject()
{

	for (int i = 0; i < SHAPE_HEIGHT; i++)
	{
		for (int j = 0; j < SHAPE_WIDTH; j++)
		{
			_wData->vBuf[_y + i][_x + j] = ' ';
		}
	}
}
