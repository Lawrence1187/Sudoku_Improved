#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <math.h>
#include <algorithm>
#include <random>
#include <array>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int EMPTY = 0;
const int KYU = 9;
std::array<int, 9> integers{1, 2, 3, 4, 5, 6, 7, 8, 9};
int solutionCount = 1;

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = {(SHORT)x, (SHORT)y};
	SetConsoleCursorPosition(hOut, coord);
}

void sleep(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void copyBoard(int board[KYU][KYU], int(&solutionBoard)[KYU][KYU]) {
	for (int row = 0; row < KYU; row++) {
		for (int col = 0; col < KYU; col++) {
			solutionBoard[col][row] = board[col][row];
		}
	}
}

void displayBoard(int board[KYU][KYU], bool clear = true) {
	if (clear) {
		setCursorPosition(0, 0);
	}
	cout << " _______";
	for (int k = 0; k < 8; k++) {
		cout << "________";
	}
	cout << "____";
	cout << endl;
	for (int j = 0; j < 27; j++) {
		if ((j + 1) % 3 == 0) {
			cout << "|_______|";
			for (int k = 0; k < 8; k++) {
				if ((k + 2) % 3 == 0 && k < 7) {
					cout << "_______|_|";
				}
				else {
					cout << "_______|";
				}
				
			}
			if ((j + 1) % 9 == 0 && j < 26) {
				cout << endl;
				cout << "|_______|";
				for (int k = 0; k < 8; k++) {
					if ((k + 2) % 3 == 0 && k < 7) {
						cout << "_______|_|";
					}
					else {
						cout << "_______|";
					}
				}
			}
			cout << endl;
		}
		else if ((j - 1) % 3 == 0) {
			cout << "|   ";
			for (int i = 0; i < 9; i++) {
				if ((i + 1) % 3 == 0 && i < 8) {
					string value = board[j / 3][i] == EMPTY ? " " : std::to_string(board[j / 3][i]);
					cout << value << "   | |   ";
				}
				else {
					string value = board[j / 3][i] == EMPTY ? " " : std::to_string(board[j / 3][i]);
					cout << value << "   |   ";
				}
			}
			cout << endl;
		}
		else {
			cout << "|       |";
			for (int m = 0; m < 8; m++) {
				if ((m + 2) % 3 == 0 && m < 7) {
					cout << "       | |";
				}
				else {
					cout << "       |";
				}
			}
			cout << endl;
		}
	}
}

bool isValid(int board[KYU][KYU], int row, int col, int value) {
	for (int rowIt = 0; rowIt < KYU; rowIt++) {
		if (board[col][rowIt] == value) {
			return false;
		}
	}
	for (int colIt = 0; colIt < KYU; colIt++) {
		if (board[colIt][row] == value) {
			return false;
		}
	}
	int rowBegin = (row / 3) * 3;
	int colBegin = (col / 3) * 3;
	for (int r = rowBegin; r < rowBegin + 3; r++) {
		for (int c = colBegin; c < colBegin + 3; c++) {
			if (board[c][r] == value) {
				return false;
			}
		}
	}
	return true;
}

bool boardIsIncomplete(int board[KYU][KYU]) {
	for (int r = 0; r < KYU; r++) {
		for (int c = 0; c < KYU; c++) {
			if (board[c][r] == EMPTY) {
				return true;
			}
		}
	}
	return false;
}

bool fillBoard(int (&board)[KYU][KYU]) {
	int row, col;
	for (int i = 0; i < KYU * KYU; i++) {
		row = i / KYU;
		col = i % KYU;
		if (board[col][row] == EMPTY) {
			//shuffle the integers array so that we always generate a different board
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::shuffle(integers.begin(), integers.end(), std::default_random_engine(seed));

			for (int currInt : integers) {
				if (isValid(board, row, col, currInt)) {
					board[col][row] = currInt;
					if (!boardIsIncomplete(board) || fillBoard(board)) { //recursion happens here
						return true;
					}
				}
			}
			break;
		}
	}
	board[col][row] = EMPTY;
	return false;
}

bool solveBoard(int(&board)[KYU][KYU], bool display = false) {
	int row, col;
	for (int i = 0; i < KYU * KYU; i++) {
		row = i / KYU;
		col = i % KYU;

		if (board[col][row] == EMPTY) {
			for (int integer = 1; integer <= KYU; integer++) {
				if (isValid(board, row, col, integer)) {
					board[col][row] = integer;
					if (display) {
						sleep(100);
						displayBoard(board);
					}
					if (!boardIsIncomplete(board)) {
						solutionCount++;
						if (display) {
							sleep(100);
							displayBoard(board);
							return true; //return statement here because we don't want to erase the progress when the program actually solves it (display == true) for us
						}
						break;
					}
					else if (solveBoard(board, display)) {
						return true;
					}
				}
			}
			break;
		}
	}
	board[col][row] = EMPTY; //here we are erasing the value since we are only checking if a solution exists
	if (display) {
		sleep(100);
		displayBoard(board);
	}
	return false;
}

void generateBoard(int(&board)[KYU][KYU], int difficulty = 1) {
	memset(board, EMPTY, sizeof(board));
	fillBoard(board);
	cout << "Board filled" << endl;

	//Here is the code snippet to remove random values from the board
	srand(time(NULL));
	while (difficulty > 0) {
		int row = rand() % KYU;
		int col = rand() % KYU;
		while (board[col][row] == EMPTY) {
			row = rand() % KYU;
			col = rand() % KYU;
		}
		int hiddenValue = board[col][row];
		board[col][row] = EMPTY;

		solutionCount = 0;
		solveBoard(board); //display is set to false since we don't need to show all the steps when generating a board

		if (solutionCount != 1) {
			board[col][row] = hiddenValue;
			difficulty--;
		}
	}
	
}

int main() {
	int board[KYU][KYU];
	int solutionBoard[KYU][KYU];
	cout << "Can start properly" << endl;
	generateBoard(board, 3);
	cout << "Board generated" << endl;
	copyBoard(board, solutionBoard);
	cout << "Board copied" << endl;
	system("cls"); //this command clears the console, but be careful, it is Windows specific and may not work on other types of consoles
	displayBoard(board);
	string input;
	cout << "Let the machine solve the board? (y/n) ";
	cin >> input;
	if (input == "n" || input == "N") {
		cout.flush(); //good habit to have, makes sure that the user sees any buffered text before the program terminates
		return 0;
	}
	solveBoard(board, true);
	displayBoard(solutionBoard, false);
	cout.flush();
	return 0;
}