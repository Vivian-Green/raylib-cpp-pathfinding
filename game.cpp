#include "game.h"
#include <random>
#include <iostream>

Game::Game(){
	grid = Grid();
	gameOver = false;
	score = 0;
}

void Game::Draw() {
	//grid.PathFind(5);
	grid.Frame(); 
}