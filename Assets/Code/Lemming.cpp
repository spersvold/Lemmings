#include "Lemming.h"


Lemming::Lemming(){
	mapPosX = mapPosY = 0;
	width = height = 0;

	limitX = 0;

	currState = FALL;
	direction = 0;

	currFallTime = lifeTime = 0;

	displacement = 0;

	hasUmbrella = false;
	canClimb = false;

	initialFallenDead = isDead = false;

	isGoingToExplode = false;

	levelTime = 0;

	isImmobilized = false;

	isRescued = false;

	currNumberStairsBuilt = 0;

	for (int i = 0; i < MaxSounds; i++) {
		idSounds[i] = -1;
	}

	flipType = SDL_FLIP_NONE;

	inputManager = SingletonManager::getInstanceSingleton()->getInputManager();
}

Lemming::~Lemming(){
}


void Lemming::init(const int x, const int y, const int mapPosX, const int mapPosY){
	ElementGame::init(x, y, "Assets/Art/Images/Lemmings/lem_ani.png", false, 0, 40, 10, 10, 1, 1, 318, 0, 20, 4, 2);
	this->mapPosX = mapPosX;
	this->mapPosY = mapPosY;
	width = 20;
	height = 20;

	limitX = 8;
	
	currState = FALL;
	direction = 0;
	
	currFallTime = 0;
	lifeTime = 4;
	
	displacement = 1;
	hasUmbrella = false;
	canClimb = false;
	initialFallenDead = false;
	isDead = false;
	isGoingToExplode = false;
	levelTime = 0;

	isImmobilized = false;

	isRescued = false;

	currNumberStairsBuilt = 0;

	idSounds[BeforeExplode] = audioManager->getSoundID("Assets/Art/Audios/Sounds/beforeExplode.wav");
	idSounds[Explosion] = audioManager->getSoundID("Assets/Art/Audios/Sounds/explosion.wav");
	idSounds[FallenDead] = audioManager->getSoundID("Assets/Art/Audios/Sounds/fallDead.wav");
	idSounds[Stairs] = audioManager->getSoundID("Assets/Art/Audios/Sounds/stair.wav");
	idSounds[Yippee] = audioManager->getSoundID("Assets/Art/Audios/Sounds/yippee.wav");

	flipType = SDL_FLIP_NONE;
}

void Lemming::update(Map* map, const int x1, const int y1, const int x2, const int y2, const int time) {
	switch (currState) {
	case MOVE:
		updateMove(map, x1, x2, y2);
		break;
	case FALL:
		updateFall(map, x1, x2, y2);
		break;
	case BREAK:
		updateBreak(map, x1, y1, x2, y2);
		break;
	case GLIDE:
		updateGlide(map, x1, x2, y2);
		break;
	case CLIMB:
		updateClimb(map, x1, y1, x2, y2);
		break;
	case DIG:
		updateDig(map, x1, y1, x2, y2);
		break;
	case PICK:
		updatePick(map, x1, y1, x2, y2);
		break;
	case IMMOBILE:
		if (!isImmobilized) {
			Immobilize(map, x1, x2, y1, y2);
			isImmobilized = true;
		}
		break;
	case STAIRS:
		updateStairs(map, x1, x2, y2);
		break;
	case NO_STAIRS:
		if (currentSprite == numSprites) {
			SetMove();
		}
		break;
	case DEAD:
		break;
	case DEAD_FALL:
		if (currentSprite == numSprites) {
			isDead = true;
		}
		break;
	case OPEN_UMBRELLA:
		break;
	case END_CLIMB:
		break;
	case RESCUED:
		if (!isRescued) {
			if (currentSprite == numSprites)
				isRescued = true;
		}
		break;
	case EXPLODING:
		if (currentSprite == numSprites) {
			SetExplosion();
		}
		break;
	case EXPLOSION:
		Explode(map, x1, y1, x2, y2);
		break;
	default:
		break;
	}

	if (isGoingToExplode) {
		CheckExplosion(time);
	}

	UpdateAnimation();
}

void Lemming::updateMove(Map* map, const int x1, const int x2, const int y2) {
	if (map->GetMap(x1 + 1, y2 + 1) == 0 && map->GetMap(x2 - 1, y2 + 1) == 0) {
		SetFall();
	}
	else if (direction == 0) { // To the right.
		if (map->GetMap(x2, y2 - 1) != 0 && map->GetMap(x2, y2 - 2) == 0)
			Move(true); // Diagonal up.
		else if (map->GetMap(x2, y2) == 0 && map->GetMap(x2, y2 + 1) != 0)
			Move(false); // Down.
		else if (map->GetMap(x2 + 1, y2 - 2) != 0) {
			if (map->GetMap(x2 + 1, y2 - 2) == 1 && canClimb) {
				SetClimb();
			}
			else {
				SetDirection(2);
			}
		}
		else {
			Move();
		}
	}
	else { // To the left.
		if (map->GetMap(x1, y2 - 1) != 0 && map->GetMap(x1, y2 - 2) == 0)
			Move(true); // Diagonal up.
		else if (map->GetMap(x1, y2) == 0 && map->GetMap(x1, y2 + 1) != 0)
			Move(false); // Down.
		else if (map->GetMap(x1, y2 - 2) != 0) {

			if (map->GetMap(x1, y2 - 2) == 1 && canClimb) {
				SetClimb();
			}
			else {
				SetDirection(0);
			}
		}
		else
			Move();
	}
}

void Lemming::updateFall(Map* map, const int x1, const int x2, const int y2) {
	if (hasUmbrella) {
		SetOpenUmbrella();
		return;
	}

	Fall();
	if (map->GetMap(x1 + displacement, y2 + displacement) != 0 || map->GetMap(x2 - displacement, y2 + displacement) != 0) {
		if (initialFallenDead) {
			SetFallenDeath();
		}
		else {
			SetMove();
		}
	}
}

void Lemming::updateBreak(Map* map, const int x1, const int y1, const int x2, const int y2) {
	if ((map->GetMap(x1 + 1, y2 + 1) == 0 && map->GetMap(x1, y1 + 1) == 0) || (map->GetMap(x2 + 1, y2 + 1) == 0 && map->GetMap(x2, y1 + 1) == 0)) {
		SetMove();
	}
	else {
		BreakWall(map, x1, x2, y1, y2);
	}
}

void Lemming::updateGlide(Map* map, const int x1, const int x2, const int y2) {
	Glide();
	if (map->GetMap(x1 + displacement, y2 + displacement) != 0 || map->GetMap(x2 - displacement, y2 + displacement) != 0) {
		SetMove();
	}
}

void Lemming::updateClimb(Map* map, const int x1, const int y1, const int x2, const int y2) {
	Climb();
	if ((map->GetMap(x2, y2) == 0 && map->GetMap(x2, y2 + 1) == 0) && (map->GetMap(x2 + 1, y2) == 0 && map->GetMap(x2 + 1, y2 + 1) != 0) ||
		(map->GetMap(x1, y2) == 0 && map->GetMap(x1, y2 + 1) == 0) && (map->GetMap(x1 - 1, y2) == 0 && map->GetMap(x1 - 1, y2 + 1) != 0)) {
		Move();
		SetMove();
	}
	else if (map->GetMap(x1, y1) == 1 && map->GetMap(x2, y1) == 1) {
		SetFall();
	}
}

void Lemming::updateDig(Map* map, const int x1, const int y1, const int x2, const int y2) {
	if ((map->GetMap(x1 - 1, y1 + 1) == 0 && map->GetMap(x2 + 1, y1 + 1) == 0) && map->GetMap(x2, y2 + 1) == 0) {
		SetMove();
	}
	else {
		Dig(map, x2, y2);
	}
}

void Lemming::updatePick(Map* map, const int x1, const int y1, const int x2, const int y2) {
	if ((map->GetMap(x1 - 1, y1 + 1) == 0 && map->GetMap(x2 + 1, y1 + 1) == 0) && map->GetMap(x2, y2 + 1) == 0) {
		SetMove();
	}
	else {
		Pick(map, x2, y2);
	}
}

void Lemming::updateStairs(Map* map, const int x1, const int x2, const int y2) {
	// TODO: When the position is fixed, the '-1' must be removed from the condition.
	if ((direction == 0 && (map->GetMap(x2 + 1, y2 - 1) != 0 && map->GetMap(x2 + 2, y2 - 1) != 0)) ||
		(direction == 2 && (map->GetMap(x1 - 1, y2 - 1) != 0 && map->GetMap(x1 - 2, y2 - 1) != 0))) {
		SetMove();
		currNumberStairsBuilt = 0;
		return;
	}
	else if (currentSprite == 11) {
		PutStep(map, x1, x2, y2);
		currNumberStairsBuilt++;
		if (currNumberStairsBuilt == 25 || currNumberStairsBuilt == 28 || currNumberStairsBuilt == 33)
			audioManager->playSound(idSounds[Stairs]);
	}
	else if (currentSprite == numSprites) {
		Move(true);
		if (currNumberStairsBuilt >= 32) {
			SetNoStairs();
			currNumberStairsBuilt = 0;
		}
	}
}

void Lemming::render(){
	videoManager->renderTexture(idImg, srcPosX, srcPosY, width, height, scaleX, scaleY, posX + mapPosX, posY + mapPosY, 0, 0, 0, flipType);
}


int Lemming::GetLimitX(){
	return limitX;
}

int Lemming::GetDirection(){
	return direction;
}

Lemming::StatesEnum Lemming::GetCurrState() {
	return currState;
}

bool Lemming::GetIsDead(){
	return isDead;
}

bool Lemming::GetIsGoingToExplode(){
	return isGoingToExplode;
}

bool Lemming::GetIsRescued(){
	return isRescued;
}


bool Lemming::SetSkill(int numUses, int skill, int time){
	if (numUses > 0){
		switch (skill){ // If they don't have the skill, they're put in.
		case 2: // CLIMB
			if (!canClimb && currState != IMMOBILE){
				canClimb = true;
				return true;
			}
			break;
		case 3: // UMBRELLA
			if (!hasUmbrella && currState != IMMOBILE){
				hasUmbrella = true;
				return true;
			}
			break;
		case 4: // EXPLODING
			if (!isGoingToExplode && currState != EXPLODING && currState != EXPLOSION && currState != FALL){
				SetLevelTimeToExplode(time);
				return true;
			}
			break;
		case 5: // IMMOBILE
			if (currState != IMMOBILE && currState != FALL && currState != CLIMB){
				SetImmobilize();
				return true;
			}
			break;
		case 6: // STEPS
			if (currState != STAIRS && currState != IMMOBILE && currState != FALL && currState != CLIMB){
				SetBuiltStairs();
				return true;
			}
			break;
		case 7: // BREAK WALL
			if (currState != BREAK && currState != IMMOBILE && currState != FALL && currState != CLIMB){
				SetBreakWall();
				return true;
			}
			break;
		case 8: // PICK
			if (currState != PICK && currState != IMMOBILE && currState != FALL && currState != CLIMB){
				SetPick();
				return true;
			}
			break;
		case 9: // DIG
			if (currState != DIG && currState != IMMOBILE && currState != FALL && currState != CLIMB){
				SetDig();
				return true;
			}
			break;
		}
	}

	return false;
}

void Lemming::SetDirection(int dir) {
	direction = dir;
	flipType = dir == 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

void Lemming::SetMove(){
	currState = MOVE;
	numSprites = 10;
	srcPosX = _srcPosX = 0;
	srcPosY = 0;
}

void Lemming::SetFall(){
	currState = FALL;
	numSprites = 4;
	srcPosX = _srcPosX = 0;
	srcPosY = 40;
	currFallTime = 0;
}

void Lemming::SetFallenDeath(){
	audioManager->playSound(idSounds[FallenDead]);
	currState = DEAD_FALL;
	numSprites = 16;
	srcPosX = _srcPosX = 0;
	srcPosY = 220;
}

void Lemming::SetBreakWall(){
	currState = BREAK;
	numSprites = 32;
	srcPosX = _srcPosX = 0;
	srcPosY = 120;
}

void Lemming::SetOpenUmbrella(){
	currState = OPEN_UMBRELLA;
	numSprites = 4;
	srcPosX = _srcPosX = 80;
	srcPosY = 40;
	height = 16;
	SetGlidingUmbrella();
}

void Lemming::SetGlidingUmbrella(){
	currState = GLIDE;
	numSprites = 4;
	srcPosX = _srcPosX = 140;
	srcPosY = 40;
}

void Lemming::SetDig(){
	currState = DIG;
	numSprites = 8;
	srcPosX = _srcPosX = 0;
	srcPosY = 160;
}

void Lemming::SetPick(){
	currState = PICK;
	numSprites = 24;
	srcPosX = _srcPosX = 160;
	srcPosY = 160;
}

void Lemming::SetImmobilize(){
	currState = IMMOBILE;
	numSprites = 16;
	srcPosX = _srcPosX = 0;
	srcPosY = 60;
}

void Lemming::SetBuiltStairs(){
	currState = STAIRS;
	numSprites = 16;
	srcPosX = _srcPosX = 0;
	srcPosY = 100;
}

void Lemming::SetNoStairs(){
	currState = NO_STAIRS;
	numSprites = 7;
	srcPosX = _srcPosX = 200;
	srcPosY = 0;
}

void Lemming::SetClimb(){
	currState = CLIMB;
	numSprites = 9;
	srcPosX = _srcPosX = 0;
	srcPosY = 80;
}

void Lemming::SetFinalClimb(){
	currState = END_CLIMB;
	numSprites = 7;
	srcPosX = _srcPosX = 180;
	srcPosY = 80;
}

void Lemming::SetExploding() {
	audioManager->playSound(idSounds[BeforeExplode]);
	currState = EXPLODING;
	numSprites = 16;
	srcPosX = _srcPosX = 0;
	srcPosY = 200;
}

void Lemming::SetExplosion() {
	audioManager->playSound(idSounds[Explosion]);
	currState = EXPLOSION;
	numSprites = 9;
	srcPosX = _srcPosX = 100;
	srcPosY = 260;
}

void Lemming::SetLevelTimeToExplode(int time){
	isGoingToExplode = true;
	levelTime = time;
}

void Lemming::SetRescued(){
	audioManager->playSound(idSounds[Yippee]);
	currState = RESCUED;
	numSprites = 4;
	srcPosX = _srcPosX = 20;
	srcPosY = 20;
}


bool Lemming::IsCursorOnLemming() {
	int mouseX, mouseY;
	inputManager->getMouseXY(mouseX, mouseY);

	return ((mouseX > posX + mapPosX && mouseX < posX + mapPosX + width)
		&& (mouseY > posY + mapPosY && mouseY < posY + mapPosY + height));
}

void Lemming::PutUmbrella() {
	hasUmbrella = true;
}

void Lemming::PutClimb() {
	canClimb = true;
}

void Lemming::Move(){
	switch (direction){
	case 0:
		posX += displacement;
		break;
	case 2:
		posX -= displacement;
		break;
	}
}

void Lemming::Move(bool isDiagonalUp){
	if (isDiagonalUp){
		Move();
		posY -= displacement;
	}
	else
		posY += displacement;
}

void Lemming::BreakWall(Map* map, int x1, int x2, int y1, int y2){
	if (currentSprite == 8 || currentSprite == 16 || currentSprite == 24 || currentSprite == 32){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 4; j++){
				map->ChangeMapAtPos(x2 + i, y2 - j, 0, 0);
			}
		}
		Move();
	}
}

void Lemming::Glide(){
	if (currentSprite == 2 || currentSprite == 4){
		posY += displacement;
	}
}

void Lemming::Climb(){
	if (currentSprite == 3 || currentSprite == 6 || currentSprite == 9){
		posY -= displacement;
	}
}

// Destroy the terrain downward.
void Lemming::Dig(Map* map, int x2, int y2){
	if (currentSprite == 6){
		for (int i = -1; i < 2; i++){
			map->ChangeMapAtPos(x2 + i, y2, 0, 0);
		}
		posY += displacement;
	}
}

// Destroy the terrain downward and diagonally.
void Lemming::Pick(Map* map, int x2, int y2){
	if (currentSprite == 5){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 4; j++){
				map->ChangeMapAtPos(x2 + i, y2 - j, 0, 0);
			}
			map->ChangeMapAtPos(x2 + i, y2, 0, 0);
		}
		Move();
		posY += displacement;
	}
}

void Lemming::Immobilize(Map* map, int x1, int x2, int y1, int y2){
	for (int i = 1; i < 3; i++){
		for (int j = -4; j < 3; j++){
			if (map->GetMap(x1 + i, y2 + j) == 0){
				map->ChangeMapAtPos(x1 + i, y2 + j, 3);
			}

			if (map->GetMap((x2 - 2) + i, y2 + j) == 0){
				map->ChangeMapAtPos((x2 - 2) + i, y2 + j, 3);
			}
		}
	}
}

void Lemming::PutStep(Map* map, int x1, int x2, int y2){
	switch (direction){
	case 0:
		for (int i = 0; i < 3; i++){
			if (map->GetMap(x2 + i, y2) == 0)
				map->ChangeMapAtPos(x2 + i, y2, 3, 3);
		}
		break;
	case 2:
		for (int i = 0; i < 3; i++){
			if (map->GetMap(x1 - i, y2) == 0)
				map->ChangeMapAtPos(x1 - i, y2, 3, 3);
		}
		break;
	}
}

void Lemming::Fall(){
	posY += displacement;

	if (currFallTime == fallMaxTime){
		initialFallenDead = true;
	}
	else if (currFallTime < fallMaxTime){
		currFallTime++;
	}
}

void Lemming::CheckExplosion(int time){
	int currTime = time - levelTime;
	if (currTime >= lifeTime){
		SetExploding();
		isGoingToExplode = false;
	}
}

// Destroys the terrain around it.
void Lemming::Explode(Map* map, int x1, int y1, int x2, int y2){
	if (currentSprite == numSprites){
		for (int i = -6; i < 3; i++){
			map->ChangeMapAtPos(x1, y2 + i, 0, 0);
			map->ChangeMapAtPos(x1 + 1, y2 + i, 0, 0);
			map->ChangeMapAtPos(x1 + 2, y2 + i, 0, 0);
			map->ChangeMapAtPos(x1 + 3, y2 + i, 0, 0);

			map->ChangeMapAtPos(x2 + 1, y2 + i, 0, 0);
			map->ChangeMapAtPos(x2, y2 + i, 0, 0);
			map->ChangeMapAtPos(x2 - 1, y2 + i, 0, 0);
			map->ChangeMapAtPos(x2 - 2, y2 + i, 0, 0);
		}
		isDead = true;
	}
}