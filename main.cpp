#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <ratio>
#include <chrono>
#include <vector>
#include "othello.hpp"

using namespace std;

// // Function prototypes
// void minimaxDecision(Board board, int maxPlyDepth, int &x, int &y);
// int minimaxValue(Board board, Board::piece_t originalTurn, int searchPly, int maxPlyDepth);

// Main game loop
int main()
{
	srand(time(NULL));
	Board gameBoard;
	gameBoard.setCurrentPlayer(Board::BLACK);

	int maxPlyDepth = 1;
	vector<double> timeSamples;

	while (!gameBoard.gameOver())
	{
		gameBoard.display();
		cout << "It is player " << gameBoard.getWhosePiece() << "'s turn." << endl;
		cout << "Enter move." << endl;
		int x, y;
		if (gameBoard.getWhosePiece() == Board::WHITE) // Change comments depending on who to play
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
	cout << "X's score: " << gameBoard.score(Board::BLACK) << endl;
	cout << "O's score: " << gameBoard.score(Board::WHITE) << endl;

	// Compute and print average time sample
	double avgSample = 0;
	for (int i = 0; i < timeSamples.size(); i++)
	{
		avgSample += timeSamples[i];
	}
	avgSample = avgSample/static_cast<double>(timeSamples.size());
	cout << "Avg search time = " << avgSample << " seconds." << endl;
}
