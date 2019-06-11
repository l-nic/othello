#ifndef BOARD_HPP
#define BOARD_HPP

// The board class represents a board and current player making a move
class Board
{
private:
	char whoseTurn;
	char board[8][8];
	bool checkFlip(int x, int y, int deltaX, int deltaY);
	void flipPieces(int x, int y, int deltaX, int deltaY);
public:
	Board();
	char getWhosePiece();
	char getOpponentPiece();
	void setCurrentPlayer(char player);
	void display();
	void makeMove(int x, int y);
	bool validMove(int x, int y);
	void getMoveList(int moveX[], int moveY[], int &numMoves);
	void getRandomMove(int &x, int &y);
	bool gameOver();
	int heuristic(char whoseTurn);
	int score(char piece);
};

#endif // BOARD_HPP
