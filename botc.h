#pragma once
#include "view.h"
#include "game.h"
#include <functional>

class BotC {
private:
	Snake& snake_;
	Game &game_;
	const std::list<Rabbit> &rabs_;
	const std::list<Snake> &snks_;
	
	void Control() {
		auto head = snake_.cords_.front();//БЕРЁТ КООРДИНАТЫ ГОЛОВЫ ВЫБИРАЕТ ПЕРВУЮ КОНФЕТУ ИЗ СПИСКА И ДВИЖЕТСЯ К НЕЙ
		auto rab = rabs_.front().pos;
		if (head.x < rab.x)
			// && game_.checkIfFree(nextCord(head, X_INC)))
			snake_.dir_ = X_INC;
		if (head.x > rab.x)
			// && game_.checkIfFree(nextCord(head, X_DEC)))
			snake_.dir_ = X_DEC;
		if (head.y < rab.y)
			//&& game_.checkIfFree(nextCord(head, Y_INC)))
			snake_.dir_ = Y_INC;
		if (head.y > rab.y)
			//&& game_.checkIfFree(nextCord(head, Y_DEC)))
			snake_.dir_ = Y_DEC;//СНАЧАЛА ИДЁТ ПО ВЕРТИКАЛИ
	}
public:
	BotC(Game &game) :
		snake_(game.makeSnake()),
		game_(game),
		rabs_(game.getRabbits()),
		snks_(game.getSnakes())
	{
		View::get()->subscribeTimer(std::bind(&BotC::Control, this));
	}
};
