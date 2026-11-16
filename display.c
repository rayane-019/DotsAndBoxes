#include <stdio.h>
#include "display.h"

void display_separator(void)
{
    printf("***************************************************************"
           "***************\n");
}

void display_board(const Board *b)
{
    int r, c;

    /* Column numbers */
    printf("  ");
    for (c = 0; c < DOT_COLS; c++)
        printf("%d ", c);
    printf("\n");

    for (r = 0; r < DOT_ROWS; r++) {
        /* Row of dots with horizontal lines */
        printf("%d ", r);
        for (c = 0; c < DOT_COLS; c++) {
            printf(".");
            if (c < BOX_COLS)
                printf("%c", b->horiz[r][c] ? '-' : ' ');
        }
        printf("\n");

        /* Row of vertical lines with box owners in between */
        if (r < BOX_ROWS) {
            printf("  ");
            for (c = 0; c < DOT_COLS; c++) {
                printf("%c", b->vert[r][c] ? '|' : ' ');
                if (c < BOX_COLS)
                    printf("%c", b->owner[r][c]);
            }
            printf("\n");
        }
    }
}

void display_scores(const Board *b)
{
    display_separator();
    printf("Player A score: %d\n", board_count(b, 'A'));
    printf("Player B score: %d\n", board_count(b, 'B'));
    display_separator();
}

void display_winner(const Board *b)
{
    int a = board_count(b, 'A');
    int bb = board_count(b, 'B');

    printf("\nGame over! Final score: A = %d, B = %d\n", a, bb);
    if (a > bb)
        printf("Player A wins!\n");
    else if (bb > a)
        printf("Player B wins!\n");
    else
        printf("It's a draw!\n");
}
