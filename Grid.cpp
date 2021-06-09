#pragma
#include "Grid.h"
Unit::Unit(Grid* grid, float x, float y) {

	this->_grid = grid;
	this->_x = x;
	this->_y = y;
	this->_prev = NULL;
	this->_next = NULL;
	this->_obj = NULL;

	this->_grid->Add(this);
}

Unit::Unit(Grid* grid, LPGAMEOBJECT obj, int row, int col) {
	this->_grid = grid;
	this->_x = obj->x;
	this->_y = obj->y;
	this->_prev = NULL;
	this->_next = NULL;
	this->_obj = obj;
	this->_grid->Add(this, row, col);
}

void Unit::Move(float x, float y) {
	_grid->Move(this, x, y);
}

void Grid::Add(Unit* unit) {
	// Determine which grid cell it's in.
	//int cellX = (int)(unit->_x / Grid::CELL_HEIGHT);
	//int cellY = (int)(unit->_y / Grid::CELL_WIDTH);

	int row = (int)(unit->_y / CELL_HEIGHT);
	int col = (int)(unit->_x / CELL_WIDTH);

	unit->_prev = NULL;
	unit->_next = _cells[row][col];
	_cells[row][col] = unit;

	if (unit->_next != NULL) {
		unit->_next->_prev = unit;
	}
}

void Grid::Add(Unit* unit, int row, int col) {

	if (row == this->NUM_ROWS)
		row = this->NUM_ROWS - 1;
	if (col == this->NUM_COLS)
		col = this->NUM_COLS - 1;

	unit->_prev = NULL;
	unit->_next = _cells[row][col];
	_cells[row][col] = unit;

	if (unit->_next != NULL) {
		unit->_next->_prev = unit;
	}
}

void Grid::Move(Unit* unit, float x, float y) {
	int oldRow = (int)(unit->_y / CELL_HEIGHT);
	int oldCol = (int)(unit->_x / CELL_WIDTH);

	// See which cell it's moving to.
	int newRow = (int)(x / Grid::CELL_HEIGHT);
	int newCol = (int)(y / Grid::CELL_WIDTH);

	if (newRow > NUM_ROWS || newCol > NUM_COLS) return;

	unit->_x = x;
	unit->_y = y;


	// If it didn't change cells, we're done.
	if (oldRow == newRow && oldCol == newCol) return;

	// Unlink it from the list of its old cell.
	if (unit->_prev != NULL)
	{
		unit->_prev->_next = unit->_next;
	}

	if (unit->_next != NULL)
	{
		unit->_next->_prev = unit->_prev;
	}

	// If it's the head of a list, remove it.
	if (oldRow <= NUM_ROWS && oldCol <= NUM_COLS) {
		if (_cells[oldRow][oldCol] == unit)
		{
			_cells[oldRow][oldCol] = unit->_next;
		}
	}

	// Add it back to the grid at its new cell.
	Add(unit);
}

vector<Unit*> Grid::getObjectsInViewPort(float cam_x, float cam_y) {
	//int startCol = (int)(cam_x / CELL_WIDTH);
	//int endCol = (int)((cam_x + 272) / CELL_WIDTH);

	//int startRow = (int)(cam_y / CELL_HEIGHT);
	//int endRow = (int)((cam_y + 256) / CELL_HEIGHT);
	_objects.clear();

	int startCol = (int)((cam_x - 16 * 2) / CELL_WIDTH);
	int endCol = (int)ceil((cam_x + 272 + 16 * 2) / CELL_WIDTH);
	int ENDCOL = (int)ceil((2816) / CELL_WIDTH);
	if (endCol > ENDCOL)
		endCol = ENDCOL;
	if (startCol < 0)
		startCol = 0;
	//DebugOut(L"[GRID] %d %d\n", startCol, endCol);
	int startRow = (int)(cam_y / CELL_HEIGHT);
	int endRow = (int)ceil((cam_y + 256) / CELL_HEIGHT);
	int ENDROW = (int)ceil((432) / CELL_HEIGHT);
	if (endRow > ENDROW)
		endRow = ENDROW;

	for (int i = startRow; i <= endRow; i++) {
		for (int j = startCol; j <= endCol; j++) {
			Unit* unit = _cells[i][j];
			while (unit != NULL)
			{
				if (unit->GetObject()->isDestroyed == false) {
					_objects.push_back(unit);
					unit = unit->_next;
				}
				else
				{
					if (_cells[i][j] == unit)
						_cells[i][j] = unit->_next;
					if (unit->_next != NULL)
						unit->_next->_prev = unit->_prev;
					if (unit->_prev != NULL)
						unit->_prev->_next = unit->_next;
					Unit* tmp = unit;
					unit = unit->_next;
					delete tmp->GetObject();
					delete tmp;
				}
			}
		}
	}
	//DebugOut(L"[c]::%d\n", _objects.size());
	return _objects;
}

void Grid::CountUnit() {
	int c = 0;
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{

			Unit* unit = _cells[i][j];
			while (unit)
			{
				c++;
				unit = unit->_next;
			}
		}
	}
	DebugOut(L"\nNumber of Unit: %d\n", c);
}