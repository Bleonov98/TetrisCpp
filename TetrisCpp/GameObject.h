#include "Buffer.h"

#define SHAPE_WIDTH 4
#define SHAPE_HEIGHT 3
#define POSITION 4

class GameObject
{
public:

	char16_t shapeSprite[5][POSITION][SHAPE_HEIGHT][SHAPE_WIDTH] = { 
		{
			{
			u"  #",
			u"  #",
			u"  #"
			},

			{
			u"###",
			u"   ",
			u"   "
			}, 

			{
			u"#  ",
			u"#  ",
			u"#  "
			},

			{
			u"   ",
			u"   ",
			u"###"
			}
		},
		{
			{
			u"#  ",
			u"#  ",
			u"## "
			},

			{
			u"###",
			u"#  ",
			u"   "
			},

			{
			u" ##",
			u"  #",
			u"  #"
			},

			{
			u"   ",
			u"  #",
			u"###"
			}
		},
		{
			{
			u"###",
			u"###",
			u"###"
			},

			{
			u"###",
			u"###",
			u"###"
			},

			{
			u"###",
			u"###",
			u"###"
			},

			{
			u"###",
			u"###",
			u"###"
			},
		},
		{
			{
			u"   ",
			u" # ",
			u"###"
			},

			{
			u"#  ",
			u"## ",
			u"#  "
			},

			{
			u"###",
			u" # ",
			u"   "
			},

			{
			u"  #",
			u" ##",
			u"  #"
			},
		},
		{
			{
			u"   ",
			u" ##",
			u"## "
			},

			{
			u"#  ",
			u"## ",
			u" # "
			},

			{
			u"   ",
			u"## ",
			u" ##"
			},

			{
			u"  #",
			u" ##",
			u" # "
			},
		},
	};

	int _x, _y, _color, _speed, _type, _pos = 0;

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

class Shape : public GameObject 
{
public:

	bool _alreadyDown = false;

	Shape(wd* wData, int x, int y, int color, int speed, int type) : GameObject(wData, x, y, color, speed, type) {
	};

	void MoveShape();

	void RotateShape();
};