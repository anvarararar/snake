#pragma once
#include "view.h"
#include "game.h"
#include <cctype>

class HumanC {
private:
	Snake& snake;
	void Control(int key) {//ФУНКЦИЯ ОБРАБОТЧИК
		switch (std::toupper(key)) {
			case 'W':
				snake.dir_=Y_DEC;
				return;
			case 'S':
				snake.dir_=Y_INC;
				return;
			case 'A':
				snake.dir_=X_DEC;
				return;
			case 'D':
				snake.dir_=X_INC;
				return;
		}
	}
public:
	HumanC(Game &game, Color clr = YELLOW) : snake(game.makeSnake()) {
		snake.clr_ = clr;
		View::get()->subscribeKey(std::bind(&HumanC::Control, this, std::placeholders::_1));
	}
};
