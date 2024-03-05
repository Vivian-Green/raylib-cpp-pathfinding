#include "grid.h"
#include "colors.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>


Grid::Grid() : numRows(100), numCols(100), cellSize(10), colors(GetCellColors()), successCount(0), failCount(0), shouldReset(false), reviewFrames(0) {
	Initialize();
}
std::vector<float> recentDistances;

enum CellState {
	EMPTY = 0,
	START = 6,
	END = 5,
	PATH = 2,
	OBSTACLE = 9,
	MAZE_OBSTACLE = 0,
	MAZE_PATH = 1,
	REVIEW_CELL = 3
};

void Grid::Initialize() {
	shouldReset = false;
	visitedCells = 0;
	maxVisitedCells = 5000;
	startCell = { 0, 0 };
	endCell = { 0, 0 };
	end = Cell();
	locations = {};
	closestCells = {};
	reviewFrames = 60;
	

	numRows = 100;
	numCols = 100;

	openSet = {};
	closedSet = std::vector<std::vector<bool>>(numRows, std::vector<bool>(numCols, false));

	std::cout << "Initializing grid..." << std::endl;
	for (int row = 0; row < numRows; row++) {
		for (int column = 0; column < numCols; column++) {
			grid[row][column] = CellState::EMPTY;
		}
	}

	InitMaze();

	SetStartCell();
	SetEndCell();

	std::cout << "Grid initialized." << std::endl;

	AddLocationsByDistance({ startCell });

	start = { static_cast<int>(startCell.x), static_cast<int>(startCell.y), 0, 0, nullptr };
	end = { static_cast<int>(endCell.x), static_cast<int>(endCell.y), 0, 0, nullptr };

	if (openSet.empty()) {
		openSet.push_back(start);
	}

	Frame();
}


void Grid::Frame() {
	if (!shouldReset) {
		PathFindStep(10);
		Draw();
	}
	else {
		ReviewCells();
		Draw();
	}
}

// Function to check if a cell is in the vector
bool Grid::ContainsCell(const std::vector<Cell>& cells, const Cell& target) {
	for (const auto& cell : cells) {
		if (cell.x == target.x && cell.y == target.y) {
			return true;
		}
	}
	return false;
}

void Grid::ReviewCells() {
	if (reviewFrames > 0) {
		reviewFrames--;
	} else {
		Initialize();
		return;
	}

	/*std::cout << "!";

	if (end.parent == NULL || IsCellOutside(end.y, end.x) || ContainsCell(closestCells, end)) {
		Initialize();
		return;
	}

	if (end.parent->x == start.x && end.parent->y == start.y) {
		end = Cell();
		Initialize();
		return;
	}

	closestCells.push_back(end);
	std::cout << end.x << "?" << end.y;
	grid[end.y][end.x] = CellState::PATH;
	std::cout << "review cell: " << end.x << ", " << end.y << std::endl;
	end = *end.parent;*/
}



void Grid::PathFindStep(int nextSteps) {
	visitedCells++;
	if (visitedCells > maxVisitedCells || openSet.size() == 0) {
		failCount++;
		shouldReset = true;
		closestCells = {};
		return;
	}

	// Find the element with the lowest cost in openSet
	auto minIt = std::min_element(openSet.begin(), openSet.end());
	Cell current = *minIt;

	//grid[current.y][current.x] = CellState::PATH;

	//std::cout << "current cell: " << current.x << ", " << current.y << std::endl;
	//std::cout << "end cell: " << end.x << ", " << end.y << std::endl;

	Vector2 thisPosition = { current.x, current.y };
	float thisDistance = GetDistanceSquaredToTarget(thisPosition);

	// Store the recent distances
	recentDistances.push_back(thisDistance);


	//std::cout << "distance:::::::::::::::::::::::: " << sqrt(thisDistance) << std::endl;

	//std::cout << "dist: " << thisDistance << std::endl;
	if (thisDistance < 0.5) {
		end = current;
		shouldReset = true;
		reviewFrames = 5;
		ReconstructPath();
		successCount++;
		return;
	}

	openSet.erase(minIt);
	closedSet[current.y][current.x] = true;
	Cell closest = current;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {

			if (i == 0 && j == 0) continue;

			// if (abs(i) + abs(j) == 2) continue; // comment this out to consider diagonals
				
			int neighborX = current.x + i;
			int neighborY = current.y + j;
			//std::cout << neighborX << ", " << neighborY << "; ";
			if (IsCellOutside(neighborY, neighborX)) continue;
			if (closedSet[neighborY][neighborX] == true) continue;

			float tentativeG = current.g + 1.0f; // Assuming a uniform cost for simplicity

			// Check if the neighbor is not in the open set or has a lower cost
			auto it = std::find_if(openSet.begin(), openSet.end(), [&](const Cell& c) {
				return c.x == neighborX && c.y == neighborY;
			});

			if (it != openSet.end() || tentativeG > it->g) continue;
			if (it != openSet.end()) openSet.erase(it);

			float h = GetDistanceSquaredToTarget(Vector2{ (float)neighborX, (float)neighborY });
			if (IsCellEmpty(neighborX, neighborY)) {
				if (grid[neighborY][neighborX] == 0) {
					grid[neighborY][neighborX] = CellState::REVIEW_CELL;
				}
				Cell neighbor = { neighborX, neighborY, tentativeG, h, &current };
				bool isCloser = (h * current.g < tentativeG * GetDistanceSquaredToTarget(Vector2{ (float)closest.x, (float)closest.x}));
				if (isCloser) closest = neighbor;

				openSet.push_back(neighbor);
				continue;
			}
			else {
				//Cell neighbor = { neighborX, neighborY, tentativeG, h * 1000, &current };
				//openSet.push_back(neighbor);
				closedSet[neighborY][neighborX] = true;
			}
		}
		//std::cout << std::endl;
	}

	closest.isClosestPath = true;
	closest.parent = &current;
	pathStack.push(closest);

	closestCells.push_back(closest);

	if (nextSteps > 0) {
		PathFindStep(nextSteps - 1);
	}
}

void Grid::ReconstructPath() {
	std::vector<Cell> path;

	while (!pathStack.empty()) {
		path.push_back(pathStack.top());
		pathStack.pop();
	}

	std::reverse(path.begin(), path.end());
	Cell lastCell = start;
	for (const Cell& cell : path) {
		if (!(lastCell.x == start.x && lastCell.y == start.y) && (abs(lastCell.x - cell.x) + abs(lastCell.y - cell.y) < 5)){
			grid[cell.y][cell.x] = CellState::PATH;
			lastCell = cell;
		}
		else if (lastCell.x == start.x && lastCell.y == start.y) {
			lastCell = cell;
		}
	}
}

void Grid::SetStartCell() {
	startCell.x = rand() % (numRows - 1);
	startCell.y = rand() % (numCols - 1);
	if (!IsCellEmpty(startCell.x, startCell.y)) {
		SetStartCell();
		return;
	}
	grid[(int)startCell.y][(int)startCell.x] = CellState::START;
}

void Grid::SetEndCell() {
	endCell.x = rand() % (numRows - 1);
	endCell.y = rand() % (numCols - 1);
	if (!IsCellEmpty(endCell.x, endCell.y)) {
		SetEndCell();
		return;
	}
	grid[(int)endCell.y][(int)endCell.x] = CellState::END;
}

void Grid::Draw() {
	for (int row = 0; row < numRows; row++) {
		for (int column = 0; column < numCols; column++) {
			int cellValue = grid[row][column];
			DrawRectangle(column * cellSize + 11, row * cellSize + 11, cellSize - 1, cellSize - 1, colors[cellValue]);
		}
	}
}

void Grid::InitMaze() {
	SetCellRect(0, 0, 99, 99, 9);
	SetCellRect(1, 1, 98, 98, 0);

	for (int i = 0; i < 30; i++) {
		FillRandomRect({ 20, 20 }, 9);
		FillRandomRect({ 20, 20 }, 0);
	}
}

void Grid::SetCellRect(int row1, int column1, int row2, int column2, int value) {
	// Ensure rows & columns are in order
	int startRow = std::min(row1, row2);
	int endRow = std::max(row1, row2);
	int startCol = std::min(column1, column2);
	int endCol = std::max(column1, column2);

	// Ensure rect is in bounds
	startRow = std::max(0, startRow);
	startCol = std::max(0, startCol);
	endRow = std::min(numRows - 1, endRow);
	endCol = std::min(numCols - 1, endCol);

	for (int row = startRow; row <= endRow; row++) {
		for (int col = startCol; col <= endCol; col++) {
			grid[row][col] = value;
		}
	}
}

void Grid::FillRandomRect(Vector2 maxSize, int value) {
	Vector2 corner1 = { rand() % (numCols - 1), rand() % (numRows - 1) };
	Vector2 randomSize = { rand() % (int)maxSize.x, rand() % (int)maxSize.y };
	Vector2 corner2 = { corner1.x + randomSize.x, corner1.y + randomSize.y };
	SetCellRect(corner1.x, corner1.y, corner2.x, corner2.y, value);
}

bool Grid::IsCellOutside(int row, int column) {
	return (row < 0 || row >= numRows || column < 0 || column >= numCols);
}

bool Grid::IsCellEmpty(int x, int y) {
	return grid[y][x] < 9;
}

void Grid::SetCell(int row, int column, int value)
{
	if (IsCellOutside(row, column)) return;
	grid[row][column] = value;
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

