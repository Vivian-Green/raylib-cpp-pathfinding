#include "block.h"
#include <iostream>

Block::Block(){
	cellSize = 30;
	rotationState = 0;
	colors = GetCellColors();
    rowOffset = 0;
    columnOffset = 0;
}

void Block::Draw() {
    std::vector<Position> tiles = GetCellPositions();
	for (Position item : tiles) {
		DrawRectangle(item.column * cellSize + 11, item.row * cellSize + 11, cellSize - 1, cellSize - 1, colors[id]);
	}
}

std::map<int, std::vector<Position>> Block::BuildCells(std::vector<std::vector<std::vector<int>>> blockTiles) {
    std::map<int, std::vector<Position>> cells;
    int blockSize = blockTiles[0].size();

    for (int rotation = 0; rotation < 4; ++rotation) {
        cells[rotation] = std::vector<Position>(); // Initialize the vector for the current rotation

        int i = 0;
        for (int row = 0; row < blockSize; ++row) {
            //std::cout << "row: " << row << std::endl;
            for (int column = 0; column < blockSize; ++column) {
                //std::cout << "column: " << column << std::endl;
                if (blockTiles[rotation][row][column] == 1) {
                    //std::cout << "found! : " << row << ", " << column << std::endl;
                    cells[rotation].push_back(Position(row, column));
                }
            }
            i++;
        }
    }
    int rotations = cells.size();
    for (int i = 0; i < rotations; i++) {
        std::vector<Position> rotation = cells[i];
        for (int j = 0; j < rotation.size(); j++) {
            std::cout << rotation[j].row << ", " << rotation[j].column << std::endl;
        }
        std::cout << std::endl;
    }
    return cells;
}

void Block::Move(int rows, int columns) {
    rowOffset += rows;
    columnOffset += columns;
}

void Block::Rotate()
{
    rotationState++;
    if (rotationState == (int) cells.size()) {
        rotationState = 0;
    }
}

void Block::UndoRotation()
{
    rotationState--;
    if (rotationState == -1) {
        rotationState = -1 + (int) cells.size();
    }
}

std::vector<Position> Block::GetCellPositions() {
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;

    for (Position item : tiles) {
        Position newPos = Position(item.row + rowOffset, item.column + columnOffset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}