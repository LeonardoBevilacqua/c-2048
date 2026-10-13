#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 4

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
    srand(time(NULL));
    int grid[GRID_SIZE][GRID_SIZE] = { { 0 } };

    assign_to_empty_rand_position(grid);
    assign_to_empty_rand_position(grid);
    print_board(grid);
    move_left(grid);
    print_board(grid);
    move_down(grid);
    print_board(grid);
    move_right(grid);
    print_board(grid);
    move_up(grid);
    print_board(grid);

    return 0;
}
