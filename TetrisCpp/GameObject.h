#include "Buffer.h"

#define SHAPE_WIDTH 4
#define SHAPE_HEIGHT 3
#define POSITION 4

class GameObject
{
public:

	GameObject(wd* wData, int x, int y, int color, int speed, int type) {
		_x = x, _y = y, _color = color, _speed = speed, _type = type;
		_wData = wData;

		DrawObject();
	};

	int GetX();
	
	int GetY();

	void SetX(int x);

	void SetY(int y);

	virtual void DrawObject();	

protected:

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

	wd* _wData;

	int _x, _y, _color, _speed, _type, _pos = 0;

	void EraseObject();

	virtual ~GameObject() {
		delete this;
	};
};

class Shape : public GameObject 
{
private:

	bool _alreadyDown = false;

	void RotateShape();

	void FillCoord();

public:

	vector <pair<int, int>> shapesCoord;

	Shape(wd* wData, int x, int y, int color, int speed, int type) : GameObject(wData, x, y, color, speed, type) {
		DrawObject();
	};

	bool ShapeIsDown();

	void MoveShape();
};