#pragma once
#include <vector>

namespace PuffEngine {
class IMainGame;// you can do it if all of member on this header are pointers
class IGameScreen;

class ScreenList {
public:
	ScreenList(IMainGame* game);
	~ScreenList();
	IGameScreen* moveNext();
	IGameScreen* movePrevious();

	void setScreen(int nextScreen);
	void addScreen(IGameScreen* newScreen);

	void destroy();

	IGameScreen* getCurrent();
protected:
	IMainGame* game_ = nullptr;
	std::vector<IGameScreen*> screens_;
	//pointer to the elemnts of the vector is BADDDD
	// keep track of the current screen index
	int currentScreenIndex_ = -1;
};

}
