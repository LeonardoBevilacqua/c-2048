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

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int grid[GRID_SIZE][GRID_SIZE] = { { 0 } };

    assign_to_empty_rand_position(grid);
    print_board(grid);

    return 0;
}
