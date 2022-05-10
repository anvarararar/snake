#pragma once
#include <functional>
#include <list>
#include <string_view>

struct Coord {
	int x;
	int y;
	bool inside(Coord sz) const {
		return x > 0 && x < sz.x && y > 0 && y < sz.y;
	}
};

enum Color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE}
;

struct View {
	using KeyFn = std::function<void(int)>; 
	using TimerFn = std::function<void()>;
protected:
	std::list<KeyFn>  callkey_;//МАССИВЫ С СОБЫТИЯМИ ПРИНИМАЕТ ИНТ ВОЗВР VOID - НАЖАТИЯ
	std::list<TimerFn> calltimer_;//СОБЫТИЯ ИГРЫ
	int period_;
public:
	View(const View &) = delete;
	View & operator = (const View &) = delete;
	
	virtual void draw() = 0;//МЕТОДЫ КОТОРЫЕ ИСПОЛЬЗУЮТСЯ В GUI  И TUI
	virtual void stop() = 0;
	virtual void runloop() = 0;
	
	virtual void drawEmpty(Coord pos) = 0;
	virtual void drawRab(Coord pos) = 0;
	virtual void drawSnake(Coord head, Color clr = BLACK) = 0;
	virtual void drawSnake(Coord head, Coord tail, Color clr = BLACK) = 0;
	virtual void drawSnake(std::list<Coord> body, Color clr = BLACK) = 0;

	void subscribeKey(KeyFn callkey);
	void subscribeTimer(TimerFn calltimer, int period = 0);//
	
	virtual Coord getSize() const = 0;
	
	virtual ~View() = default;
	static View *get(std::string_view arg = "");
	View() = default;
};
