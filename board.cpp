#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "board.hpp"

using namespace std;

const Board::piece_t Board::BLACK = 'X';
const Board::piece_t Board::WHITE = 'O';
const Board::piece_t Board::EMPTY = '.';

// Initialize the board in the constructor
Board::Board()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = Board::EMPTY;
	board[3][3] = Board::BLACK;
	board[3][4] = Board::WHITE;
	board[4][3] = Board::WHITE;
	board[4][4] = Board::BLACK;
}

// New methods to get the player currently moving and the opponent
Board::piece_t Board::getWhosePiece()
{
	return whoseTurn;
}

Board::piece_t Board::getOpponentPiece()
{
	if (whoseTurn == Board::BLACK)
		return Board::WHITE;
	return Board::BLACK;
}

void Board::setCurrentPlayer(Board::piece_t player)
{
	whoseTurn = player;
}

// Output the board
void Board::display()
{
	for (int y = 7; y >= 0; y--)
	{
		cout << y << " ";
		for (int x = 0; x < 8; x++)
			cout << " " << board[x][y];
		cout << endl;
	}
	cout << "   0 1 2 3 4 5 6 7" << endl;
}

// Checks a direction from x,y to see if we can make a move
bool Board::checkFlip(int x, int y, int deltaX, int deltaY)
{
	Board::piece_t opponentPiece = getOpponentPiece();
	Board::piece_t myPiece = whoseTurn;
	if (board[x][y] == opponentPiece)
	{
		while ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
		{
			x += deltaX;
			y += deltaY;
			// BUG in original code, allows -1 or 8. For example
			// if x = 0 and deltaX is -1, then now x = -1
			// I patched this with another if statement, but it might be better
			// to move the += to the bottom of the loop
			if ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
			{
				if (board[x][y] == Board::EMPTY) // not consecutive
					return false;
				if (board[x][y] == myPiece)
					return true;		// At least one piece we can flip
				else
				{
					// It is an opponent piece, just keep scanning in our direction
				}
			}
		}
	}
	return false; // Either no consecutive opponent pieces or hit the edge of the board
}

// Flips pieces in the given direction until we don't hit any more opponent pieces.
// Assumes this is a valid direction to flip (we eventually hit one of our pieces).
void Board::flipPieces(int x, int y, int deltaX, int deltaY)
{
	while (board[x][y] == getOpponentPiece())
	{
		board[x][y] = whoseTurn;;
		x += deltaX;
		y += deltaY;
	}
}


// Makes a move on the board, assumes the move is valid.
void Board::makeMove(int x, int y)
{
	// Put the piece at x,y
	board[x][y] = whoseTurn;

	// Check to the left
	if (checkFlip(x - 1, y, -1, 0))
		flipPieces(x - 1, y, -1, 0);
	// Check to the right
	if (checkFlip(x + 1, y, 1, 0))
		flipPieces(x + 1, y, 1, 0);
	// Check down
	if (checkFlip(x, y - 1, 0, -1))
		flipPieces(x, y - 1, 0, -1);
	// Check up
	if (checkFlip(x, y + 1, 0, 1))
		flipPieces(x, y + 1, 0, 1);
	// Check down-left	
	if (checkFlip(x - 1, y - 1, -1, -1))
		flipPieces(x - 1, y - 1, -1, -1);
	// Check down-right
	if (checkFlip(x + 1, y - 1, 1, -1))
		flipPieces(x + 1, y - 1, 1, -1);
	// Check up-left	
	if (checkFlip(x - 1, y + 1, -1, 1))
		flipPieces(x - 1, y + 1, -1, 1);
	// Check up-right
	if (checkFlip(x + 1, y + 1, 1, 1))
		flipPieces(x + 1, y + 1, 1, 1);
}

// Returns true if the move is valid, false if invalid
bool Board::validMove(int x, int y)
{
	// Check that the coordinates are empty
	if (board[x][y] != Board::EMPTY)
		return false;

	// If we can flip in any direction, it is valid
	// Check to the left
	if (checkFlip(x - 1, y, -1, 0))
		return true;
	// Check to the right
	if (checkFlip(x + 1, y, 1, 0))
		return true;
	// Check down
	if (checkFlip(x, y - 1, 0, -1))
		return true;
	// Check up
	if (checkFlip(x, y + 1, 0, 1))
		return true;
	// Check down-left	
	if (checkFlip(x - 1, y - 1, -1, -1))
		return true;
	// Check down-right
	if (checkFlip(x + 1, y - 1, 1, -1))
		return true;
	// Check up-left	
	if (checkFlip(x - 1, y + 1, -1, 1))
		return true;
	// Check up-right
	if (checkFlip(x + 1, y + 1, 1, 1))
		return true;

	return false; // If we get here, we didn't find a valid flip direction
}

// Fills in the arrays with valid moves for the piece.  numMoves is the number of valid moves.
void Board::getMoveList(int moveX[], int moveY[], int &numMoves)
{
	numMoves = 0;  // Initially no moves found

	// Check each square of the board and if we can move there, remember the coordinates
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
		{
			if (validMove(x, y)) // If find valid move, remember coordinates
			{
				moveX[numMoves] = x;
				moveY[numMoves] = y;
				numMoves++;		// Increment number of moves found
			}
		}
}

// True if the game is over, false if not over
bool Board::gameOver()
{
	int XMoveX[60], XMoveY[60], OMoveX[60], OMoveY[60];
	int numXMoves, numOMoves;

	getMoveList(XMoveX, XMoveY, numXMoves);
	// Temporarily flip whoseturn to opponent to get opponent move list
	whoseTurn = getOpponentPiece();
	getMoveList(OMoveX, OMoveY, numOMoves);
	whoseTurn = getOpponentPiece();  // Flip back to original
	if ((numXMoves == 0) && (numOMoves == 0))
		return true;
	return false;
}

// Using the move list, gets a random move out of this list
void Board::getRandomMove(int &x, int &y)
{
	int moveX[60], moveY[60], numMoves;
	getMoveList(moveX, moveY, numMoves);
	if (numMoves == 0)
	{
		x = -1;
		y = -1;
	}
	else
	{
		int i = rand() % numMoves;
		x = moveX[i];
		y = moveY[i];
	}
}

// Returns the score for the piece
int Board::score(Board::piece_t piece)
{
	int total = 0;
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y] == piece)
				total++;
		}
	return total;
}


// The simple heuristic is simply the number of our pieces - the number of opponent pieces.
// Weighting the edges and corners will result in a better player.
int Board::heuristic(Board::piece_t whoseTurn)
{	
	int ourScore = score(whoseTurn);
	Board::piece_t opponent = Board::BLACK;
	if (whoseTurn == Board::BLACK)
		opponent = Board::WHITE;
	int opponentScore = score(opponent);
	return (ourScore - opponentScore);
}

