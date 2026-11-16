#include "board.h"

void board_init(Board *b)
{
    int r, c;
    for (r = 0; r < DOT_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            b->horiz[r][c] = 0;
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < DOT_COLS; c++)
            b->vert[r][c] = 0;
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            b->owner[r][c] = EMPTY;
    b->boxes_claimed = 0;
}

static int in_range(int r, int c)
{
    return r >= 0 && r < DOT_ROWS && c >= 0 && c < DOT_COLS;
}

/* A box is complete when its 4 sides are drawn. O(1). */
static int box_complete(const Board *b, int r, int c)
{
    return b->horiz[r][c] && b->horiz[r + 1][c] &&
           b->vert[r][c] && b->vert[r][c + 1];
}

/* Claims box (r,c) for p if it exists, is unowned and complete. */
static int try_claim(Board *b, int r, int c, char p)
{
    if (r < 0 || r >= BOX_ROWS || c < 0 || c >= BOX_COLS)
        return 0;
    if (b->owner[r][c] != EMPTY || !box_complete(b, r, c))
        return 0;
    b->owner[r][c] = p;
    b->boxes_claimed++;
    return 1;
}

int board_play_move(Board *b, int r1, int c1, int r2, int c2, char p)
{
    int tmp, done = 0;

    if (!in_range(r1, c1) || !in_range(r2, c2))
        return MOVE_OUT_OF_RANGE;

    /* Order the two dots so (r1,c1) is the top/left one. */
    if (r2 < r1 || (r2 == r1 && c2 < c1)) {
        tmp = r1; r1 = r2; r2 = tmp;
        tmp = c1; c1 = c2; c2 = tmp;
    }

    if (r1 == r2 && c2 == c1 + 1) {            /* horizontal line */
        if (b->horiz[r1][c1])
            return MOVE_ALREADY_TAKEN;
        b->horiz[r1][c1] = 1;
        done += try_claim(b, r1 - 1, c1, p);  /* box above */
        done += try_claim(b, r1, c1, p);      /* box below */
    } else if (c1 == c2 && r2 == r1 + 1) {     /* vertical line */
        if (b->vert[r1][c1])
            return MOVE_ALREADY_TAKEN;
        b->vert[r1][c1] = 1;
        done += try_claim(b, r1, c1 - 1, p);  /* box to the left */
        done += try_claim(b, r1, c1, p);      /* box to the right */
    } else {
        return MOVE_NOT_ADJACENT;
    }
    return done;
}

int board_is_full(const Board *b)
{
    return b->boxes_claimed == TOTAL_BOXES;
}

int board_count(const Board *b, char p)
{
    int r, c, n = 0;
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            if (b->owner[r][c] == p)
                n++;
    return n;
}
