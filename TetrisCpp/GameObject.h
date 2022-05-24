#include "Buffer.h"

#define SHAPE_WIDTH 6
#define SHAPE_HEIGHT 4

class GameObject
{
public:

	vector <char16_t> shapeSpritesList;

	int _x, _y, _color, _speed, _type;

	GameObject(wd* wData, int x, int y, int color, int speed, int type) {
		_x = x, _y = y, _color = color, _speed = speed, _type = type;
		_wData = wData;

		DrawObject();
	};

	virtual void DrawObject();

	void EraseObject();

	virtual ~GameObject() {
		delete this;
	};

protected:

	wd* _wData;
};

class Shape : public GameObject {
public:

	Shape(wd* wData, int x, int y, int color, int speed, int type) : GameObject(wData, x, y, color, speed, type) {
		shapeSpritesList.push_back();
	
	};

	void MoveShape();
};