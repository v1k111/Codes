#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

void initialize_board(int rows, int cols, char board[rows][cols])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            board[i][j] = '.';
        }
    }
}

int is_valid_move(int rows, int cols, const char board[rows][cols], int col)
{
    int valid = 1;
    if (board[0][col] == '.')
    {
        return valid;
    }
    else if (board[0][col] == 'X' || board[0][col] == 'O')
    {
        valid = 0;
        return valid;
    }
    return valid;
}
