#pragma once
#include "HUD.h"
#include <string>
#include "PlayScence.h"

#define HUD_DIFF_FIRST_ROW		8
#define HUD_DIFF_SECOND_ROW		16
#define HUD_DIFF_PLAYER			11
#define HUD_DIFF_TIME			132
#define HUD_DIFF_MONEY			140
#define HUD_DIFF_LIFE			36
#define HUD_DIFF_SCORE			60
#define HUD_DIFF_P				107
#define HUD_DIFF_CARD			172

#define HUD_TIME_MAX	3
#define HUD_SCORE_MAX	7

void HUD::initFonts() {
	CSprites* sprites = CSprites::GetInstance();
	fonts.insert(make_pair('0', sprites->Get(SPRITE_FONT_0_ID)));
	fonts.insert(make_pair('1', sprites->Get(SPRITE_FONT_1_ID)));
	fonts.insert(make_pair('2', sprites->Get(SPRITE_FONT_2_ID)));
	fonts.insert(make_pair('3', sprites->Get(SPRITE_FONT_3_ID)));
	fonts.insert(make_pair('4', sprites->Get(SPRITE_FONT_4_ID)));
	fonts.insert(make_pair('5', sprites->Get(SPRITE_FONT_5_ID)));
	fonts.insert(make_pair('6', sprites->Get(SPRITE_FONT_6_ID)));
	fonts.insert(make_pair('7', sprites->Get(SPRITE_FONT_7_ID)));
	fonts.insert(make_pair('8', sprites->Get(SPRITE_FONT_8_ID)));
	fonts.insert(make_pair('9', sprites->Get(SPRITE_FONT_9_ID)));
	fonts.insert(make_pair('A', sprites->Get(SPRITE_FONT_A_ID)));
	fonts.insert(make_pair('B', sprites->Get(SPRITE_FONT_B_ID)));
	fonts.insert(make_pair('C', sprites->Get(SPRITE_FONT_C_ID)));
	fonts.insert(make_pair('D', sprites->Get(SPRITE_FONT_D_ID)));
	fonts.insert(make_pair('E', sprites->Get(SPRITE_FONT_E_ID)));
	fonts.insert(make_pair('F', sprites->Get(SPRITE_FONT_F_ID)));
	fonts.insert(make_pair('G', sprites->Get(SPRITE_FONT_G_ID)));
	fonts.insert(make_pair('H', sprites->Get(SPRITE_FONT_H_ID)));
	fonts.insert(make_pair('I', sprites->Get(SPRITE_FONT_I_ID)));
	fonts.insert(make_pair('J', sprites->Get(SPRITE_FONT_J_ID)));
	fonts.insert(make_pair('K', sprites->Get(SPRITE_FONT_K_ID)));
	fonts.insert(make_pair('L', sprites->Get(SPRITE_FONT_L_ID)));
	fonts.insert(make_pair('M', sprites->Get(SPRITE_FONT_M_ID)));
	fonts.insert(make_pair('N', sprites->Get(SPRITE_FONT_N_ID)));
	fonts.insert(make_pair('O', sprites->Get(SPRITE_FONT_O_ID)));
	fonts.insert(make_pair('P', sprites->Get(SPRITE_FONT_P_ID)));
	fonts.insert(make_pair('Q', sprites->Get(SPRITE_FONT_Q_ID)));
	fonts.insert(make_pair('R', sprites->Get(SPRITE_FONT_R_ID)));
	fonts.insert(make_pair('S', sprites->Get(SPRITE_FONT_S_ID)));
	fonts.insert(make_pair('T', sprites->Get(SPRITE_FONT_T_ID)));
	fonts.insert(make_pair('U', sprites->Get(SPRITE_FONT_U_ID)));
	fonts.insert(make_pair('V', sprites->Get(SPRITE_FONT_V_ID)));
	fonts.insert(make_pair('W', sprites->Get(SPRITE_FONT_W_ID)));
	fonts.insert(make_pair('X', sprites->Get(SPRITE_FONT_X_ID)));
	fonts.insert(make_pair('Y', sprites->Get(SPRITE_FONT_Y_ID)));
	fonts.insert(make_pair('Z', sprites->Get(SPRITE_FONT_Z_ID)));
}

LPSPRITE HUD::mappingFont(char c) {
	int ic = (int)c;
	if (!(ic >= 48 && ic <= 57 || ic >= 64 && ic <= 90))
		return NULL;
	return fonts.at(c);
}

vector<LPSPRITE> HUD::StringToSprite(string str)
{
	vector<LPSPRITE> sprites;
	LPSPRITE sprite;
	char c;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		c = (char)str[i];
		sprite = mappingFont(c);
		if (sprite != NULL)
			sprites.push_back(sprite);
	}
	return sprites;
}

HUD::HUD(int typeHUD) {
	initFonts();
	playerSprite = CSprites::GetInstance()->Get(SPRITE_ICONMARIO_ID);
}

void HUD::Render() {
	CSprites::GetInstance()->Get(SPRITE_HUD_ID)->Draw(x, y);
	playerSprite->Draw(x + HUD_DIFF_PLAYER, y + HUD_DIFF_SECOND_ROW);
	for (unsigned int i = 0; i < remainTimeSprites.size(); i++)
		remainTimeSprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_TIME, y + HUD_DIFF_SECOND_ROW);
	for (unsigned int i = 0; i < moneySprites.size(); i++)
		moneySprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_MONEY, y + HUD_DIFF_FIRST_ROW);
	for (unsigned int i = 0; i < lifeSprites.size(); i++)
		lifeSprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_LIFE, y + HUD_DIFF_SECOND_ROW);
	for (unsigned int i = 0; i < scoreSprites.size(); i++)
		scoreSprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_SCORE, y + HUD_DIFF_SECOND_ROW);
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	AddScore();
	AddCoin();
	time += dt;
	remainTime = DEFAULT_TIME - time / 1000;

	string time_str = to_string(remainTime);
	while (time_str.length() < HUD_TIME_MAX) time_str = "0" + time_str;
	remainTimeSprites = StringToSprite(time_str);

	string score_str = to_string(score);
	while (score_str.length() < HUD_SCORE_MAX) score_str = "0" + score_str;
	scoreSprites = StringToSprite(score_str);

	lifeSprites = StringToSprite(to_string(marioLife));
	moneySprites = StringToSprite(to_string(money));
}

void HUD::SetHUD(HUD* hud)
{
	marioLife = hud->marioLife;
	remainTime = hud->remainTime;
	score = hud->score;
	money = hud->money;
	time = hud->time;
	//runningStacks = hud->runningStacks;
}

void HUD::AddScore() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	mario = currentScene->GetPlayer();
	if (mario->marioScore > 0)
		this->score = mario->marioScore;
}

void HUD::AddCoin() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	mario = currentScene->GetPlayer();
	this->money = mario->coin;
}
