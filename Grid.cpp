#include "Grid.h"

Unit::Unit(Grid* grid, float x, float y) {
	this->_grid = grid;
	this->_x = x;
	this->_y = y;
	this->prev = NULL;
	this->next = NULL;

	this->_grid->Add(this);
}

void Unit::Move(float x, float y) {
	_grid->Move(this, x, y);
}

void Grid::Add(Unit* unit) {
	// Determine which grid cell it's in.
	int cellX = (int)(unit->_x / Grid::CELL_SIZE);
	int cellY = (int)(unit->_y / Grid::CELL_SIZE);

	// Add to the front of list for the cell it's in.
	unit->prev = NULL;
	unit->next = _cells[cellX][cellY];
	_cells[cellX][cellY] = unit;

	if (unit->next != NULL) {
		unit->next->prev = unit;
	}
}

void Grid::Move(Unit* unit, float x, float y) {
	int oldCellX = (int)(unit->_x / Grid::CELL_SIZE);
	int oldCellY = (int)(unit->_y / Grid::CELL_SIZE);

	// See which cell it's moving to.
	int cellX = (int)(x / Grid::CELL_SIZE);
	int cellY = (int)(y / Grid::CELL_SIZE);

	unit->_x = x;
	unit->_y = y;


	// If it didn't change cells, we're done.
	if (oldCellX == cellX && oldCellY == cellY) return;

	// Unlink it from the list of its old cell.
	if (unit->prev != NULL)
	{
		unit->prev->next = unit->next;
	}

	if (unit->next != NULL)
	{
		unit->next->prev = unit->prev;
	}

	// If it's the head of a list, remove it.
	if (_cells[oldCellX][oldCellY] == unit)
	{
		_cells[oldCellX][oldCellY] = unit->next;
	}

	// Add it back to the grid at its new cell.
	Add(unit);
}