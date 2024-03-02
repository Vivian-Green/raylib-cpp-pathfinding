#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block {
public:
	Block();
	void Draw();
	int id;
	std::map<int, std::vector<Position>> cells;
	std::map<int, std::vector<Position>> BuildCells(std::vector<std::vector<std::vector<int>>> blockTiles);

	void Move(int rows, int columns);
	void Rotate();
	void UndoRotation();
	std::vector<Position> GetCellPositions();

private:
	int cellSize;
	int rotationState;
	std::vector<Color> colors;
	int rowOffset;
	int columnOffset;
};