#pragma once
#include "grid.h"
#include <vector>
#include "block.h"

class Game {
public:
	Game();
	bool gameOver;

	int score;
	void Draw();
	Grid grid;

private:

};