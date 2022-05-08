OBJ = main.o view.o tui.o gui.o game.o
PROG = snake
LIBS =-lsfml-graphics -lsfml-window -lsfml-system
CXXFLAGS +=-Wall -Wextra -g -std=c++17

$(PROG): $(OBJ)
	$(CXX) -o $(PROG) $(OBJ) $(LIBS)

depend:
	$(CXX) -MM $(OBJ:.o=.cc) > .depend
-include .depend
