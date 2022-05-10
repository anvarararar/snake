#include "game.h"
#include "view.h"
#include <cctype>
#include <random>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <iostream>

namespace {

std::default_random_engine rand_gen{static_cast<unsigned>(time(0))};
}//анонимное просторанство имён

Coord nextCord(Coord pos, Direction dir) {
//
auto sz = View::get()->getSize();

switch (dir) {
case X_INC:
pos.x = (pos.x + 1 + sz.x) % sz.x;
return pos;
case X_DEC:
pos.x = (pos.x - 1 + sz.x) % sz.x;
return pos;
case Y_INC:
pos.y = (pos.y + 1 + sz.y) % sz.y;
return pos;
case Y_DEC:
pos.y = (pos.y - 1 + sz.y) % sz.y;
return pos;
case DIRS_N:
default:
return pos;
}
}

Game::Game(unsigned rabs_n, unsigned snks_l, unsigned period) :
	rabs_n_(rabs_n),
	snakes_l_(snks_l)
{
	auto view = View::get();
	view->subscribeTimer(std::bind(&Game::tick, this), period);//ВОЗВРАЩАЕТ УКАЗАТЕЛЬ НА ВЬЮ
	view->subscribeKey([=](int key){//ЛЯМБДА ФУНКЦИЯ В КРУГЛЫХ - АРГ ФУНКЦИИ В ФИГУРНЫХ ЧТО ИСПОЛНЯЕТСЯ
			if (std::toupper(key) == 'Q')//KEY - КЛЮЧ(КЛАВИША) TOUPPER -ПОЛУЧАЕТ КОД СИМВОЛА ПО АСКИ И ВОЗВРАЩАЕТ БОЛЬШУЮ Q
				view->stop();
			});
}

Snake &Game::makeSnake() {
	std::list<Coord> body;
	body.push_back(getFreePos());//КООРДИНАТЫ ГОЛОВЫ
	auto dir = X_INC;
	for (unsigned i = 0; i < snakes_l_; i++)
		body.push_back(body.back());
	snakes_.push_back(Snake(std::move(body), dir));
	return snakes_.back();//ДОБАВЛЯЕМ НОВУЮ ЗМЕЮ В МАССИВ ВСЕХ ЗМЕЙ
}

Snake::Snake(std::list<Coord> &&body, Direction dir) : cords_(body), dir_(dir) {
}

Rabbit::Rabbit(Coord p) : pos(p) {
}

bool Game::checkIfFree(Coord cord) const {//проверяет что нет конфеты и тела змеи
	auto rab_pred = [&](auto rab){ return rab.pos == cord;};//АВТО - АВТОМАТИЧЕСКИ ОПРЕДЕЛЯЕТ ТИП
	return std::any_of(rabbits_.cbegin(), rabbits_.cend(), rab_pred)//ПРОБЕГАЕТ ПО ЛИСТУ
		|| checkIfCanMove(cord);
}

bool Game::checkIfCanMove(Coord cord) const {//нет тела змеи
	auto snake_pred = [&](auto snake){
		auto body = snake.getCords();
		return std::any_of(body.cbegin(), body.cend(), 
				[&](auto pos){return pos == cord; }); };
	return std::any_of(snakes_.cbegin(), snakes_.cend(), snake_pred);
}

Coord Game::getFreePos() const {
	Coord sz = View::get()->getSize();
	Coord cord;
	auto x_distr = std::uniform_int_distribution<int>(0, sz.x);
	auto y_distr = std::uniform_int_distribution<int>(0, sz.y);
	do {
		cord.x = x_distr(rand_gen);
		cord.y = y_distr(rand_gen);
	} while (checkIfFree(cord));
	return cord;
}


void Game::tick() {
	auto view = View::get();
	auto sz = view->getSize();
	rabbits_.remove_if([=](auto r) 
			{ return !r.pos.inside(sz) && (view->drawEmpty(r.pos), true); }//УДАЛЯЕТ ВСЕХ КРОЛИКО В ТЕКУЩЕМ КАДРЕ
		);
	while (rabbits_.size() < rabs_n_)
		rabbits_.emplace_back(getFreePos());//ДОБАВЛЕНИЕ НОВОГО КРОЛИКА
	for (auto r : rabbits_) {
		view->drawRab(r.pos);//ОТРИСОВЫВЕТ ВСЕХ НОВЫХ КРОЛИКОВ
	}
	for (auto&& snake : snakes_) {
		if (!snake.can_move) {
			continue;//ОБРАБАТЫВАЕТ ДВИЖЕНИЕ ЗМЕИ В КАДРЕ
		}
		auto next = nextCord(snake.cords_.front(), snake.dir_);//СЛЕДУЮЩАЯ КООРДИНАТА КУДА НАДО ПЕРЕМЕСТИТЬСЯ
		auto pred = [=] (auto &&s) {
			auto &b = s.cords_;//
			return std::find(b.cbegin(), b.cend(), next) != b.cend();
		};
		if (	(next.x < 0 && snake.dir_ == X_DEC )	||
			(next.x > sz.x && snake.dir_ == X_INC)	||
			(next.y < 0 && snake.dir_ == Y_DEC)	||
			(next.y > sz.y && snake.dir_ == Y_INC)	||
			std::find_if(snakes_.cbegin(), snakes_.cend(), pred) != snakes_.cend())
		{
			killers_.emplace_back(&snake);//ДОБАВЛЯЕТ В КОНЕЦ МАССИВА KILLERS ЗМЕЮ КОТОРУЮ НАДО УБИТЬ
		} else {
			auto rab_it = std::find(rabbits_.begin(), rabbits_.end(), next);
			if (rab_it != rabbits_.end()) {
				snake.cords_.emplace_front(next);//ПРИКРЕПЛЯЕТ ГОЛОВУ А ХВОСТ ОСТАВЛЯЕТ ПРИ СЪЕДАНИИ
				view->drawSnake(snake.cords_.front(), snake.clr_);
				rabbits_.erase(rab_it);//УДАЛЕНИЕ СЪЕДЕННОГО КРОЛИКА
			} else {
				snake.cords_.emplace_front(next);
				view->drawSnake(snake.cords_.front(), snake.cords_.back(), snake.clr_);
				snake.cords_.pop_back();//ЕСЛИ НЕ СЪЕЛИ КОНФЕТУ
			}
		}
		for (auto &&killer : killers_)
			killer();//МАССИВ KILLERS - МАССИВ ЗМЕЙ КОТОРЫХ НУЖНО УБИТЬ
	}
}

bool operator == (Coord lhs, Coord rhs) {
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
bool operator == (Rabbit lhs, Rabbit rhs) {
	return lhs.pos == rhs.pos;
}
