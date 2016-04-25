#pragma once
#include "Utils.h"
#include "SingletonManager.h"

class Element
{
protected:
	int posX, posY; //Posici� de l'element en la pantalla.
	const char* imatge;
	int srcPosX, srcPosY; //Posici� inicial en el sprite. Per agafar una regi� de l'sprite, cal fer el requadre amb width i height.
	int width, height; //Amplada i al�ada de l'sprite.

	// Animacio de SpriteSheet
	int numImatges;
	int contImatges;
	int fpsAnimacio;
	bool saltaImatges;

	SingletonManager* sManager;

public:
	Element();
	~Element();

	void Init(int x, int y, const char* img, int srcX, int srcY, int w, int h);
	void Render();

	//Animacio de SpriteSheet
	void SetAnimacio();
};

