#include "Buffer.h"

#define SHAPE_WIDTH 4
#define SHAPE_HEIGHT 3
#define POSITION 4

class GameObject
{
public:

	GameObject() {};

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

	void EraseObject();

	vector<char16_t> GetType(int type);

	vector <pair<int, int>> shapesCoord;

	virtual ~GameObject() {
		delete this;
	};

	bool deleteShape = false;

protected:

	char16_t shapeSprite[5][POSITION][SHAPE_HEIGHT][SHAPE_WIDTH] = {
		{
			{
			u"#  ",
			u"#  ",
			u"#  "
			},

			{
			u"   ",
			u"   ",
			u"###"
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
			u"   ",
			u"###",
			u"#  "
			},

			{
			u"## ",
			u" # ",
			u" # "
			},

			{
			u"   ",
			u"  #",
			u"###"
			}
		},
		{
			{
			u"   ",
			u"## ",
			u"## "
			},

			{
			u"   ",
			u"## ",
			u"## "
			},

			{
			u"   ",
			u"## ",
			u"## "
			},

			{
			u"   ",
			u"## ",
			u"## "
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
			u" # ",
			u"## ",
			u" # "
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
			u" # ",
			u"## ",
			u"#  "
			},
		},
	};

	wd* _wData;

	int _x, _y, _color, _speed, _type, _pos = 0;
	
};

class Shape : public GameObject 
{
private:

	int tick, speedY = 6;

	void RotateShape();

	void FillCoord();

public:

	bool collisionBot = false;

	Shape(wd* wData, int x, int y, int color, int speed, int type) : GameObject(wData, x, y, color, speed, type) {
		FillCoord();
		DrawObject();
	};

	int GetRight();

	int GetLeft();

	bool ShapeIsDown();

	void MoveShape(bool collisionLeft, bool collisionRight, int lvl, bool ready);

	void DeleteCoord();
};