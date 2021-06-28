#pragma once
#include <vector>

using namespace std;

class CBackupHud {
	static CBackupHud* __instance;
	int marioLife = 4;
	int score = 0;
	int money = 0;
	vector<int> marioCards;
public:
	
	void SetMarioLife(int life) { marioLife = life; }
	void SetMarioScore(int score) { this->score = score; }
	void SetMarioMoney(int money) { this->money = money; }
	void SetMarioCards(vector<int> cards);

	void BackUp(int life, int score, int money, vector<int> cards);

	int GetMarioLife() { return marioLife; }
	int GetScore() { return score; }
	int GetMoney() { return money; }
	vector<int> GetMarioCards() { return marioCards; }

	static CBackupHud* GetInstance();
};