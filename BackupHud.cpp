#include "BackupHud.h"

CBackupHud* CBackupHud::__instance = NULL;

void CBackupHud::SetMarioCards(vector<int> cards) {
	marioCards.clear();
	if (cards.size() > 0) {
		for (size_t i = 0; i < cards.size(); i++) {
			marioCards.push_back(cards[i]);
		}
	}
}

void CBackupHud::BackUp(int life, int score, int money, vector<int> cards) {
	this->marioLife = life;
	this->score = score;
	this->money = money;
	marioCards.clear();
	if (cards.size() > 0) {
		/*for (size_t i = 0; i < cards.size(); i++) {
			marioCards.push_back(cards[i]);
		}*/
	}
}

CBackupHud* CBackupHud::GetInstance() {
	if (__instance == NULL) __instance = new CBackupHud();
	return __instance;
}