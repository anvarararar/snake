OBJ = main.o view.o tui.o gui.o game.o
PROG = snake
LIBS =-lsfml-graphics -lsfml-window -lsfml-system
CXXFLAGS +=-Wall -Wextra -g -std=c++17

$(PROG): $(OBJ) humanc.h botc.h game.h gui.h tui.h view.h
	$(CXX) -o $(PROG) $(OBJ) $(LIBS)

depend:
	$(CXX) -MM $(OBJ:.o=.cc) > .depend
-include .depend
