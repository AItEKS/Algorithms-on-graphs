#pragma once

#include <iostream>
#include <vector>
#include <limits>


enum Player { NONE, PLAYER1, PLAYER2 };

const int ROWS = 6;
const int COLS = 7;

struct Move 
{
	int column;
	int row;
};

class ConnectFourGame
{
private:
	std::vector<std::vector<Player>> board;

public:
	ConnectFourGame() : board(ROWS, std::vector<Player>(COLS, NONE)) {}

	const std::vector<std::vector<Player>>& GetBoard() const { return board; }
	void SetBoard(int row, int col, Player player) { board[row][col] = player; }
	bool isWinningMove(Player player);
	int evaluteBoard(Player player);
	std::vector<Move> GetAvailableMoves();
	int GetAvailableRow(int column);
	int AlphaBeta(int depth, int alpha, int beta, Player currentPlayer);
	Move FindBestMove(int depth);

	void PrintBoard() const;
};