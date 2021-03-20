CXX = g++

GDB = -ggdb
CXXFLAGS = -std=c++2a -c -Wall -Wextra $(GDB) -fcoroutines

EXEC = twitchbot

LIBS = -lsockpp

OBJECTS = main.o bot.o timerhandler.o parser.o commandhandler.o pingcommand.o changeprefix.o lurkcommand.o helpcommand.o editresult.o addtimer.o edittimer.o
	

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) -O2 -o $@ $^ $(LIBS)
	@echo "Succesfully build"

main.o: main.cpp bot.o
	$(CXX) $(CXXFLAGS) $<

bot.o: src/bot.cpp parser.o timerhandler.o commandhandler.o
	$(CXX) $(CXXFLAGS) $<

timerhandler.o: src/timerhandler.cpp
	$(CXX) $(CXXFLAGS) $<

parser.o: src/parser.cpp commandhandler.o
	$(CXX) $(CXXFLAGS) $<

commandhandler.o: src/commandhandler.cpp pingcommand.o changeprefix.o lurkcommand.o helpcommand.o editresult.o addtimer.o edittimer.o
	$(CXX) $(CXXFLAGS) $<

pingcommand.o: src/commands/pingcommand.cpp
	$(CXX) $(CXXFLAGS) $<

changeprefix.o: src/commands/changeprefix.cpp
	$(CXX) $(CXXFLAGS) $<

lurkcommand.o: src/commands/lurkcommand.cpp
	$(CXX) $(CXXFLAGS) $<

helpcommand.o: src/commands/helpcommand.cpp
	$(CXX) $(CXXFLAGS) $<

editresult.o: src/commands/editresult.cpp
	$(CXX) $(CXXFLAGS) $<

addtimer.o: src/commands/addtimer.cpp timerhandler.o
	$(CXX) $(CXXFLAGS) $<

edittimer.o: src/commands/edittimer.cpp
	$(CXX) $(CXXFLAGS) $<

# Phony targets:
clean: # Remove all object files
	rm $(wildcard *.o)

veryclean:
	rm $(EXEC) $(wildcard *.o)

todo: # List all todo's
	grep -n "TODO:" *.[ch]pp

tree: # List the dependency tree
	gcc -MM $(LIBS) *.[ch]pp

# Set the phony targets
.PHONY: clean veryclean todo tags
