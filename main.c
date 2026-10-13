#include <stdio.h>

void print_tile(int tile_value)
{
    if (tile_value == 0) printf("|    ");
    else printf("|%4d", tile_value);
}

void print_board(int grid[4][4])
{
    printf("---------------------\n");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j)
            print_tile(grid[i][j]);
        printf("|\n");
    }
    printf("---------------------\n");
}

int main(int argc, char *argv[])
{
    int test = 5;
    int grid[4][4] = { { 0 } };

    print_board(grid);

    return 0;
}
