#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
void generator(const int rows, const int columns, char field[rows][columns])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            field[i][j] = ' ';
        }
    }
    const char symbols[] = {'@', '*', '^', '+'};
    const int symbol_count = sizeof(symbols) / sizeof(symbols[0]);
    int filled_columns = columns - 2;
    int used_columns[columns];
    for (int j = 0; j < filled_columns; j++)
    {
        used_columns[j] = 0;
    }
    srand(time(NULL));
    for (int i = 0; i < filled_columns;)
    {
        int c = rand() % columns;
        if (!used_columns[c])
        {
            used_columns[c] = 1;
            for (int v = 0; v < rows; ++v)
            {
                field[v][c] = symbols[rand() % symbol_count];
            }
            i++;
        }
    }
}
void print_field(const int rows, const int columns, char field[rows][columns])
{
    clear();
    start_color();

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            printf("| %c ", field[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}
void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y)
{
    x--;
    y--;

    if (x == y)
    {
        printf("Cannot move to the same column.\n");
        return;
    }

    if (x < 0 || x >= columns || y < 0 || y >= columns)
    {
        printf("Invalid column indixes.\n");
        return;
    }
    int from_row = -1;
    for (int i = 0; i < rows; i++)
    {
        if (field[i][x] != ' ')
        {
            from_row = i;
            break;
        }
    }

    if (from_row == -1)
    {
        printf("Source column is empty.\n");
        return;
    }
    int to_row = -1;
    for (int i = rows - 1; i >= 0; --i)
    {
        if (field[i][y] != ' ')
        {
            if (field[i][y] != field[from_row][x])
            {
                printf("Symbols are not the same.\n");
                return;
            }
            else
            {
                to_row = i - 1;
                break;
            }
        }
        else
        {
            continue;
        }
    }

    if (to_row == -1)
    {
        to_row = rows - 1;
    }
    if (to_row >= rows)
    {
        printf("Column is full.\n");
        return;
    }
    field[to_row][y] = field[from_row][x];
    field[from_row][x] = ' ';
}
bool check(const int rows, const int columns, char field[rows][columns])
{
    for (int j = 0; j < columns; j++)
    {
        char current_symbol = ' ';
        for (int i = 0; i < rows; i++)
        {
            if (field[i][j] != ' ')
            {
                if (current_symbol == ' ')
                {
                    current_symbol = field[i][j];
                }
                else if (field[i][j] != current_symbol)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
void ball_sort_puzzle()
{
    const int rows = 4;
    const int columns = 6;
    char field[rows][columns];
    generator(rows, columns, field);
    while (!check(rows, columns, field))
    {
        print_field(rows, columns, field);
        int x = -1;
        int y = -1;
        printf("Enter what: ");
        scanf("%d", &x);
        printf("Enter where: ");
        scanf("%d", &y);
        down_possible(rows, columns, field, x, y);
    }
    printf("You won!");
    return;
}
int main()
{
    ball_sort_puzzle();

    return 0;
}
