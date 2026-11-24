#include "bot.h"

/* Placeholder bot: plays the first free line it finds.
 * Horizontal lines first, then vertical lines. */
void bot_choose_move(const Board *b, int *r1, int *c1, int *r2, int *c2)
{
    int r, c;

    /* Horizontal line: dot (r,c) to dot (r,c+1) */
    for (r = 0; r < DOT_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            if (!b->horiz[r][c]) {
                *r1 = r; *c1 = c; *r2 = r; *c2 = c + 1;
                return;
            }

    /* Vertical line: dot (r,c) to dot (r+1,c) */
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < DOT_COLS; c++)
            if (!b->vert[r][c]) {
                *r1 = r; *c1 = c; *r2 = r + 1; *c2 = c;
                return;
            }
}
