#include <stdlib.h>
#include <time.h>
#include "bot.h"

/* Strategy (in this order):
 *   Level 1: if a line closes a box, play it (free points).
 *   Level 2: otherwise play a "safe" line: one that does not give
 *            a box its 3rd side (that would hand the box to the opponent).
 *   Level 3: if no line is safe, play the line that gives away
 *            the FEWEST boxes to the opponent. */

#define MAX_LINES (DOT_ROWS * BOX_COLS + BOX_ROWS * DOT_COLS)   /* 25 + 24 = 49 */

/* One line of the board.
 * is_h = 1: horizontal line horiz[r][c], from dot (r,c) to dot (r,c+1)
 * is_h = 0: vertical line   vert[r][c],  from dot (r,c) to dot (r+1,c) */
typedef struct {
    int is_h, r, c;
} Line;

/* Number of drawn sides (0 to 4) of box (r,c). */
static int sides(const Board *b, int r, int c)
{
    return b->horiz[r][c] + b->horiz[r + 1][c] +
           b->vert[r][c] + b->vert[r][c + 1];
}

/* Writes the boxes touching line l into br[], bc[]. Returns how many (1 or 2). */
static int boxes_of_line(Line l, int br[2], int bc[2])
{
    int n = 0;

    if (l.is_h) {
        if (l.r > 0)        { br[n] = l.r - 1; bc[n] = l.c; n++; }   /* box above */
        if (l.r < BOX_ROWS) { br[n] = l.r;     bc[n] = l.c; n++; }   /* box below */
    } else {
        if (l.c > 0)        { br[n] = l.r; bc[n] = l.c - 1; n++; }   /* box left  */
        if (l.c < BOX_COLS) { br[n] = l.r; bc[n] = l.c;     n++; }   /* box right */
    }
    return n;
}

/* Converts a line into its two dots. */
static void line_to_dots(Line l, int *r1, int *c1, int *r2, int *c2)
{
    *r1 = l.r;
    *c1 = l.c;
    *r2 = l.is_h ? l.r : l.r + 1;
    *c2 = l.is_h ? l.c + 1 : l.c;
}

/* Fills lines[] with every line not drawn yet. Returns how many. */
static int free_lines(const Board *b, Line lines[])
{
    int r, c, n = 0;

    for (r = 0; r < DOT_ROWS; r++)
        for (c = 0; c < BOX_COLS; c++)
            if (!b->horiz[r][c]) {
                lines[n].is_h = 1; lines[n].r = r; lines[n].c = c; n++;
            }
    for (r = 0; r < BOX_ROWS; r++)
        for (c = 0; c < DOT_COLS; c++)
            if (!b->vert[r][c]) {
                lines[n].is_h = 0; lines[n].r = r; lines[n].c = c; n++;
            }
    return n;
}

/* 1 if some box touching l has exactly `count` sides already. */
static int touches_box_with(const Board *b, Line l, int count)
{
    int br[2], bc[2], i, n = boxes_of_line(l, br, bc);

    for (i = 0; i < n; i++)
        if (sides(b, br[i], bc[i]) == count)
            return 1;
    return 0;
}

/* Level 3 helper: how many boxes would the opponent win if the bot played l?
 * Works on a COPY of the board: draws l, then lets the opponent close
 * every 3-sided box, again and again, until none is left. */
static int boxes_given(const Board *b, Line l)
{
    Board copy = *b;
    int given = 0, found, r, c, r1, c1, r2, c2, done;
    Line side;

    line_to_dots(l, &r1, &c1, &r2, &c2);
    board_play_move(&copy, r1, c1, r2, c2, 'B');

    do {
        found = 0;
        for (r = 0; r < BOX_ROWS; r++)
            for (c = 0; c < BOX_COLS; c++) {
                if (copy.owner[r][c] != EMPTY || sides(&copy, r, c) != 3)
                    continue;
                /* Find the missing side of this box and let the opponent draw it. */
                if (!copy.horiz[r][c])          { side.is_h = 1; side.r = r;     side.c = c; }
                else if (!copy.horiz[r + 1][c]) { side.is_h = 1; side.r = r + 1; side.c = c; }
                else if (!copy.vert[r][c])      { side.is_h = 0; side.r = r;     side.c = c; }
                else                            { side.is_h = 0; side.r = r;     side.c = c + 1; }
                line_to_dots(side, &r1, &c1, &r2, &c2);
                done = board_play_move(&copy, r1, c1, r2, c2, 'A');
                if (done > 0) {
                    given += done;
                    found = 1;
                }
            }
    } while (found);

    return given;
}

void bot_choose_move(const Board *b, int *r1, int *c1, int *r2, int *c2)
{
    static int seeded = 0;
    Line lines[MAX_LINES], safe[MAX_LINES];
    int n, n_safe = 0, i, best, cost, best_cost;

    if (!seeded) {                  /* so the bot does not always play the same game */
        srand((unsigned) time(NULL));
        seeded = 1;
    }

    n = free_lines(b, lines);

    /* Level 1: take a box if we can. */
    for (i = 0; i < n; i++)
        if (touches_box_with(b, lines[i], 3)) {
            line_to_dots(lines[i], r1, c1, r2, c2);
            return;
        }

    /* Level 2: play a random safe line (never give a box its 3rd side). */
    for (i = 0; i < n; i++)
        if (!touches_box_with(b, lines[i], 2))
            safe[n_safe++] = lines[i];
    if (n_safe > 0) {
        line_to_dots(safe[rand() % n_safe], r1, c1, r2, c2);
        return;
    }

    /* Level 3: every line gives something away: give away the fewest boxes. */
    best = 0;
    best_cost = boxes_given(b, lines[0]);
    for (i = 1; i < n; i++) {
        cost = boxes_given(b, lines[i]);
        if (cost < best_cost) {
            best_cost = cost;
            best = i;
        }
    }
    line_to_dots(lines[best], r1, c1, r2, c2);
}
