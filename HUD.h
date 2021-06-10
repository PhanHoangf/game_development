#pragma once
#include "GameObject.h"
#include "Mario.h"
#include <map>

#define FONT_BBOX_WIDTH						8
#define DEFAULT_TIME						300

#define SPRITE_HUD_ID						99999
#define SPRITE_ICONMARIO_ID					50003
#define SPRITE_ICONLUIGI_ID					50004
#define SPRITE_FILLARROW_ID					50005
#define ANI_P_ID							5006

#define PLAYSCENE_HUD	0
#define WORLDSCENE_HUD	1

#define CARD_ANI_SET_ID	58

#define SPRITE_FONT_0_ID				50013
#define SPRITE_FONT_1_ID				50014
#define SPRITE_FONT_2_ID				50015
#define SPRITE_FONT_3_ID				50016
#define SPRITE_FONT_4_ID				50017
#define SPRITE_FONT_5_ID				50018
#define SPRITE_FONT_6_ID				50019
#define SPRITE_FONT_7_ID				50020
#define SPRITE_FONT_8_ID				50021
#define SPRITE_FONT_9_ID				50022
#define SPRITE_FONT_A_ID				50023
#define SPRITE_FONT_B_ID				50024
#define SPRITE_FONT_C_ID				50025
#define SPRITE_FONT_D_ID				50026
#define SPRITE_FONT_E_ID				50027
#define SPRITE_FONT_F_ID				50028
#define SPRITE_FONT_G_ID				50029
#define SPRITE_FONT_H_ID				50030
#define SPRITE_FONT_I_ID				50031
#define SPRITE_FONT_J_ID				50032
#define SPRITE_FONT_K_ID				50033
#define SPRITE_FONT_L_ID				50034
#define SPRITE_FONT_M_ID				50035
#define SPRITE_FONT_N_ID				50036
#define SPRITE_FONT_O_ID				50037
#define SPRITE_FONT_P_ID				50038
#define SPRITE_FONT_Q_ID				50039
#define SPRITE_FONT_R_ID				50040
#define SPRITE_FONT_S_ID				50041
#define SPRITE_FONT_T_ID				50042
#define SPRITE_FONT_U_ID				50043
#define SPRITE_FONT_V_ID				50044
#define SPRITE_FONT_W_ID				50045
#define SPRITE_FONT_X_ID				50046
#define SPRITE_FONT_Y_ID				50047
#define SPRITE_FONT_Z_ID				50048

#define CARD_ANI_ID	57

class HUD :public CGameObject {
	CMario* mario;

	std::map<char, LPSPRITE> fonts;

	LPSPRITE playerSprite = NULL;
	vector<LPSPRITE> remainTimeSprites;
	vector<LPSPRITE> lifeSprites;
	vector<LPSPRITE> moneySprites;
	vector<LPSPRITE> scoreSprites;
	LPSPRITE cardSprites;
	vector<int> cards;
	CAnimationSet* TakenCards = NULL;


	int marioLife = 4;
	int score = 0;
	int money = 0;
	int time = 0;
	int remainTime = DEFAULT_TIME;

	int type_hud;

	bool isTakingCard = false;
	DWORD startTakingCard = 0;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	HUD(int type_hud = 0);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetHUD(HUD* hud);
	void initFonts();
	LPSPRITE mappingFont(char c);
	vector<LPSPRITE> StringToSprite(string str);
	void AddScore();
	void AddCoin();
	void RenderCard();
	void AddCard();
};