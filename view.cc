#include "gui.h"
#include "tui.h"
#include <cstring>

View *View::get(std::string_view arg) {
	static View *view = (arg == "gui")		?
		reinterpret_cast<View *>(new Gui{})	:
		reinterpret_cast<View *>(new Tui{})	;
	return view;
}

void View::subscribeKey(std::function<void(int)> callkey) {//ДОБАВЛЯЕТ ФУНКЦИЮ ОБРАБОТЧИКА ДЛЯ ОБЪЕКТОВ - НАЖАТИЙ КЛАВИШ
	callkey_.push_back(callkey);
}

void View::subscribeTimer(std::function<void()> calltimer, int period) {//ДВИЖЕНИЯ ЗВМЕЙКИ - ПОКАДРОВЫЕ
	calltimer_.push_back(calltimer);
	if (period)
		period_ = period;
}
