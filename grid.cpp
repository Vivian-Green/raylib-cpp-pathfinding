#include "grid.h"
#include "colors.h"
#include <iostream>

Grid::Grid() {
	numRows = 100;
	numCols = 100;
	cellSize = 10;
	colors = GetCellColors();
	Initialize();
	successCount = 0;
	closestCells = {};
	shouldReset = false;
	reviewCell = 0;
}

void Grid::Initialize() {
	startCell = { 0, 0 };
	endCell = { 0, 0 };
	locations = {};
	closestCells = {};
	reviewCell = 0;
	shouldReset = false;

	numRows = 100;
	numCols = 100;

	std::cout << "initializing grid" << std::endl;
	for (int row = 0; row < numRows; row++) {
		//std::cout << "row " << row << std::endl;
		for (int column = 0; column < numCols; column++) {
			grid[row][column] = 0;
		}
	}

	InitMaze();

	SetStartCell();
	SetEndCell();	

	std::cout << "grid initialized" << std::endl;

	AddLocationsByDistance({ startCell });
	Frame();
}

void Grid::Frame() {
	if (!shouldReset) {
		PathFind(1);
		Draw();
	} else {
		if (reviewCell < closestCells.size()) {
			grid[(int)closestCells[reviewCell].y][(int)closestCells[reviewCell].x] = 1;
		} else {	
			if (reviewCell > 0) {
				Initialize();
				return;
			}
		}
		reviewCell++;
		Draw();
	}
}

void Grid::FillRandomRect(Vector2 maxSize, int value) {
	Vector2 corner1 = { rand() % (numCols - 1), rand() % (numRows - 1) };
	Vector2 randomSize = { rand() % (int)maxSize.x, rand() % (int)maxSize.y};
	Vector2 corner2 = { corner1.x + randomSize.x, corner1.y + randomSize.y };
	SetCellRect(corner1.x, corner1.y, corner2.x, corner2.y, value);
}


void Grid::PathFind(int iterations) {
	//std::cout << iterations << std::endl;
	if (iterations < 1) {
		return;
	}

	Vector2 closestNearbyCell = GetClosestRecentCell(8);
	closestCells.push_back(closestNearbyCell);

	
	if (closestNearbyCell.x != startCell.x || closestNearbyCell.y != startCell.y) {
		if (closestNearbyCell.x != endCell.x || closestNearbyCell.y != endCell.y) {
			grid[(int)closestNearbyCell.y][(int)closestNearbyCell.x] = 2;
		}
	}

	//std::cout << "getting nearby valid cells from pathfind" << std::endl;
	GetNearbyValidCells(closestNearbyCell.x, closestNearbyCell.y);
	PathFind(iterations-1);

}

void Grid::SetStartCell() {
	startCell.x = rand() % (numRows - 1);
	startCell.y = rand() % (numCols - 1);
	if (!IsCellEmpty(startCell.x, startCell.y)) {
		SetStartCell();
		return;
	}
	grid[(int) startCell.y][(int) startCell.x] = 6;
}
void Grid::SetEndCell() {
	endCell.x = rand() % (numRows - 1);
	endCell.y = rand() % (numCols - 1);
	if (!IsCellEmpty(endCell.x, endCell.y)) {
		SetEndCell();
		return;
	}
	grid[(int) endCell.y][(int) endCell.x] = 5;
}

Vector2 Grid::GetClosestRecentCell(int length) {
	//std::cout << "locations size " << locations.size() << std::endl;

	Vector2 closest = { -1, -1 };
	float closestDistance = 99999999;
	//std::cout << "size: " << locations.size() << std::endl;
	for (int i = locations.size() - 1; i > 0 && i > locations.size() - length - 1; i--) {
		Vector2 thisPosition = { locations[i].x , locations[i].y };
		float thisDistance = GetDistanceSquaredToTarget(thisPosition);

		//std::cout << "dist: " << thisDistance << std::endl;
		if (thisDistance < closestDistance) {
			if (grid[(int) thisPosition.y][(int) thisPosition.x] < 1) {
				closestDistance = thisDistance;
				closest = thisPosition;
			}

			if (thisDistance < 1.5) {
				shouldReset = true;
				reviewCell = 0;
				successCount++;
				return closest;
			}
		}
	}

	Vector2 bad = { -1, -1 };
	if (closest.x == bad.x && closest.y == bad.y) {

		std::cout << "bad!" << std::endl;
		if (closestCells.size() > 2) {
			return closestCells[(int)closestCells.size() / 2];
		}
		return locations[(int)locations.size()/4];
	}

	if (closestCells.size() > 10) {
		if (sqrt(GetDistanceSquaredToTarget(closestCells[closestCells.size() / 2])) - sqrt(GetDistanceSquaredToTarget(closest)) < closestCells.size() / 30) {
			int min = closestCells.size() - closestCells.size() / 2;
			if (min < 1) {
				min = 1;
			}
			for (int i = closestCells.size() - 1; i > min; i--) {
				grid[(int)closestCells[closestCells.size()-1].y][(int)closestCells[closestCells.size() - 1].x] = 3;
				closestCells.pop_back();
			}
			
			return closestCells[closestCells.size() - 1];
		}
	}

	return closest;
}

void Grid::InitMaze() {
	SetCellRect(0, 0, 99, 99, 9);
	SetCellRect(1, 1, 98, 98, 0);

	for (int i = 0; i < 30; i++) {
		FillRandomRect({ 20, 20 }, 9);
		FillRandomRect({ 20, 20 }, 0);
	}
	
}

void Grid::Print() {
	std::cout << "printing grid " << std::endl;
	for (int row = 0; row < numRows; row++) {
		//std::cout << "row " << row << std::endl;
		for (int column = 0; column < numCols; column++) {
			std::cout << grid[row][column] << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::Draw() {
	//std::cout << "drawing grid" << std::endl;
	for (int row = 0; row < numRows; row++) {
		//std::cout << "row " << row << std::endl;
		for (int column = 0; column < numCols; column++) {
			//std::cout << "column " << column << std::endl;
			//std::cout << grid[0] << std::endl;
			int cellValue = grid[row][column];
			DrawRectangle(column * cellSize+11, row * cellSize+11, cellSize-1, cellSize-1, colors[cellValue]);
		}
	}
}

bool Grid::IsCellOutside(int row, int column)
{
	if (row >= 0 && row < numRows && column >= 0 && column < numCols) {
		return false;
	}
	return true;
}

bool Grid::IsCellEmpty(int x, int y)
{
	if (grid[y][x] < 9) {
		return true;
	}
	return false;
}

void Grid::SetCell(int row, int column, int value)
{
	if (!IsCellOutside(row, column)) {
		grid[row][column] = value;
	}
}

void Grid::SetCellRect(int row1, int column1, int row2, int column2, int value)
{	
	// ensure rows & columns are in order
	if (row1 > row2) {
		SetCellRect(row2, column1, row1, column2, value);
		return;
	}
	if (column1 > column2) {
		SetCellRect(row1, column2, row2, column1, value);
		return;
	}

	// ensure rect is in bounds
	if (IsCellOutside(row1, column1) || IsCellOutside(row2, column2)) {
		// todo: clip here?
		std::cout 
			<< "SetCellRect rect OOB: (" << row1 << ", " << column1 << "), (" << row2 << ", " << column2 << ") " << std::endl 
			<< "    outside of bounds ([0, " << numRows - 1 << "], [0, " << numCols - 1 << "])";
		return;
	}

	for (int row = row1; row <= row2; row++) {
		for (int column = column1; column <= column2; column++) {
			grid[row][column] = value;
		}
	}
}

void Grid::GetNearbyValidCells(int x, int y) {
	Vector2 thisPosition = { (float)x - 1, (float)y - 1 };
	Vector2 pos1 = locationIfValid(thisPosition);
	thisPosition = { (float)x, (float)y - 1 };
	Vector2 pos2 = locationIfValid(thisPosition);
	thisPosition = { (float)x + 1, (float)y - 1 };
	Vector2 pos3 = locationIfValid(thisPosition);

	thisPosition = { (float)x - 1, (float)y};
	Vector2 pos4 = locationIfValid(thisPosition);

	thisPosition = { (float)x + 1, (float)y};
	Vector2 pos6 = locationIfValid(thisPosition);

	thisPosition = { (float)x - 1, (float)y + 1 };
	Vector2 pos7 = locationIfValid(thisPosition);
	thisPosition = { (float)x, (float)y + 1 };
	Vector2 pos8 = locationIfValid(thisPosition);
	thisPosition = { (float)x + 1, (float)y + 1 };
	Vector2 pos9 = locationIfValid(thisPosition);

	AddLocationsByDistance({ pos1, pos2, pos3, pos4, pos6, pos7, pos8, pos9 });
}

void Grid::AddLocationsByDistance(std::vector<Vector2> positions) {
	float furthestDistance = -1;
	int furthesti = 0;
	bool containsAny = false;
	//std::cout << "a" << std::endl;

	if (positions.size() > 1) {
		for (int i = 0; i < positions.size(); i++) {
			if (positions[i].x < 0 || positions[i].y < 0) {
				continue;
			}
			//std::cout << positions[i].x << ", " << positions[i].y << "| ";

			float thisDistance = GetDistanceSquaredToTarget(positions[i]);
			//std::cout << " !" << thisDistance;
			//std::cout << " " << furthestDistance << "! ";
			if (thisDistance > furthestDistance) {
				//std::cout << "$";
				furthestDistance = thisDistance;
				furthesti = i;
			}

			containsAny = true;
		}
	}

	if (positions[furthesti].x > -1 && positions[furthesti].y > -1) {
		locations.push_back(positions[furthesti]);
		//grid[(int) positions[furthesti].y][(int)positions[furthesti].x] = 4;
		//std::cout << "adding location: " << positions[furthesti].x << ", " << positions[furthesti].y << std::endl;
	}
	
	positions[furthesti] = {-1, -1};
	if (containsAny) {
		AddLocationsByDistance(positions);
	}
	else {
		//std::cout << "locations.size(): " << locations.size() << std::endl;
	}
}

Vector2 Grid::locationIfValid(Vector2 thisPosition) {
	//std::cout << "AAA: " << thisPosition.x << ", " << thisPosition.y << std::endl;
	if (grid[(int)thisPosition.y][(int)thisPosition.x] < 1 || grid[(int)thisPosition.y][(int)thisPosition.x] == 6) {
		return thisPosition;
	}
	Vector2 bad = { -1, -1 };
	return bad;
}

float Grid::GetDistanceSquaredToTarget(Vector2 thisPosition) {
	Vector2 relative = { thisPosition.x - endCell.x, thisPosition.y - endCell.y };
	return abs(relative.x * relative.x + relative.y * relative.y);
}

