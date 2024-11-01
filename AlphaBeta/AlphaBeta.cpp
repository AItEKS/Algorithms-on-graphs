#include "AlphaBeta.h"


// NOTE: игра идёт сверху вниз - надо переделать либо логику проверки, либо отображение для пользователя 
bool ConnectFourGame::isWinningMove(Player player)
{
	// Проверка на выигрышные комбинации: по вертикали, горизонтали, диагонали
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (board[row][col] == player)
			{
				if (col + 3 < COLS &&
					board[row][col + 1] == player &&
					board[row][col + 2] == player &&
					board[row][col + 3] == player) return true;

				if (row + 3 < ROWS &&
					board[row + 1][col] == player &&
					board[row + 2][col] == player &&
					board[row + 3][col] == player) return true;

				if (row + 3 < ROWS && col + 3 < COLS &&
					board[row + 1][col + 1] == player &&
					board[row + 2][col + 2] == player &&
					board[row + 3][col + 3] == player) return true;

				if (row + 3 < ROWS && col - 3 > 0 &&
					board[row + 1][col - 1] == player &&
					board[row + 2][col - 2] == player &&
					board[row + 3][col - 3] == player) return true;
			}
		}
	}
	return false;
}

int ConnectFourGame::evaluteBoard(Player player)
{
	int score = 0;
	Player opponet = (player == PLAYER1) ? PLAYER2 : PLAYER1;
	
	auto countScore = [&](int count) {
		switch (count) 
		{
			case 4: return 10000;          // 4 в ряд
			case 3: return 500;            // 3 в ряд
			case 2: return 10;             // 2 в ряд
			case 1: return 1;              // 1 фишка
			default: return 0;             // ничего не даем
		}
	};

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (board[row][col] == NONE) continue;

			if (col + 3 < COLS)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row][col + i] == player) countPlayer++;
					else if (board[row][col + i] == opponet) countOpponent++;
				}
				score += (countScore(countPlayer) - countScore(countOpponent));
			}

			if (row + 3 < ROWS)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row + i][col] == player) countPlayer++;
					else if (board[row + i][col] == opponet) countOpponent++;
				}
				score += (countScore(countPlayer) - countScore(countOpponent));
			}

			if (row + 3 < ROWS && col + 3 < COLS)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row + i][col + i] == player) countPlayer++;
					else if (board[row + i][col + i] == opponet) countOpponent++;
				}
				score += (countScore(countPlayer) - countScore(countOpponent));
			}

			if (row + 3 < ROWS && col - 3 >= 0)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row + i][col - i] == player) countPlayer++;
					else if (board[row + i][col - i] == opponet) countOpponent++;
				}
				score += (countScore(countPlayer) - countScore(countOpponent));
			}
		}
	}
	return score;
}

std::vector<Move> ConnectFourGame::GetAvailableMoves()
{
	std::vector<Move> moves;
	for (int col = 0; col < COLS; col++)
	{
		if (board[0][col] == NONE)
		{
			moves.push_back({ col, GetAvailableRow(col) });
		}
	}
	return moves;
}

int ConnectFourGame::GetAvailableRow(int column)
{
	for (int row = ROWS - 1; row >= 0; --row)
	{
		if (board[row][column] == NONE) return row;
	}
	return -1;
}

int ConnectFourGame::AlphaBeta(int depth, int alpha, int beta, Player currentPlayer)
{
	if (depth == 0 || isWinningMove(PLAYER1) || isWinningMove(PLAYER2)) return evaluteBoard(currentPlayer);

	if (currentPlayer == PLAYER1)
	{
		int maxEvalute = std::numeric_limits<int>::min();
		for (const auto& move : GetAvailableMoves())
		{
			board[move.row][move.column] = PLAYER1;
			int eval = AlphaBeta(depth - 1, alpha, beta, PLAYER2);
			board[move.row][move.column] = NONE;
			maxEvalute = std::max(maxEvalute, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) break; // Отсечение
		}
		return maxEvalute;
	}

	else
	{
		int minEvalute = std::numeric_limits<int>::max();
		for (const auto& move : GetAvailableMoves())
		{
			board[move.row][move.column] = PLAYER2;
			int eval = AlphaBeta(depth - 1, alpha, beta, PLAYER1);
			board[move.row][move.column] = NONE;
			minEvalute = std::min(minEvalute, eval);
			beta = std::max(beta, eval);
			if (beta <= alpha) break; // Отсечение
		}
		return minEvalute;
	}
}

Move ConnectFourGame::FindBestMove(int depth)
{
	Move bestMove{ -1, -1 };
	int bestValue = std::numeric_limits<int>::min();
	for (const auto& move : GetAvailableMoves())
	{
		board[move.row][move.column] = PLAYER1;
		int moveValue = AlphaBeta(depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), PLAYER2);
		board[move.row][move.column] = NONE;

		if (bestValue < moveValue)
		{
			bestValue = moveValue;
			bestMove = move;
		}
	}
	return bestMove;
}

void ConnectFourGame::PrintBoard() const
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++) 
		{
			char c = ' ';
			switch (board[row][col]) 
			{
				case PLAYER1: c = 'X'; break;
				case PLAYER2: c = 'O'; break;
				default: c = ' ';
			}
			std::cout << '|' << c;
		}
		std::cout << '|' << std::endl;
	}
	std::cout << "---------------" << std::endl;
	std::cout << "Столбцы: ";
	for (int col = 1; col <= COLS; col++)
	{
		std::cout << col << " ";
	}
	std::cout << std::endl;
}