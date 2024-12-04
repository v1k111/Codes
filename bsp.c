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
    char symbols[] = {'@', '*', '^', '+', '>', '<', '&'};
    const int symbol_count = columns - 2;
    symbols[symbol_count] = '\0';
    int filled_columns = columns - 2;
    int used_columns[columns];
    for (int j = 0; j < filled_columns; j++)
    {
        used_columns[j] = 0;
    }
    int s1count = 0;
    int s2count = 0;
    int s3count = 0;
    int s4count = 0;
    int s5count = 0;
    int s6count = 0;
    int s7count = 0;
    srand(time(NULL));
    for (int i = 0; i < filled_columns;)
    {
        int c = rand() % columns;
        if (!used_columns[c])
        {
            used_columns[c] = 1;
            for (int v = 0; v < rows; ++v)
            {
                char randsymbol = symbols[rand() % symbol_count];
                if (symbol_count > 0 && randsymbol == '@' && s1count < rows)
                {
                    field[v][c] = randsymbol;
                    s1count++;
                }
                else if (symbol_count > 1 && randsymbol == '*' && s2count < rows)
                {
                    field[v][c] = randsymbol;
                    s2count++;
                }
                else if (symbol_count > 2 && randsymbol == '^' && s3count < rows)
                {
                    field[v][c] = randsymbol;
                    s3count++;
                }
                else if (symbol_count > 3 && randsymbol == '+' && s4count < rows)
                {
                    field[v][c] = randsymbol;
                    s4count++;
                }
                else if (symbol_count > 4 && randsymbol == '>' && s5count < rows)
                {
                    field[v][c] = randsymbol;
                    s5count++;
                }
                else if (symbol_count > 5 && randsymbol == '<' && s6count < rows)
                {
                    field[v][c] = randsymbol;
                    s6count++;
                }
                else if (symbol_count > 6 && randsymbol == '&' && s7count < rows)
                {
                    field[v][c] = randsymbol;
                    s7count++;
                }
                else if(symbol_count < 1)
                {
                    return;
                }
                else
                {
                    --v;
                }
            }
            i++;
        }
    }
}
void game_field(const int rows, const int columns, char field[rows][columns])
{
    clear();
    start_color();

    for (int i = 0; i < rows; ++i)
    {
        printw("%d ", i + 1);

        for (int j = 0; j < columns; ++j)
        {
            printw("|");
            
            printw(" %c ", field[i][j]);
        }
        printw("|\n");
    }
    printw("   ");
    for (int i = 0; i < columns; i++)
    {
        printw("--- ");
    }
    printw("\n");
    printw("   ");
    for (int i = 0; i < columns; i++)
    {
        printw(" %d  ", i + 1);
    }
    printw("\n");
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
    const int columns = 9;
    char field[rows][columns];
    generator(rows, columns, field);
    initscr();
    noecho();
    cbreak();
    while (!check(rows, columns, field))
    {
        game_field(rows, columns, field);
        refresh();
        int x = -1;
        int y = -1;
        echo();
        printw("Enter what: ");
        scanw("%d", &x);
        printw("Enter where: ");
        scanw("%d", &y);
        noecho();
        down_possible(rows, columns, field, x, y);
        refresh();
        napms(1000);
    }
    game_field(rows, columns, field);
    printw("You won!");
    refresh();
    getch();
    endwin();
}
int main()
{
    ball_sort_puzzle();
    return 0;
}
