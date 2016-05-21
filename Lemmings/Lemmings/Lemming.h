#pragma once
#include "ElementGame.h"
#include "Map.h"

// NOTA: Si els getters no s'utilitzen, s'haur�n d'esborrar. Els setters es poden deixar.
class Lemming :
	public ElementGame
{
private:
	int estat;
	int dir; // Direcci� del Lemming a l'hora de moure's || 0 / 2.
	SDL_RendererFlip flipType;
	int temps;
	int velocitat;
	bool paraigues;
	bool escalar;
	int pintW, pintH;

	InputManager* inputManager;

public:
	Lemming();
	~Lemming();

	void init(int x, int y);
	void update(Map *fons, int x1, int y1, int x2, int y2);
	void render();

	int GetPosX();
	int GetPosY();
	int GetWidth();
	int GetHeight();
	int GetDir();

	bool SetSkill(int numUsos, int skill);

	void SetDir(int dir);
	void SetAnimacio();
	/*
	MOVE: Moures, FALL: Caure, BREAK: Trencar Parets, GLIDE: Levitar, DIG: Cavar,
	STOP: Inmovilitzat, STAIR: Posar rajoles,PICK: Cavar amb el Pic 
	EXPLOSION: Foc de l'Explosio s, DEAD: El Lemming es destruit, DEADFALL: El Lemming cau d'una gran altura
	OPENUMBRELLA: El Lemming obra el paraig�es, NOSTAIR: El Lemming es queda sense rajoles, ENDCLIMB El Lemming acaba d'escalar i torna a terra pla
	EXPLODING: El Lemming fa l'animacio de que va a explotar*/
	enum EstatsEnum{
		MOVE, FALL, BREAK, GLIDE, CLIMB, DIG, PICK, STOP, STAIR, EXPLOSION, 
		DEAD, DEADFALL, OPENUMBRELLA, NOSTAIR, ENDCLIMB, EXPLODING
	};
	int GetEstat();

	// Sets per canviar l'Estat 
	void SetMoure();   
	void SetCaure();   

	void SetForadarParet();
	void SetObrirParaigues();
	void SetPlanejarParaigues();
	void SetCavar();
	void SetPicar();
	void SetImmobilitzar();
	void SetConstruirEscala();
	void SetNoEscales();
	void SetExplosio(); 
	void SetExplotar();
	void SetEscalar(); 
	void SetFinalEscalar(); 
	void SetMortCaure(); 

	// Estats temporals 
	void Moure();
	void Moure(bool diagAmunt);
	void TrencarMur();
	void Levitar();
	void Escalar();
	void Cavar();
	void Picar();
	void Caure();
	void Immobilitzar();
	void ConstruirEscala();
	void Explotar();

	// Equipables, com Paraig�es i Escalar
	void SetParaigues();
	void SetEscalar();

};

