#pragma once
#include "view.h"
#include "game.h"
#include <functional>
#include <algorithm>

class NearestC {
private:
	Snake& snake_;
	Game &game_;
	const std::list<Rabbit> &rabs_;
	const std::list<Snake> &snks_;

	void Control() {
		auto head = snake_.cords_.front();
		auto rab = std::min_element(rabs_.begin(), rabs_.end(),
                        [ trnsfm = [head](auto rab) {
                            auto dx = rab.pos.x - head.x;
                            auto dy = rab.pos.y - head.y;
                            return dx * dx + dy * dy;
                        }] (const auto &lhs, const auto &rhs) {
                            return trnsfm(lhs) < trnsfm(rhs);
                        })->pos;//ВЫЧИСЛЯЕТ БЛИЖАЙШУЮ
		if (head.x < rab.x &&
                        game_.checkIfFree(nextCord(head, X_INC))) {
			snake_.dir_ = X_INC;
                        return;
                }
		if (head.x > rab.x &&
                        game_.checkIfFree(nextCord(head, X_DEC))) {
			snake_.dir_ = X_DEC;
                        return;
                }
		if (head.y < rab.y &&
                        game_.checkIfFree(nextCord(head, Y_INC))) {
			snake_.dir_ = Y_INC;
                        return;
                }
		if (head.y > rab.y &&
                        game_.checkIfFree(nextCord(head, Y_DEC))) {
			snake_.dir_ = Y_DEC;
                        return;
                }
	}
public:
	NearestC(Game &game) :
		snake_(game.makeSnake()),
		game_(game),
		rabs_(game.getRabbits()),
		snks_(game.getSnakes())
	{
		View::get()->subscribeTimer(std::bind(&NearestC::Control, this));
	}
};