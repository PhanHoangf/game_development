#include "Map.h"
#include "Game.h"

CMap::CMap(int TileSetID, int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int TotalColumnsOfTileSet, int TotalTiles) {
	CGame* game = CGame::GetInstance();
	MapTexture = CTextures::GetInstance()->Get(TileSetID);
	this->TotalRowsOfMap = TotalRowsOfMap;
	this->TotalColumnsOfMap = TotalColumnsOfMap;
	this->TotalRowsOfTileSet = TotalRowsOfTileSet;
	this->TotalColumnsOfTileSet = TotalColumnsOfTileSet;
	this->TotalTiles = TotalTiles;
	CamX = 0;
	CamY = 0;
	TileMap = NULL;
}

CMap::~CMap()
{
}



void CMap::DrawMap() {
	CGame* game = CGame::GetInstance();
	int firstColumn = floor(CamX / TILE_WIDTH);
	int lastColumn = ceil((CamX + (game->GetScreenWidth())) / TILE_WIDTH);
	for (int currentRow = 0; currentRow < TotalRowsOfMap; currentRow++) {
		for (int currentColumn = firstColumn; currentColumn < TotalColumnsOfMap; currentColumn++) {
			int spriteMapIndex = TileMap[currentRow][currentColumn] - 1;
			Tiles.at(spriteMapIndex)->Draw(currentColumn * TILE_WIDTH, currentRow * TILE_HEIGHT);
		}
	}
}

void CMap::SetTileMapData(int** TileMapData) {
	TileMap = TileMapData;
}

void CMap::ExtractTileFromTileSet() {
	for (int TileNum = 0; TileNum < TotalTiles; TileNum++) {
		int left = TileNum % TotalColumnsOfTileSet * TILE_WIDTH;
		int top = TileNum / TotalColumnsOfTileSet * TILE_HEIGHT;
		int right = left + TILE_WIDTH;
		int bottom = top + TILE_HEIGHT;
		DebugOut(L"[DETAILS]	left %d top %d right %d bottom %d\n", left, top, right, bottom);
		LPSPRITE NewTile = new CSprite(TileNum + 5000, left, top, right, bottom, MapTexture);
		Tiles.push_back(NewTile);
	}
}

//int CMap::GetMapWidth() {
//	return TotalColumnsOfMap * TILE_WIDTH;
//}
//
//int CMap::GetMapHeight() {
//	return TotalRowsOfMap * TILE_HEIGHT;
//}

void CMap::SetCamPos(float x, float y) {
	this->CamX = x;
	this->CamY = y;
}