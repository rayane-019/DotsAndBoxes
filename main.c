#include <stdio.h>
#include "game.h"

/* Asks the player to choose a mode.
 * Returns 0 for two players, 1 for playing against the bot, -1 if input ended. */
static int ask_mode(void)
{
    char line[16];

    for (;;) {
        printf("\nChoose a mode:\n");
        printf("  1) Two players\n");
        printf("  2) Play against the bot\n");
        printf("Your choice: ");
        fflush(stdout);

        if (fgets(line, sizeof line, stdin) == NULL)
            return -1;
        if (line[0] == '1' && (line[1] == '\n' || line[1] == '\0'))
            return 0;
        if (line[0] == '2' && (line[1] == '\n' || line[1] == '\0'))
            return 1;
        printf("Please type 1 or 2.\n");
    }
}

int main(void)
{
    char line[16];
    int mode;

    printf("=== Dots and Boxes ===\n");
    printf("Two players (A and B) take turns joining two neighbouring dots.\n");
    printf("Complete a box to claim it and play again. Most boxes wins.\n");

    for (;;) {
        mode = ask_mode();
        if (mode == -1)
            return 0;                   /* input closed: exit cleanly */

        if (game_run(mode) == -1)
            return 0;

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
