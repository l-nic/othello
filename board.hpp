#ifndef BOARD_HPP
#define BOARD_HPP

// The board class represents a board and current player making a move
class Board
{
public:
	typedef char piece_t;
private:
	piece_t whoseTurn;
	piece_t board[8][8];
	bool checkFlip(int x, int y, int deltaX, int deltaY);
	void flipPieces(int x, int y, int deltaX, int deltaY);
public:
	static const piece_t BLACK;
	static const piece_t WHITE;
	static const piece_t EMPTY;
	Board();
	piece_t getWhosePiece();
	piece_t getOpponentPiece();
	void setCurrentPlayer(piece_t player);
	void display();
	void makeMove(int x, int y);
	bool validMove(int x, int y);
	void getMoveList(int moveX[], int moveY[], int &numMoves);
	void getRandomMove(int &x, int &y);
	bool gameOver();
	int heuristic(piece_t whoseTurn);
	int score(piece_t piece);
};

#endif // BOARD_HPP
