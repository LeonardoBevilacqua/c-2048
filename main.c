#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define GRID_SIZE 4

static struct termios orig_opts;
static int is_raw_mode = 0;

void init_term()
{
    printf("\e[?1049h"); // Enable alternate buffer
    printf("\e[2J");     // Clean the screen
    printf("\e[H");      // Move the cursor home
}

void clear_term()
{
    printf("\e[2J");     // Clean the screen
    printf("\e[H");      // Move the cursor home
}

void reset_term()
{
    clear_term();
    printf("\e[?1049l"); // Disable alternate buffer
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_opts);
    struct termios raw_opts = orig_opts;
    // Disable canonical mode (line buffering) and local echo
    // Note: We keep ISIG active so Ctrl+C still fires the SIGINT signal
    raw_opts.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &raw_opts);
    is_raw_mode = 1;
}

void disable_raw_mode() {
    if (!is_raw_mode) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_opts);
    is_raw_mode = 0;
}

void handle_sigint(int signal)
{
    reset_term();
    disable_raw_mode();
    exit(signal);
}

void print_tile(int tile_value)
{
    if (tile_value == 0) printf("|    ");
    else printf("|%4d", tile_value);
}

void print_board(int grid[GRID_SIZE][GRID_SIZE])
{
    printf("---------------------\n");
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j)
            print_tile(grid[i][j]);
        printf("|\n");
    }
    printf("---------------------\n");
}

int assign_to_empty_rand_position(int grid[GRID_SIZE][GRID_SIZE])
{
    int value = rand() % 100 < 90 ? 2 : 4;
    int row = rand() % GRID_SIZE;
    int col = rand() % GRID_SIZE;

    while (grid[row][col] != 0) {
        row = rand() % GRID_SIZE;
        col = rand() % GRID_SIZE;
    }

    grid[row][col] = value;
}

int is_out_of_bounds(int tile)
{
    return tile < 0 || tile >= GRID_SIZE;
}

void move_to_empty_tile(
    int grid[GRID_SIZE][GRID_SIZE],
    int row, int col,
    int row_offset, int col_offset)
{
    int neighbor_row = row + row_offset;
    int neighbor_col = col + col_offset;

    if (is_out_of_bounds(row) || is_out_of_bounds(col) ||
        is_out_of_bounds(neighbor_row) || is_out_of_bounds(neighbor_col)) return;
    if (grid[row][col] == 0) return;

    if (grid[neighbor_row][neighbor_col] > 0 && grid[neighbor_row][neighbor_col] != grid[row][col])
        move_to_empty_tile(grid, neighbor_row, neighbor_col, row_offset, col_offset);

    if (grid[neighbor_row][neighbor_col] == 0) {
        grid[neighbor_row][neighbor_col] = grid[row][col];
        grid[row][col] = 0;
        return;
    }

    if (grid[neighbor_row][neighbor_col] == grid[row][col]) {
        grid[neighbor_row][neighbor_col] += grid[row][col];
        grid[row][col] = 0;
        return;
    }
}

void move_right(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE - 1; ++j)
            move_to_empty_tile(grid, i, j, 0, 1);
    }
}

void move_left(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = GRID_SIZE - 1; j > 0; --j)
            move_to_empty_tile(grid, i, j, 0, -1);
    }
}

void move_up(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int j = 0; j < GRID_SIZE; ++j) {
    for (int i = GRID_SIZE - 1; i > 0; --i)
            move_to_empty_tile(grid, i, j, -1, 0);
    }
}

void move_down(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int j = 0; j < GRID_SIZE; ++j) {
        for (int i = 0; i < GRID_SIZE - 1; ++i)
            move_to_empty_tile(grid, i, j, 1, 0);
    }
}

int main(int argc, char *argv[])
{
    int grid[GRID_SIZE][GRID_SIZE] = { { 0 } };

    init_term();
    signal(SIGINT, handle_sigint);
    enable_raw_mode();
    srand(time(NULL));

    assign_to_empty_rand_position(grid);

    while (1) {
        print_board(grid);
        char key = getchar();
        switch (key) {
            case 'a':
            move_left(grid);
            break;
            case 'd':
            move_right(grid);
            break;
            case 'w':
            move_up(grid);
            break;
            case 's':
            move_down(grid);
            break;
        }
        assign_to_empty_rand_position(grid);
        clear_term();
    }

    disable_raw_mode();
    reset_term();

    return 0;
}
