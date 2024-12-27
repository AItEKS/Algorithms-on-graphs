#include <iostream>
#include "AlphaBeta.h"

int main()
{
    setlocale(LC_ALL, "");
    ConnectFourGame game;

    std::cout << "\t\t\t\t\t\t���� ������� � ��� \n" << std::endl;

    int difficult = 3;
    std::cout << "������� ��������� ����(1-10): "; std::cin >> difficult;
    while (difficult <= 0 || difficult > 10)
    {
        std::cout << "�������� ����! ���������� ������ ��������� �����: "; std::cin >> difficult;
    }

    // Computer makes the first move
    Move bestMove = game.FindBestMove(difficult + 4);
    game.SetBoard(bestMove.row, bestMove.column, PLAYER1);

    // Check if computer wins immediately
    if (game.isWinningMove(PLAYER1))
    {
        game.PrintBoard();
        std::cout << "��������� ������� � ������� ����!" << std::endl;
        return 0; // End the game if computer wins
    }

    std::cout << std::endl;

    while (true)
    {
        game.PrintBoard();

        int userMove;
        std::cout << "������� ����� ������� ��� ������ ����: "; std::cin >> userMove;

        while (userMove <= 0 || userMove > COLS || game.GetAvailableRow(userMove - 1) == -1)
        {
            std::cout << "�������� ����! ���������� ������ �����: "; std::cin >> userMove;
        }

        userMove--;

        Move userColMove{ userMove, game.GetAvailableRow(userMove) };
        game.SetBoard(userColMove.row, userColMove.column, PLAYER2);

        if (game.isWinningMove(PLAYER2))
        {
            game.PrintBoard();
            std::cout << "����������! �� �������� ���������!" << std::endl;
            break;
        }

        // Computer's turn
        game.PrintBoard();
        bestMove = game.FindBestMove(difficult + 4);
        game.SetBoard(bestMove.row, bestMove.column, PLAYER1);

        if (game.isWinningMove(PLAYER1))
        {
            game.PrintBoard();
            std::cout << "�� ���������!" << std::endl;
            break;
        }
    }

    return 0;
}