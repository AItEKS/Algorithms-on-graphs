#include <iostream>

#include "AlphaBeta.h"


int main()
{
	setlocale(LC_ALL, "");
	ConnectFourGame game;

	while (true)
	{
		game.PrintBoard();

		int userMove;
		std::cout << "Введите номер колонки для вашего хода: "; std::cin >> userMove;

		while (userMove <= 0 || userMove > COLS || game.GetAvailableRow(userMove - 1) == -1)
		{
			std::cout << "Неверный ввод! Попробуйте ввести снова: "; std::cin >> userMove;
		}

		userMove--;

		Move userColMove{ userMove, game.GetAvailableRow(userMove) };
		game.SetBoard(userColMove.row, userColMove.column, PLAYER2);

		if (game.isWinningMove(PLAYER2))
		{
			game.PrintBoard();
			std::cout << "Поздравляю! Вы выиграли компьютер!" << std::endl;
			break;
		}
		
		game.PrintBoard();

		Move bestMove = game.FindBestMove(8);

		game.SetBoard(bestMove.row, bestMove.column, PLAYER1);

		if (game.isWinningMove(PLAYER1))
		{
			game.PrintBoard();
			std::cout << "Вы проиграли!" << std::endl;
			break;
		}
	}

	return 0;
}
