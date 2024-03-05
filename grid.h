#pragma once
#include <vector>
#include <raylib.h>
#include <map>
#include <stack>

struct Cell {
	int x, y;
	float g, h;
	Cell* parent;
	bool isClosestPath;
	bool operator<(const Cell& other) const {
		return (g + h) < (other.g + other.h);
	}
};

class Grid {
public:
	std::vector<Cell> openSet;
	std::vector<std::vector<bool>> closedSet;
	Cell start;
	Cell end;
	int successCount;
	int failCount;
	Grid();
	void Reset();
	int grid[100][100];
	int numRows;
	int numCols;
	int visitedCells;
	int maxVisitedCells;
	Vector2 startCell;
	Vector2 endCell;
	std::stack<Cell> pathStack;
	void Initialize();
	void FillRandomRect(Vector2 maxSize, int value);
	void InitMaze();
	void Print();
	void Draw();
	void Frame();
	bool ContainsCell(const std::vector<Cell>& cells, const Cell& target);
	void ReviewCells();
	void SplitPathAt(int splitPointIndex);
	void PathFindStep(int nextSteps);
	void ReconstructPath();
	void ReconstructPath(const Cell& endCell);
	bool IsCellOutside(int row, int column);
	bool IsCellEmpty(int row, int column);

	void SetCell(int row, int column, int value);
	void GetNearbyValidCells(int x, int y);
	void AddLocationsByDistance(std::vector<Vector2> positions);
	Vector2 locationIfValid(Vector2 thisPosition);
	void SetCellRect(int row1, int column1, int row2, int column2, int value);
	std::vector<std::vector<Vector2>> parentMap;

private:
	bool shouldReset;
	int reviewFrames = 0;
	int cellSize;
	std::vector<Vector2> locations;
	void SetStartCell();
	void SetEndCell();
	Vector2 GetClosestRecentCell(int length);
	float GetDistanceSquaredToTarget(Vector2 thisPosition);
	std::vector<Color> colors;
	std::vector<Cell> closestCells;
};