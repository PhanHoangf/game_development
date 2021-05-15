#pragma once
class CCamera {
private:
	float camX;
	float camY;
public:
	void SetCamPos(float x, float y) {
		camX = x;
		camY = y;
	}

	float GetCamX() {
		return camX;
	}

	float GetCamY() {
		return camY;
	}
};

typedef CCamera* LPCAMERA;