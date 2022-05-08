#include "view.h"
#include <termios.h>
#include <list>

class Tui : public View {
	termios term;
	bool loop_flag_ = true;

	void clear();
	void chgPos(int x = 0, int y = 0);
	void chgPosRel(int x, int y = 0);
	void savePos();
	void restorePos();
	void drawHorLine(int len);
	void drawVerLine(int len);
	void setColor(Color clr = BLACK);
public:
	void draw() override;
	void stop() override;
	void runloop() override;
	void drawEmpty(Coord pos) override;
	void drawRab(Coord pos) override;
	void drawSnake(Coord head, Color clr) override;
	void drawSnake(Coord head, Coord tail, Color clr) override;
	void drawSnake(std::list<Coord> body, Color clr) override;
	Coord getSize() const override;
	~Tui();
	Tui();
};
