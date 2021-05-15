#pragma once
#include <d3d9.h>
#include"Textures.h"
#include "Sprites.h"
#include "Utils.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

class CMap
{
	int TotalRowsOfMap, TotalColumnsOfMap;
	int TotalRowsOfTileSet, TotalColumnsOfTileSet;
	int TotalTiles;
	LPDIRECT3DTEXTURE9 MapTexture; //TileSet of map (worldx_bank.png)
	vector<LPSPRITE> Tiles; // Danh sach Sprites cua TileSet
	int** TileMap;
	float CamX, CamY;
public:
	CMap(int mapTextureID, int TotalRowsOfMap, int TotalColumnsOfMap, int TotalRowsOfTileSet, int TotalColumnsOfTileSet, int TotalTiles);
	~CMap();
	void ReadMapTXT(char* filePath);
	void ExtractTileFromTileSet();
	void DrawMap();
	void SetTileMapData(int** TileMapData);
	int GetMapWidth() { return TotalColumnsOfMap * TILE_WIDTH; }
	int GetMapHeight() { return TotalRowsOfMap * TILE_HEIGHT; }
	void SetCamPos(float x, float y);
};

