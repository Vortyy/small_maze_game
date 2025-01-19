#!WARNING you need to export LD_LIBRARY_PATH w/o it you binary won't be able to link to libdynamic_array.so 

#launch the game with default map
launch : 
	game map.txt

#build the game and launch directly after
build_launch : build_game
	game map.txt

#build the game
build_game : 
	clang src/rayTest.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -L./lib/ -ldynamic_array -o game

clean :
	rm -rf game
