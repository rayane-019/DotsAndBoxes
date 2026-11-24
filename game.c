#include <stdio.h>
#include "game.h"
#include "board.h"
#include "display.h"
#include "bot.h"

#define LINE_LEN 128

/* Reads one line and parses 4 integers into r1 c1 r2 c2.
 * Returns 1 on success, 0 on bad format, -1 on EOF. */
static int read_move(int *r1, int *c1, int *r2, int *c2)
{
    char line[LINE_LEN];
    char extra;

    if (fgets(line, sizeof line, stdin) == NULL)
        return -1;
    if (sscanf(line, "%d %d %d %d %c", r1, c1, r2, c2, &extra) != 4)
        return 0;
    return 1;
}

static void explain_error(int code)
{
    switch (code) {
    case MOVE_OUT_OF_RANGE:
        printf("Invalid move: rows must be 0-%d and columns 0-%d.\n",
               DOT_ROWS - 1, DOT_COLS - 1);
        break;
    case MOVE_NOT_ADJACENT:
        printf("Invalid move: the two dots must be next to each other "
               "(horizontally or vertically).\n");
        break;
    case MOVE_ALREADY_TAKEN:
        printf("Invalid move: that line is already drawn.\n");
        break;
    default:
        printf("Invalid move.\n");
    }
}

int game_run(int vs_bot)
{
    Board board;
    char player = 'A';
    int r1, c1, r2, c2, status, result;
    int bot_turn;

    board_init(&board);
    display_separator();
    display_board(&board);

    while (!board_is_full(&board)) {
        bot_turn = vs_bot && player == 'B';

        if (bot_turn) {
            bot_choose_move(&board, &r1, &c1, &r2, &c2);
            printf("Bot (B) plays: %d %d %d %d\n", r1, c1, r2, c2);
        } else {
            printf("Player %c's turn. Enter the row and column of the first dot "
                   "(e.g., A0 -> 0 0) and second dot: ", player);
            fflush(stdout);

            status = read_move(&r1, &c1, &r2, &c2);
            if (status == -1)
                return -1;
            if (status == 0) {
                printf("Please enter 4 numbers, e.g. 0 0 0 1\n");
                continue;
            }
        }

        result = board_play_move(&board, r1, c1, r2, c2, player);
        if (result < 0) {
            if (bot_turn) {             /* should never happen: bot bug */
                printf("Error: the bot chose an invalid move.\n");
                return 0;
            }
            explain_error(result);
            continue;
        }

        display_scores(&board);
        display_board(&board);

        /* Completing a box gives the same player another turn. */
        if (result == 0)
            player = (player == 'A') ? 'B' : 'A';
    }

    display_winner(&board);
    return 0;
}
