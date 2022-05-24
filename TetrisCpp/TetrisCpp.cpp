#include "Game.h"

int main()
{
	bool restart = false;

	do
	{
		Game gm;
		gm.RunWorld(restart);
	} while (restart);
 

	return 0;
}