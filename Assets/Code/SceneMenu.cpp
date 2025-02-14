#include "SceneMenu.h"


SceneMenu* SceneMenu::gInstance = NULL;

SceneMenu::SceneMenu() {
	sGame = SceneGame::getInstanceSceneGame();
	smManager = SceneManager::getInstanceSM();

	float scaleX = 0.65f;
	float scaleY = 0.8f;
	background = new Background();
	background->init(0, 0, "Assets/Art/Images/Menu/menu.png", false, 0, 0, 809, 446, scaleX, scaleY);

	idMusic = audioManager->getMusicID("Assets/Art/Audios/Music/start.wav");

	Button button;
	int wButton = static_cast<int>(132.f * scaleX);
	int hButton = static_cast<int>(48.f * scaleY);
	button.init(PLAYER, 98, 188, wButton, hButton, scaleX, scaleY, 0, 0, NULL, NULL);
	buttons.push_back(button);
	button.init(MUTE_SOUND, 422, 188, wButton, hButton, scaleX, scaleY, 0, 0, NULL, NULL);
	buttons.push_back(button);
	button.init(EXIT, 260, 285, wButton, hButton - 1, scaleX, scaleY, 0, 0, NULL, NULL);
	buttons.push_back(button);

	Background imgAudio;
	int wImgAudio = 135;
	int hImgAudio = 50;
	imgAudio.init(422, 188, "Assets/Art/Images/Menu/Fx.png", false, 0, 0, wImgAudio, hImgAudio, scaleX, scaleY);
	imgAudios[0] = imgAudio;
	imgAudio.init(422, 188, "Assets/Art/Images/Menu/Music.png", false, 0, 0, wImgAudio, hImgAudio, scaleX, scaleY);
	imgAudios[1] = imgAudio;

	currAudio = Music;
}

SceneMenu::~SceneMenu() {
}


SceneMenu* SceneMenu::getInstanceSceneMenu() {
	if (gInstance == NULL)
		gInstance = new SceneMenu();

	return gInstance;
}


void SceneMenu::init() {
	audioManager->playMusic(idMusic, -1);
}

void SceneMenu::update() {
	updateButtons();
	updateInput();
}

void SceneMenu::render() {
	background->render();
	if (currAudio != Mute)
		imgAudios[currAudio].render();
}


void SceneMenu::updateButtons() {
	for (itBut = buttons.begin(); itBut != buttons.end(); itBut++) {
		if (itBut->update()) {
			switch (itBut->getId()) {
			case PLAYER:
				audioManager->pauseMusic();
				inputManager->setCursorRelative(true);
				smManager->changeScene(smManager->PRE_GAME);
				break;
			case MUTE_SOUND:
				switch (currAudio) {
				case Mute:
					currAudio = Fx;
					audioManager->setVolumeMusic(0);
					audioManager->setVolumeSound(audioManager->Volume);
					break;
				case Fx:
					currAudio = Music;
					audioManager->setVolumeMusic(audioManager->Volume);
					audioManager->setVolumeSound(audioManager->Volume);
					break;
				case Music:
					currAudio = Mute;
					audioManager->setVolumeMusic(0);
					audioManager->setVolumeSound(0);
					break;
				}
				break;
			case EXIT:
				inputManager->setNumber(EXIT);
				break;
			}
			inputManager->resetClick();
		}
	}
}

void SceneMenu::updateInput() {
	switch (inputManager->getNumber()) {
	case PLAYER:
		audioManager->pauseMusic();
		inputManager->setCursorRelative(true);
		smManager->changeScene(smManager->PRE_GAME);
		break;
	case MUTE_SOUND:
		switch (currAudio) {
		case Mute:
			currAudio = Fx;
			audioManager->setVolumeMusic(0);
			audioManager->setVolumeSound(audioManager->Volume);
			break;
		case Fx:
			currAudio = Music;
			audioManager->setVolumeMusic(audioManager->Volume);
			audioManager->setVolumeSound(audioManager->Volume);
			break;
		case Music:
			currAudio = Mute;
			audioManager->setVolumeMusic(0);
			audioManager->setVolumeSound(0);
			break;
		}
		break;
	}
	if (inputManager->getNumber() != -1)
		inputManager->resetNumber();
}