#include <iostream>
#include "AlphaBeta.h"

int main()
{
    setlocale(LC_ALL, "");
    ConnectFourGame game;

    std::cout << "\t\t\t\t\t\tИгра «Четыре в ряд» \n" << std::endl;

    int difficult = 3;
    std::cout << "Введите сложность игры(1-10): "; std::cin >> difficult;
    while (difficult <= 0 || difficult > 10)
    {
        std::cout << "Неверный ввод! Попробуйте ввести сложность снова: "; std::cin >> difficult;
    }

    // Computer makes the first move
    Move bestMove = game.FindBestMove(difficult + 4);
    game.SetBoard(bestMove.row, bestMove.column, PLAYER1);

    // Check if computer wins immediately
    if (game.isWinningMove(PLAYER1))
    {
        game.PrintBoard();
        std::cout << "Компьютер выиграл с первого хода!" << std::endl;
        return 0; // End the game if computer wins
    }

    std::cout << std::endl;

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

        // Computer's turn
        game.PrintBoard();
        bestMove = game.FindBestMove(difficult + 4);
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