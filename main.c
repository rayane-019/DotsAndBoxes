#include <stdio.h>
#include <string.h>
#include "game.h"

int main(void)
{
    char line[16];

    printf("=== Dots and Boxes ===\n");
    printf("Two players (A and B) take turns joining two neighbouring dots.\n");
    printf("Complete a box to claim it and play again. Most boxes wins.\n");

    for (;;) {
        if (game_run_two_player() == -1)
            return 0;                   /* input closed: exit cleanly */

        printf("\nPlay again? (y/n): ");
        fflush(stdout);
        if (fgets(line, sizeof line, stdin) == NULL)
            return 0;
        if (line[0] != 'y' && line[0] != 'Y')
            break;
    }

    printf("Thanks for playing!\n");
    return 0;
}
