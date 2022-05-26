#include "GameObject.h"

void Shape::MoveShape()
{
	if (_y == ROWS - 3) {
		_alreadyDown = true;
	}

	if (!_alreadyDown) {

		EraseObject();


		if (GetAsyncKeyState(VK_RIGHT)) {
			if (_x <= COLS - 6) {
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
		for (int j = 0; j < SHAPE_WIDTH; j++)
		{
			_wData->vBuf[_y + i][_x + j] = ' ';
		}
	}
}
