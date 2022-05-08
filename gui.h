#include "view.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Gui : public View {
	const unsigned cell_sz = 10;
	sf::RenderWindow window_;
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
	Gui();
};
