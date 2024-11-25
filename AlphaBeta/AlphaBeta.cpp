#include "AlphaBeta.h"


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

				if (row + 3 < ROWS && col - 3 >= 0 &&
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
	Player opponent = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	auto countScore = [&](int count, bool isOpponent) {
		switch (count)
		{
		case 4: return isOpponent ? -10000 : 10000; // 4 в ряд (противник - большой штраф)
		case 3: return isOpponent ? -500 : 500;     // 3 в ряд (противник - штраф)
		case 2: return isOpponent ? -10 : 10;       // 2 в ряд
		case 1: return isOpponent ? -1 : 1;         // 1 фишка
		default: return 0;                          // ничего не даем
		}
		};

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (board[row][col] == NONE) continue;

			// Горизонтальная проверка (-)
			if (col + 3 < COLS)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row][col + i] == player) countPlayer++;
					else if (board[row][col + i] == opponent) countOpponent++;
				}
				score += (countScore(countPlayer, false) + countScore(countOpponent, true));
			}

			// Вертикальная проверка (|)
			if (row + 3 < ROWS)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row + i][col] == player) countPlayer++;
					else if (board[row + i][col] == opponent) countOpponent++;
				}
				score += (countScore(countPlayer, false) + countScore(countOpponent, true));
			}

			// Диагональная проверка (\)
			if (row + 3 < ROWS && col + 3 < COLS)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row + i][col + i] == player) countPlayer++;
					else if (board[row + i][col + i] == opponent) countOpponent++;
				}
				score += (countScore(countPlayer, false) + countScore(countOpponent, true));
			}

			// Диагональная проверка (/)
			if (row + 3 < ROWS && col - 3 >= 0)
			{
				int countPlayer = 0, countOpponent = 0;
				for (int i = 0; i < 4; i++)
				{
					if (board[row + i][col - i] == player) countPlayer++;
					else if (board[row + i][col - i] == opponent) countOpponent++;
				}
				score += (countScore(countPlayer, false) + countScore(countOpponent, true));
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
	if (depth == 0) {
		return evaluteBoard(currentPlayer);
	}

	bool win = isWinningMove(currentPlayer);
	if (win) return (currentPlayer == PLAYER1) ? 10000 : -10000;

	if (currentPlayer == PLAYER1)
	{
		int maxEvaluate = std::numeric_limits<int>::min();
		for (const auto& move : GetAvailableMoves())
		{
			board[move.row][move.column] = PLAYER1;
			int eval = AlphaBeta(depth - 1, alpha, beta, PLAYER2);
			board[move.row][move.column] = NONE;
			maxEvaluate = std::max(maxEvaluate, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) break;
		}
		return maxEvaluate;
	}
	else
	{
		int minEvaluate = std::numeric_limits<int>::max();
		for (const auto& move : GetAvailableMoves())
		{
			board[move.row][move.column] = PLAYER2;
			int eval = AlphaBeta(depth - 1, alpha, beta, PLAYER1);
			board[move.row][move.column] = NONE;
			minEvaluate = std::min(minEvaluate, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha) break;
		}
		return minEvaluate;
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
	std::cout << " ";
	for (int col = 1; col <= COLS; col++)
	{
		std::cout << col << " ";
	}
	std::cout << std::endl;
}
