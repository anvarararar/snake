#pragma once
#include <list>
#include "view.h"

enum Direction {X_INC = 0, X_DEC, Y_INC, Y_DEC, DIRS_N};
Coord nextCord(Coord pos, Direction dir);

struct Rabbit {
	Coord pos;
	Rabbit(Coord p);
};

bool operator == (Coord lhs, Coord rhs);
bool operator == (Rabbit lhs, Rabbit rhs);

class Snake {
	public:
	bool can_move = true;
	std::list<Coord> cords_;
	Color clr_ = WHITE;
	Direction dir_;
	auto getCords() const {
		return cords_;
	}
	Snake(std::list<Coord> &&body, Direction dir);
};

class SnakeKiller {//ОЧИЩАЕТ ЗМЕЙКУ ЕСЛИ МИГАЛА ДОСТ-Е КОЛ-ВО
	int i = 0;
	Snake *snake_;
	static constexpr int TIMES_TO_FLICKER = 10;
public:
	SnakeKiller(Snake *snake) : snake_(snake) {
		snake_->can_move = false;
	}
	void operator()() {
		if (i % 2)
			View::get()->drawSnake(snake_->cords_, snake_->clr_);
		else {
			for (auto pos : snake_->cords_)
				View::get()->drawEmpty(pos);
		}
		if (++i >= TIMES_TO_FLICKER) {
			for (auto pos : snake_->cords_)
				View::get()->drawEmpty(pos);
			snake_->cords_.clear();	
		}
	}
};

class Game final {
	unsigned rabs_n_;
	unsigned snakes_l_;
	std::list<Rabbit> rabbits_;
	std::list<Snake> snakes_;
	std::list<SnakeKiller> killers_;
	Coord getFreePos() const;
	Coord getFreePos(Coord prev_pos) const;
	void killSnake(Snake &snake);
public:
	void tick();
	Game(unsigned rabs_n = 10, unsigned snks_l = 3, unsigned period = 150);
	Snake &makeSnake();
	const auto &getSnakes() const {
		return snakes_;
	}
	const auto &getRabbits() const {
		return rabbits_;
	}
	bool checkIfFree(Coord cord) const;
	bool checkIfCanMove(Coord cord) const;
};
