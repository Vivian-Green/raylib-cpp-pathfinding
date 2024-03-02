#pragma once
#include <vector>
#include <raylib.h>
#include <map>

class Grid {
public:
	int successCount;
	Grid();
	void Reset();
	int grid[100][100];
	void Initialize();
	void FillRandomRect(Vector2 maxSize, int value);
	void PathFind(int iterations);
	void InitMaze();
	void Print();
	void Draw();
	void Frame();
	bool IsCellOutside(int row, int column);
	bool IsCellEmpty(int row, int column);

	void SetCell(int row, int column, int value);
	void GetNearbyValidCells(int x, int y);
	void AddLocationsByDistance(std::vector<Vector2> positions);
	Vector2 locationIfValid(Vector2 thisPosition);
	void SetCellRect(int row1, int column1, int row2, int column2, int value);

private:
	bool shouldReset;
	int reviewCell = 0;
	int numRows;
	int numCols;
	int cellSize;
	std::vector<Vector2> locations;
	Vector2 startCell;
	Vector2 endCell;
	void SetStartCell();
	void SetEndCell();
	Vector2 GetClosestRecentCell(int length);
	float GetDistanceSquaredToTarget(Vector2 thisPosition);
	std::vector<Color> colors;
	std::vector<Vector2> closestCells;
};