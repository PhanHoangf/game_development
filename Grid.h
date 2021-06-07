#pragma once
#include <cstddef>


class Unit {
	friend class Grid;

public:
	Unit(Grid* grid, float x, float y);
	void Move(float x, float y);
private:
	float _x, _y;
	Grid* _grid;
	Unit* prev;
	Unit* next;
};

class Grid {
public:
	Grid() {
		for (int x = 0; x < NUM_CELLS; x++)
		{
			for (int y = 0; y < NUM_CELLS; y++)
			{
				_cells[x][y] = NULL;
			}
		}
	}
	static const int NUM_CELLS = 10;
	static const int CELL_SIZE = 20;
	void Add(Unit* unit);
	void Move(Unit* unit, float x, float y);
private:
	Unit* _cells[NUM_CELLS][NUM_CELLS];
};