#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

class InputManager
{
private:
	static InputManager *iInstance; /*!<	Singleton instance*/

	enum numbers
	{
		ZERO,
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE
	};
	int number;

	bool quit;
	bool esc;
	bool back;
	bool pause;
	bool clickLeft, clickRight;
	bool enter;

public:
	InputManager();
	~InputManager();

	//! Gets Singleton instance.
	static InputManager *getInstanceInput();

	void Update();

	void SetCursorRelative(bool active);

	int CheckNumber();
	bool CheckQuit();
	bool CheckESC();
	bool CheckBack();
	bool CheckPause();
	bool CheckClickLeft();
	bool CheckClickRight();
	// bool CheckEnter();

	void ResetNumber();
	void ResetESC();
	void ResetClick();
	void ResetPause();

	void SetNumber(int num);

	void GetMouseXY(int &x, int &y);
};

#endif