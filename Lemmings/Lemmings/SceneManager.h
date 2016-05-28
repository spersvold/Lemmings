#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Utils.h"
#include "Scene.h"
#include "SingletonManager.h"

//#define _LASTSCENE

class SceneManager
{
public:
	~SceneManager();

	//! Gets Singleton instance.
	static SceneManager* getInstanceSM();

	//! Enum Scenes.
	/*! All the Scenes in the game. */
	enum SceneEnum{
		MENU, CODE_LEVEL, /*PRE_GAME,*/ GAME
	};

	void init();
	void update();
	void render();

	void changeScene(int next_scene);
	void reinitScene(int scene);

	int getCurrScene();

private:
	SceneManager();

	static SceneManager* smInstance; /*!<	Singleton instance*/
	
	vector<Scene*> mVectorScenes;
	int mCurrScene;

	VideoManager* videoManager;
};

#endif