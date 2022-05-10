#include "tui.h"
#include "gui.h"
#include "game.h"
#include "humanc.h"
#include "botc.h"
#include "nearestc.h"
#include <cstring>
#include <memory>

int main(int argc, char **argv) {
	std::unique_ptr<View> ui{View::get(argc > 1 ? argv[1] : "tui")};//класс отвечает за всю программу создаёт объект игры
	ui->draw();
	Game game{};
	HumanC huc{game};
	BotC bot{game};
	BotC bota{game};//ищет первую в списке
	//NearestC bot1{game};//ближайшую
	//NearestC bot2{game};
	//NearestC bot3{game};
	//NearestC bot4{game};
	//NearestC bot5{game};
	ui->runloop();
}
