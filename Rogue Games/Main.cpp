#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <string>
#include <stdexcept>



int OBSTACLES = 100; // # of obstacles generated
struct Hero;

class Board { // Class for a board with 2D Vector for height and width. 
	int WIDTH;
	int HEIGHT;
	std::vector<std::vector<char>> map;
	bool placeHero;
public:
	void drawBoard();
	Board(int h, int w):HEIGHT(h), WIDTH(w), map(HEIGHT, std::vector<char>(WIDTH, '.')), placeHero(false) {};
	void fillBoard(Hero& h);
	void heroPlaced() { placeHero = true; }
	void accessMap(int& x, int& y, const char& ch) { map[x][y] = ch; }
	char returnData(int& x, int& y) { return map[x][y]; }
};

struct Hero { // Hero class with predetermined moves and score. Methods for returning moves and score and moving the hero around the board.
private:
	int moves;
	int score;
	int xCoord;
	int yCoord;
public:
	Hero():moves(25), score(0) {};
	int movesRemaining() { return moves; }
	void subtractMove() { --moves; }
	void moveHero(Board& b);
	void setCoords(int& x, int& y) { xCoord = x, yCoord = y; }
	void getDirection(Board& b);
	void checkScore(Board& b, int& x, int& y);
	int getScore() { return score; }
};

void Board::drawBoard() { // Function the draws the board using a simple nested for loop.
	for (int i = 0; i < HEIGHT; ++i) {
		for (int y = 0; y < WIDTH; ++y) {
			std::cout << map[i][y];
		}
		std::cout << std::endl;
	}
}

void Board::fillBoard(Hero& h) { // fills the board with walls and items. Items are determined to fill 25% of the board. Also sets a flag once the "hero" is placed.

	for (int i = 0; i < HEIGHT; ++i) {
		for (int y = 0; y < WIDTH; ++y) {

			int item = rand() % 5;
			int hero = rand() % 100;

			if (i == 0 || i == HEIGHT - 1)
				map[i][y] = '%';
			else if (y == 0 || y == WIDTH - 1)
				map[i][y] = '%';
			else if (item == 1 && OBSTACLES > 0) {
				map[i][y] = '$';
				--OBSTACLES;
			}
			else if (hero == 25 && placeHero == false) {
				map[i][y] = '@';
				heroPlaced();
				h.setCoords(i, y);
			}
		}
	}
}

void Hero::getDirection(Board& b) { //Standard output of directions for moving the hero and score and how many moves are left before the end.

		while (moves > 0 && score < 1000) {
			std::cout << "\nMove your Hero using W, A, S, D";
			std::cout << std::string(2, '\n');
			std::cout << "Score: " << score << '\n';
			std::cout << "Moves left: " << moves << std::string(2, '\n');
			moveHero(b);
			b.drawBoard();
		}
}

void Hero::moveHero(Board& b) { // received input from the user to determine the direction the hero should be moved using a switch statement. Also contains a exception catch if the user attempts to move the Hero outside of 
								// of the predetermined map.
	int x = 0;
	int y = 0;
	char MOVE;
	try{
		std::cin >> MOVE;

		switch (MOVE){
		case 'w':
			--moves;
			x = xCoord - 1;
			checkScore(b, x, yCoord);
			b.accessMap(x, yCoord, '@');
			b.accessMap(xCoord, yCoord, '.');
			setCoords(x, yCoord);
			break;
		case 's':
			--moves;
			x = xCoord + 1;
			checkScore(b, x, yCoord);
			b.accessMap(x, yCoord, '@');
			b.accessMap(xCoord, yCoord, '.');
			setCoords(x, yCoord);
			break;
		case 'd':
			--moves;
			y = yCoord + 1;
			checkScore(b, xCoord, y);
			b.accessMap(xCoord, y, '@');
			b.accessMap(xCoord, yCoord, '.');
			setCoords(xCoord, y);
			break;
		case 'a':
			--moves;
			y = yCoord - 1;
			checkScore(b, xCoord, y);
			b.accessMap(xCoord, y, '@');
			b.accessMap(xCoord, yCoord, '.');
			setCoords(xCoord, y);
			break;
		}
	}
	catch (std::out_of_range& e) {
		std::cerr << e.what();
	}
}

void Hero::checkScore(Board& b, int& x, int& y) { // function to check if the Hero has reached a "$" or "%". Score will be incremented or exception will be thrown pending.
	if (b.returnData(x, y) == '$')
		score += 10;
	if (b.returnData(x, y) == '%')
		throw std::out_of_range("The way is blocked...\n");

}

int main() {
	char answer;
	srand(time(NULL));
	do {
		Hero John;
		Board Map = Board(20, 20);
		Map.fillBoard(John);
		Map.drawBoard();
		John.getDirection(Map);

		if (John.movesRemaining() == 0)
			std::cout << "\nYou're out of moves! Your score is: " << John.getScore() << '\n';
		if (John.getScore() == 1000)
			std::cout << "\nCongratulations! You collected all of the treasure!\n";

		std::cout << "Would you like to play again? Y/N\n";
		std::cin >> answer;
	} while (answer == 'Y');
}


