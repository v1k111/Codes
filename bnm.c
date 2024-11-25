#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define BOARD_SIZE 10

void init_ncurses();
void end_ncurses();
void print_board_setup(char board[BOARD_SIZE][BOARD_SIZE], int player, int current_length);
void print_board_game(char left_board[BOARD_SIZE][BOARD_SIZE], char right_board[BOARD_SIZE][BOARD_SIZE], bool player1_turn);
bool can_place_ship(char board[BOARD_SIZE][BOARD_SIZE], int start_row, int start_col, int end_row, int end_col, int expected_length);
void place_ship(char board[BOARD_SIZE][BOARD_SIZE], int start_row, int start_col, int end_row, int end_col);
void mark_killed_ship(char board[BOARD_SIZE][BOARD_SIZE], int row, int col);
void setup_board(char board[BOARD_SIZE][BOARD_SIZE], int ships[], int num_ships, int player);
bool take_shot(char board[BOARD_SIZE][BOARD_SIZE], char shot_board[BOARD_SIZE][BOARD_SIZE], int row, int col);
bool is_ship_killed(char board[BOARD_SIZE][BOARD_SIZE], int row, int col);
bool has_won(char board[BOARD_SIZE][BOARD_SIZE]);
void main_game_loop(char player1_board[BOARD_SIZE][BOARD_SIZE], char player2_board[BOARD_SIZE][BOARD_SIZE], char player1_shots[BOARD_SIZE][BOARD_SIZE], char player2_shots[BOARD_SIZE][BOARD_SIZE]);

void init_ncurses()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
}

void end_ncurses()
{
    endwin();
}

void print_board_setup(char board[BOARD_SIZE][BOARD_SIZE], int player, int current_length)
{
    clear();
    mvprintw(0, 0, "Player %d, place your ships:", player);
    mvprintw(1, 0, "Ship length to place: %d", current_length);
    mvprintw(2, 0, "   A B C D E F G H I J");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        mvprintw(3 + i, 0, "%2d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 'O')
            {
                attron(COLOR_PAIR(4));
                mvprintw(3 + i, 3 + j * 2, "O");
                attroff(COLOR_PAIR(4));
            }
            else
            {
                mvprintw(3 + i, 3 + j * 2, ".");
            }
        }
    }
    refresh();
}
void setup_board(char board[BOARD_SIZE][BOARD_SIZE], int ships[], int num_ships, int player)
{
    for (int i = 0; i < num_ships; i++)
    {
        int length = ships[i];
        while (true)
        {
            print_board_setup(board, player, length);
            refresh();
            if (length == 1)
            {
                mvprintw(14, 0, "Enter position (e.g., A1): ");
                refresh();
                char col_char;
                int row;
                echo();
                scanw(" %c%d", &col_char, &row);
                noecho();
                int col = toupper(col_char) - 'A';
                row--;
                if (can_place_ship(board, row, col, row, col, length))
                {
                    place_ship(board, row, col, row, col);
                    break;
                }
                mvprintw(16, 0, "Invalid placement. Try again.");
                refresh();
                getch();
            }
            else
            {
                mvprintw(14, 0, "Enter start (e.g., A1): ");
                refresh();
                char start_col_char;
                int start_row;
                echo();
                scanw(" %c%d", &start_col_char, &start_row);
                noecho();
                mvprintw(15, 0, "Enter end (e.g., A1): ");
                refresh();
                char end_col_char;
                int end_row;
                echo();
                scanw(" %c%d", &end_col_char, &end_row);
                noecho();
                int start_col = toupper(start_col_char) - 'A';
                int end_col = toupper(end_col_char) - 'A';
                start_row--;
                end_row--;

                if (can_place_ship(board, start_row, start_col, end_row, end_col, length))
                {
                    place_ship(board, start_row, start_col, end_row, end_col);
                    break;
                }
                mvprintw(16, 0, "Invalid placement. Try again.");
                refresh();
                getch();
            }
        }
    }
    clear();
    refresh();
}
void print_board_game(char left_board[BOARD_SIZE][BOARD_SIZE], char right_board[BOARD_SIZE][BOARD_SIZE], bool player1_turn)
{
    clear();
    mvprintw(0, 6, "Player 1");
    mvprintw(0, 46, "Player 2");
    mvprintw(1, 3, "A B C D E F G H I J");
    mvprintw(1, 43, "A B C D E F G H I J");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        mvprintw(2 + i, 0, "%2d ", i + 1);
        mvprintw(2 + i, 40, "%2d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            char left_char = left_board[i][j] == 'O' ? '.' : left_board[i][j];
            char right_char = right_board[i][j] == 'O' ? '.' : right_board[i][j];
            if (right_char == 'X')
            {
                attron(COLOR_PAIR(1));
                mvprintw(2 + i, 3 + j * 2, "%c", right_char);
                attroff(COLOR_PAIR(1));
            }
            else
            {
                mvprintw(2 + i, 3 + j * 2, "%c", right_char);
            }
            if (left_char == 'X')
            {
                attron(COLOR_PAIR(1));
                mvprintw(2 + i, 43 + j * 2, "%c", left_char);
                attroff(COLOR_PAIR(1));
            }
            else
            {
                mvprintw(2 + i, 43 + j * 2, "%c", left_char);
            }
        }
    }

    if (player1_turn)
    {
        attron(COLOR_PAIR(4));
        mvprintw(7, 28, "--->");
        attroff(COLOR_PAIR(4));
    }
    else
    {
        attron(COLOR_PAIR(4));
        mvprintw(7, 28, "<---");
        attroff(COLOR_PAIR(4));
    }
    refresh();
}

void mark_killed_ship(char board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};
    for (int direction = 0; direction < 4; direction++)
    {
        int r = row, c = col;
        while (true)
        {
            r += dr[direction];
            c += dc[direction];

            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != 'X')
                break;
        }
        r = row, c = col;
        while (true)
        {
            r -= dr[direction];
            c -= dc[direction];

            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != 'X')
                break;
        }
    }
    for (int r = row - 1; r <= row + 1; r++)
    {
        for (int c = col - 1; c <= col + 1; c++)
        {
            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == '.')
            {
                board[r][c] = '*';
            }
        }
    }
    for (int direction = 0; direction < 4; direction++)
    {
        int r = row, c = col;

        while (true)
        {
            r += dr[direction];
            c += dc[direction];
            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != 'X')
                break;
            for (int rr = r - 1; rr <= r + 1; rr++)
            {
                for (int cc = c - 1; cc <= c + 1; cc++)
                {
                    if (rr >= 0 && rr < BOARD_SIZE && cc >= 0 && cc < BOARD_SIZE && board[rr][cc] == '.')
                    {
                        board[rr][cc] = '*';
                    }
                }
            }
        }
    }
}

bool is_ship_killed(char board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    for (int d = 0; d < 4; d++)
    {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int nr = row, nc = col;
        while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE)
        {
            if (board[nr][nc] == 'O')
                return false;
            if (board[nr][nc] != 'X')
                break;

            nr += dr;
            nc += dc;
        }
    }

    return true;
}
bool take_shot(char board[BOARD_SIZE][BOARD_SIZE], char shot_board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    if (board[row][col] == 'O')
    {
        board[row][col] = 'X';
        shot_board[row][col] = 'X';
        if (is_ship_killed(board, row, col))
        {
            mark_killed_ship(board, row, col);
            mark_killed_ship(shot_board, row, col);
        }
        return true;
    }
    else if (board[row][col] == '.')
    {
        board[row][col] = '*';
        shot_board[row][col] = '*';
        return false;
    }
    return false;
}

bool has_won(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 'O')
            {
                return false;
            }
        }
    }
    return true;
}
void main_game_loop(char player1_board[BOARD_SIZE][BOARD_SIZE], char player2_board[BOARD_SIZE][BOARD_SIZE], char player1_shots[BOARD_SIZE][BOARD_SIZE], char player2_shots[BOARD_SIZE][BOARD_SIZE])
{
    bool player1_turn = true;

    while (true)
    {
        clear();
        print_board_game(player1_shots, player2_shots, player1_turn);
        mvprintw(14, 0, "Player %d's turn. Enter your shot (e.g., A1):", player1_turn ? 1 : 2);
        refresh();

        char col_char;
        int row;
        echo();
        scanw(" %c%d", &col_char, &row);
        noecho();

        int col = toupper(col_char) - 'A';
        row--;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        {
            bool hit = take_shot(player1_turn ? player2_board : player1_board, player1_turn ? player1_shots : player2_shots, row, col);
            clear();
            print_board_game(player1_shots, player2_shots, player1_turn);

            if (hit)
            {
                if (is_ship_killed(player1_turn ? player2_board : player1_board, row, col))
                {
                    mvprintw(16, 0, "Killed! Player %d continues.", player1_turn ? 1 : 2);
                }
                else
                {
                    mvprintw(16, 0, "Hit! Player %d continues.", player1_turn ? 1 : 2);
                }

                if (has_won(player1_turn ? player2_board : player1_board))
                {
                    mvprintw(18, 0, "Player %d wins!", player1_turn ? 1 : 2);
                    mvprintw(20, 0, "Press any key to exit.");
                    clear();
                    if (player1_turn)
                    {
                        print_board_game(player1_shots, player1_board, player1_turn);
                        for (int i = 0; i < BOARD_SIZE; i++)
                        {
                            for (int j = 0; j < BOARD_SIZE; j++)
                            {
                                if (player1_board[i][j] == 'O')
                                {
                                    attron(COLOR_PAIR(4));
                                    mvprintw(3 + i - 1, 3 + j * 2, "O");
                                    attroff(COLOR_PAIR(4));
                                }
                            }
                        }
                    }
                    else
                    {
                        print_board_game(player2_shots, player2_board, player1_turn);
                        for (int i = 0; i < BOARD_SIZE; i++)
                        {
                            for (int j = 0; j < BOARD_SIZE; j++)
                            {
                                if (player2_board[i][j] == 'O')
                                {
                                    attron(COLOR_PAIR(4));
                                    mvprintw(3 + i, 43 + j * 2, "O");
                                    attroff(COLOR_PAIR(4));
                                }
                            }
                        }
                    }
                    refresh();
                    getch();
                    break;
                }
            }
            else
            {
                mvprintw(16, 0, "Miss! Switching turn.");
                player1_turn = !player1_turn;
            }
        }
        else
        {
            mvprintw(16, 0, "Invalid input. Try again.");
        }
        refresh();
        napms(1000);
    }
}
bool can_place_ship(char board[BOARD_SIZE][BOARD_SIZE], int start_row, int start_col, int end_row, int end_col, int expected_length)
{
    if (start_row > end_row || start_col > end_col || start_row < 0 || start_col < 0 || end_row >= BOARD_SIZE || end_col >= BOARD_SIZE)
        return false;

    bool horizontal = start_row == end_row;
    bool vertical = start_col == end_col;
    if (!horizontal && !vertical)
        return false;

    int length = horizontal ? (end_col - start_col + 1) : (end_row - start_row + 1);
    if (length != expected_length)
        return false;

    for (int i = 0; i < length; i++)
    {
        int r = horizontal ? start_row : (start_row + i);
        int c = horizontal ? (start_col + i) : start_col;
        if (board[r][c] != '.')
            return false;
        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int nr = r + dr, nc = c + dc;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] != '.')
                    return false;
            }
        }
    }
    return true;
}
void place_ship(char board[BOARD_SIZE][BOARD_SIZE], int start_row, int start_col, int end_row, int end_col)
{
    bool horizontal = start_row == end_row;
    int length = horizontal ? (end_col - start_col + 1) : (end_row - start_row + 1);
    for (int i = 0; i < length; i++)
    {
        int r = horizontal ? start_row : (start_row + i);
        int c = horizontal ? (start_col + i) : start_col;
        board[r][c] = 'O';
    }
}
void game()
{
    char board1[BOARD_SIZE][BOARD_SIZE];
    char board2[BOARD_SIZE][BOARD_SIZE];
    char shots1[BOARD_SIZE][BOARD_SIZE];
    char shots2[BOARD_SIZE][BOARD_SIZE];

    memset(board1, '.', sizeof(board1));
    memset(board2, '.', sizeof(board2));
    memset(shots1, '.', sizeof(shots1));
    memset(shots2, '.', sizeof(shots2));

    int ships[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int num_ships = sizeof(ships) / sizeof(ships[0]);

    init_ncurses();

    mvprintw(0, 0, "Player 1, set up your board:");
    setup_board(board1, ships, num_ships, 1);
    mvprintw(0, 0, "Player 2, set up your board:");
    setup_board(board2, ships, num_ships, 2);

    clear();
    main_game_loop(board1, board2, shots1, shots2);

    getch();
    end_ncurses();
}
int main()
{
    game();
    return 0;
}
