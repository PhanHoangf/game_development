#pragma once
#include <cstddef>
#include "GameObject.h"
#include "Utils.h"


class Unit {
	friend class Grid;

public:
	Unit(Grid* grid, float x, float y);
	Unit(Grid* grid, LPGAMEOBJECT obj, int row, int col);
	void Move(float x, float y);
	LPGAMEOBJECT GetObject() { return _obj; }

private:
	float _x, _y;
	Grid* _grid;
	Unit* _prev;
	Unit* _next;
	LPGAMEOBJECT _obj;
};

class Grid {
public:
	Grid() {
		for (int x = 0; x < totalRows; x++)
		{
			for (int y = 0; y < totalCols; y++)
			{
				_cells[x][y] = NULL;
			}
		}
	}
	Grid(int rows, int cols, int mapWidth, int mapHeight){
		totalRows = rows;
		totalCols = cols;
		this->mapWidth = mapWidth;
		this->mapHeight = mapHeight;

		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < cols; y++)
			{
				_cells[x][y] = NULL;
			}
		}
	}
	//static const int NUM_CELLS = 10;
	static const int CELL_WIDTH = 136;
	static const int CELL_HEIGHT = 128;
	//static const int NUM_COLS = 21;
	//static const int NUM_ROWS = 4;

	void Add(Unit* unit);
	void Add(Unit* unit, int row, int col);

	void Move(Unit* unit, float x, float y);

	vector<Unit*> getObjectsInViewPort(float cam_x, float cam_y);
	void CountUnit();
	//vector<LPGAMEOBJECT> _allObject; //! all game Objects
private:
	Unit* _cells[50][50];
	vector<Unit*> _objects; //! Objects in viewport
	int totalRows, totalCols, mapWidth, mapHeight;
};