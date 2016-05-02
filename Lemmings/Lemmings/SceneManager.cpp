#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneMenu.h"
#include "SceneGame.h"
#include "SceneCodeLevel.h"

SceneManager* SceneManager::smInstance = NULL;

SceneManager::SceneManager(){
	videoManager = SingletonManager::getInstanceSingleton()->getVideoManager();
}

SceneManager::~SceneManager(){
	videoManager->close();
}


SceneManager* SceneManager::getInstanceSM(){
	if (smInstance == NULL)
		smInstance = new SceneManager();
	
	return smInstance;
}


void SceneManager::init(){
	videoManager->clearScreen(0xFFFFFF);
	videoManager->updateScreen();

	mVectorScenes.clear();

	mVectorScenes.push_back(new SceneLogo);
	mVectorScenes.push_back(new SceneMenu);
	mVectorScenes.push_back(new SceneCodeLevel);
	mVectorScenes.push_back(new SceneGame);

	for (int i = 0; i < mVectorScenes.size(); i++)
		mVectorScenes[i]->init();

	//mCurrScene = LOGO;
	mCurrScene = MENU;
}


void SceneManager::update(){
	mVectorScenes[mCurrScene]->update();
	videoManager->updateTime();
}


void SceneManager::render(){
	videoManager->clearScreen(0xFFFFFF);
	mVectorScenes[mCurrScene]->render();
	videoManager->updateScreen();
}


void SceneManager::changeScene(SceneEnum next_scene){
	mCurrScene = next_scene;
}


int SceneManager::getCurrScene(){
	return mCurrScene;
}