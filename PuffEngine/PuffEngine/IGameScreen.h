#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace PuffEngine {
;
class IMainGame;

enum class ScreenState {
	NONE,
	RUNNING,
	EXIT_APPLICATION,
	CHANGE_NEXT,
	CHANGE_PREVIOUS
};

class IGameScreen {
public:
	friend class ScreenList;
	IGameScreen() {
		//Empty
	}
	virtual ~IGameScreen() {
		//Empty
	}
	//Returns the index of the next or pervious screen
	virtual int getNextScreenIndex() const = 0;
	virtual int getPreviousScreenIndex() const = 0;

	//Called beginnin and end of app, pure virtual
	virtual void build() = 0;
	virtual void destroy() = 0;
	
	//Called  when a screen enters and exits focus
	virtual void onEntry() = 0;
	virtual void onExit() = 0;

	//Called  main game loop
	virtual void update() = 0;
	virtual void draw() = 0;

	//getters
	int getScreenIndex() const { return screenIndex_; }
	ScreenState getState() const { return currentState_; }
	//setters
	void setParentGame(IMainGame* game) { game_ = game; }
	void setRunning() { currentState_ = ScreenState::RUNNING; }
protected:
	ScreenState currentState_ = ScreenState::NONE;
	IMainGame* game_ = nullptr;

	int screenIndex_ = -1;

};


}
