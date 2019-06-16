#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <ratio>
#include <chrono>
#include <vector>
#include "othello.hpp"

// #define PRINT_BOARD
#define NUM_GAMES 100
#define MAX_LEVEL 1

using namespace std;
using namespace std::chrono;

// Main game loop
int main()
{
	srand(time(NULL));

	vector<double> timeSamples;

        for (int i = 0; i < NUM_GAMES; i++) 
	{
		othello_t gameBoard;
		othello_init(&gameBoard);
		player_t curPlayer = PLAYER_BLACK;
		while (!othello_game_over(&gameBoard))
		{
#ifdef PRINT_BOARD
			othello_display(&gameBoard);
			cout << "It is player " << curPlayer << "'s turn." << endl;
			cout << "Enter move." << endl;
#endif
			int x, y;
			if (othello_has_valid_move(&gameBoard, curPlayer))
			{
				if (curPlayer == PLAYER_WHITE) // Change comments depending on who to play
				{
					// record start time
					high_resolution_clock::time_point t1 = high_resolution_clock::now();
					// perform minimax search
					othello_compute_move(&gameBoard, curPlayer, &x, &y, MAX_LEVEL);
//					othello_compute_random_move(&gameBoard, curPlayer, &x, &y);
					// record finish time
					high_resolution_clock::time_point t2 = high_resolution_clock::now();
					// compute delta
					auto time_span = duration_cast<nanoseconds>(t2 - t1);
#ifdef PRINT_BOARD
					cout << "MiniMax search completed in " << time_span.count() << " nanoseconds." << endl;
#endif
					// record sample
					timeSamples.push_back(time_span.count());
				}
				else
				{
					othello_compute_random_move(&gameBoard, curPlayer, &x, &y);
				}
			}
			bool validMove = othello_is_valid_move(&gameBoard, curPlayer, x, y);
			if (validMove || !othello_has_valid_move(&gameBoard, curPlayer))
			{
//				cout << "Moving to " << x << " " << y << endl;
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
#ifdef PRINT_BOARD
		cout << endl << "The game is over!" << endl;
		othello_display(&gameBoard);
		cout << PLAYER_BLACK << "'s score: " << othello_score(&gameBoard, PLAYER_BLACK) << endl;
		cout << PLAYER_WHITE <<"'s score: " << othello_score(&gameBoard, PLAYER_WHITE) << endl;
#endif
	}

	ofstream outFile;
	outFile.open ("search-duration.plotme");
	// Compute and print average time sample
	double avgSample = 0;
	for (int i = 0; i < timeSamples.size(); i++)
	{
		avgSample += timeSamples[i];
		outFile << timeSamples[i] << "\n";
	}
	outFile.close();
	avgSample = avgSample/static_cast<double>(timeSamples.size());
	cout << "Avg search time = " << avgSample << " nanoseconds." << endl;
}
