#include "tui.h"
#include <cstdio>
#include <csignal>
#include <sys/ioctl.h>
#include <poll.h>
#include <chrono>

namespace {

volatile winsize size_;//
struct sigaction sa;
void winSizeChgHandler(int sig) {
	ioctl(1, TIOCGWINSZ, &size_);
}

}

Tui::Tui() {
	setbuf(stdout, NULL);
	clear();
	ioctl(1, TIOCGWINSZ, &size_);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = winSizeChgHandler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGWINCH, &sa, NULL);

	tcgetattr(0, &term);
	termios newterm = term;
	cfmakeraw(&newterm);
	tcsetattr(0, TCSANOW, &newterm);
}

Tui::~Tui() {
	tcsetattr(0, TCSANOW, &term);
}

Coord Tui::getSize() const {
	return Coord{size_.ws_col - 4, size_.ws_row - 4};
}

void Tui::clear() {
	std::printf("\e[2J\e[?25l");
}

void Tui::chgPos(int x, int y) {
	std::printf("\e[%d;%dH", y, x);
}

void Tui::chgPosRel(int x, int y) {
	char xc, yc;
	if (x < 0) {
		xc = 'D';
		x = -x;
	} else
		xc = 'C';
	if (y < 0) {
		yc = 'A';
		y = -y;
	} else
		yc = 'B';
	if (x)
		std::printf("\e[%d%c", x, xc);
	if (y)
		std::printf("\e[%d%c", y, yc);
}

void Tui::savePos() {
	std::printf("\e[s");
}

void Tui::restorePos() {
	std::printf("\e[u");
}

void Tui::setColor(Color clr) {
	std::printf("\e[%dm", 40 + clr);

}

void Tui::draw() {
	setColor();
	clear();
	chgPos();
	drawHorLine(size_.ws_col);
	chgPos();
	drawVerLine(size_.ws_row);
	chgPos(size_.ws_col, 0);
	drawVerLine(size_.ws_row - 1);
	chgPos(0, size_.ws_row); 
	drawHorLine(size_.ws_col - 1);
}

void Tui::stop() {
	loop_flag_ = false;
	system("clear"); 
}

void Tui::runloop() {
	pollfd in;
	in.fd = 0;
	in.events = POLLIN;

	auto nexttime = period_;
	while (loop_flag_) {
		auto start = std::chrono::system_clock::now();
		int res = poll(&in, 1, nexttime);
		if (res < 0) {
			perror("Poll error: ");//ОБРАБОТКА ОШИБОК
			return;
		}
		if (res > 0) {
			auto end = std::chrono::system_clock::now();
			int key = std::getchar();
			for (auto &&sub : callkey_)
				sub(key);//ОБРАБОТКА НАЖАТИЙ
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			nexttime = period_ - dur.count();
		}
		if (nexttime < 0 || res == 0) {
			for (auto &&sub : calltimer_)
				sub();//ОБРАБОТКА ДВИЖЕНЯ ЗМЕЙКИ
			nexttime = period_;
		}
	}
}

void Tui::drawHorLine(int len) {
	for (int i = 0; i < len; ++i)
		std::putchar('-');
}

void Tui::drawVerLine(int len) {
	for (int i = 0; i < len; ++i) {
		savePos();
		std::putchar('|');
		restorePos();
		chgPosRel(0, 1);
          }
}

void Tui::drawEmpty(Coord pos) {
	setColor();
	chgPos(pos.x + 2, pos.y + 2);
	putchar(' ');
}

void Tui::drawRab(Coord pos) {
	setColor();
	chgPos(pos.x + 2, pos.y + 2);
	putchar('#');
}

void Tui::drawSnake(std::list<Coord> body, Color clr) {
	setColor(clr);
	for (auto cord : body) {
		chgPos(cord.x + 2, cord.y + 2);
		putchar('o');
	}
}

void Tui::drawSnake(Coord head, Color clr) {
	setColor(clr);
	chgPos(head.x + 2, head.y + 2);
	putchar('o');
}

void Tui::drawSnake(Coord head, Coord tail, Color clr) {
	drawSnake(head, clr);
	drawEmpty(tail);
}
