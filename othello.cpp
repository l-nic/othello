#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <ratio>
#include <chrono>
#include <vector>
#include "board.hpp"

using namespace std;

// Function prototypes
void minimaxDecision(Board board, int maxPlyDepth, int &x, int &y);
int minimaxValue(Board board, char originalTurn, int searchPly, int maxPlyDepth);

// This is the minimax decision function. It calls minimaxValue for each position
// on the board and returns the best move (largest value returned) in x and y.
void minimaxDecision(Board board, int maxPlyDepth, int &x, int &y)
{	
	int moveX[60], moveY[60];
	int numMoves;
	char opponent = board.getOpponentPiece();

	board.getMoveList(moveX, moveY, numMoves);
	if (numMoves == 0) // if no moves return -1
	{
		x = -1;
		y = -1;
	}
	else
	{
		// Remember the best move
		int bestMoveVal = -99999;
		int bestX = moveX[0], bestY = moveY[0];
		// Try out every single move
		for (int i = 0; i < numMoves; i++)
		{
			// Apply the move to a new board
			Board tempBoard = board;
			tempBoard.makeMove(moveX[i], moveY[i]);
			// Recursive call
			// Set turn to opponent
			tempBoard.setCurrentPlayer(tempBoard.getOpponentPiece());
			int val = minimaxValue(tempBoard, board.getWhosePiece(), 1, maxPlyDepth);
			// Remember best move
			if (val > bestMoveVal)
			{
				bestMoveVal = val;
				bestX = moveX[i];
				bestY = moveY[i];
			}
		}
		// Return the best x/y
		x = bestX;
		y = bestY;
	}
}

// minimaxValue makes a recursive call to itself to search another ply in the tree.
// It is hard-coded here to look 5 ply ahead.  originalTurn is the original player piece
// which is needed to determine if this is a MIN or a MAX move.  It is also needed to 
// calculate the heuristic. currentTurn flips between X and O.
int minimaxValue(Board board, char originalTurn, int searchPly, int maxPlyDepth)
{
	if ((searchPly == maxPlyDepth) || board.gameOver()) // Change to desired ply lookahead
	{
		return board.heuristic(originalTurn); // Termination criteria
	}
	int moveX[60], moveY[60];
	int numMoves;
	char opponent = board.getOpponentPiece();

	board.getMoveList(moveX, moveY, numMoves);
	if (numMoves == 0) // if no moves skip to next player's turn
	{
		Board temp = board;
		temp.setCurrentPlayer(opponent);
		return minimaxValue(temp, originalTurn, searchPly + 1, maxPlyDepth);
	}
	else
	{
		// Remember the best move
		int bestMoveVal = -99999; // for finding max
		if (originalTurn != board.getWhosePiece())
			bestMoveVal = 99999; // for finding min
		// Try out every single move
		for (int i = 0; i < numMoves; i++)
		{
			// Apply the move to a new board
			Board tempBoard = board;
			tempBoard.makeMove(moveX[i], moveY[i]);
			// Recursive call
			// Opponent's turn
			tempBoard.setCurrentPlayer(tempBoard.getOpponentPiece());
			int val = minimaxValue(tempBoard, originalTurn, searchPly + 1, maxPlyDepth);
			// Remember best move
			if (originalTurn == board.getWhosePiece())
			{
				// Remember max if it's the originator's turn
				if (val > bestMoveVal)
					bestMoveVal = val;
			}
			else
			{
				// Remember min if it's opponent turn
				if (val < bestMoveVal)
					bestMoveVal = val;
			}
		}
		return bestMoveVal;
	}
	return -1;  // Should never get here
}

// Main game loop
int main()
{
	srand(time(NULL));
	Board gameBoard;
	gameBoard.setCurrentPlayer('X');

	int maxPlyDepth = 1;
	vector<double> timeSamples;

	while (!gameBoard.gameOver())
	{
		gameBoard.display();
		cout << "It is player " << gameBoard.getWhosePiece() << "'s turn." << endl;
		cout << "Enter move." << endl;
		int x, y;
		if (gameBoard.getWhosePiece() == 'O') // Change comments depending on who to play
		{
			// record start time
			chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
			// perform  minimax search
			minimaxDecision(gameBoard, maxPlyDepth, x, y);
			// record finish time
			chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
			// compute delta
			chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
			cout << "MiniMax search completed in " << time_span.count() << " seconds." << endl;
			// record sample
			timeSamples.push_back(time_span.count());
		}
		else
		{
			gameBoard.getRandomMove(x, y);
		}
		if (gameBoard.validMove(x, y) || (x == -1))
		{
			cout << "Moving to " << x << " " << y << endl;
			// Use -1 if no move possible
			if (x != -1)
				gameBoard.makeMove(x, y);
			gameBoard.setCurrentPlayer(gameBoard.getOpponentPiece());
		}
		else
		{
			cout << "Invalid move.  Enter move again. " << endl;
		}
	}
	cout << endl << "The game is over!" << endl;
	gameBoard.display();
	cout << "X's score: " << gameBoard.score('X') << endl;
	cout << "O's score: " << gameBoard.score('O') << endl;

	// Compute and print average time sample
	double avgSample = 0;
	for (int i = 0; i < timeSamples.size(); i++)
	{
		avgSample += timeSamples[i];
	}
	avgSample = avgSample/static_cast<double>(timeSamples.size());
	cout << "Avg search time = " << avgSample << " seconds." << endl;
}
