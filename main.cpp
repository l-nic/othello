#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <ratio>
#include <chrono>
#include <vector>
#include "othello.hpp"

using namespace std;

// Main game loop
int main()
{
	srand(time(NULL));
	othello_t gameBoard;
	othello_init(&gameBoard);
	player_t curPlayer = PLAYER_BLACK;

	int maxPlyDepth = 1;
	vector<double> timeSamples;

	while (!othello_game_over(&gameBoard))
	{
		othello_display(&gameBoard);
		cout << "It is player " << curPlayer << "'s turn." << endl;
		cout << "Enter move." << endl;
		int x, y;
		if (curPlayer == PLAYER_WHITE) // Change comments depending on who to play
		{
			// record start time
			chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
			// perform minimax search
			othello_compute_move(&gameBoard, curPlayer, &x, &y);
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
			if (othello_has_valid_move(&gameBoard, curPlayer))
				othello_compute_random_move(&gameBoard, curPlayer, &x, &y);
		}
		bool validMove = othello_is_valid_move(&gameBoard, curPlayer, x, y);
		if (validMove || !othello_has_valid_move(&gameBoard, curPlayer))
		{
			cout << "Moving to " << x << " " << y << endl;
			// Use -1 if no move possible
			if (validMove)
				othello_make_move(&gameBoard, curPlayer, x, y);
			// swap players
			if (curPlayer == PLAYER_BLACK)
				curPlayer = PLAYER_WHITE;
			else
				curPlayer = PLAYER_BLACK;
		}
		else
		{
			cout << "Invalid move.  Enter move again. " << endl;
		}
	}
	cout << endl << "The game is over!" << endl;
	othello_display(&gameBoard);
	cout << PLAYER_BLACK << "'s score: " << othello_score(&gameBoard, PLAYER_BLACK) << endl;
	cout << PLAYER_WHITE <<"'s score: " << othello_score(&gameBoard, PLAYER_WHITE) << endl;

	// Compute and print average time sample
	double avgSample = 0;
	for (int i = 0; i < timeSamples.size(); i++)
	{
		avgSample += timeSamples[i];
	}
	avgSample = avgSample/static_cast<double>(timeSamples.size());
	cout << "Avg search time = " << avgSample << " seconds." << endl;
}
