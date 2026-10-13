#include <stdio.h>

int main(int argc, char *argv[])
{
    int test = 5;
    int grid[4][4];
    printf("---------------------\n");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("|%4d", 5);
        }
        printf("|\n");
    }
    printf("---------------------\n");

    return 0;
}
