#ifndef BOARD_H
#define BOARD_H

/* Grid size in BOXES: 4 rows x 5 columns of boxes
 * => 5 rows x 6 columns of dots. */
#define BOX_ROWS 4
#define BOX_COLS 5
#define DOT_ROWS (BOX_ROWS + 1)
#define DOT_COLS (BOX_COLS + 1)
#define TOTAL_BOXES (BOX_ROWS * BOX_COLS)

#define EMPTY ' '

typedef struct {
    /* horiz[r][c] = 1 if a line joins dot (r,c) and dot (r,c+1) */
    int horiz[DOT_ROWS][BOX_COLS];
    /* vert[r][c] = 1 if a line joins dot (r,c) and dot (r+1,c) */
    int vert[BOX_ROWS][DOT_COLS];
    /* owner[r][c] = 'A', 'B' or EMPTY for the box whose top-left dot is (r,c) */
    char owner[BOX_ROWS][BOX_COLS];
    int boxes_claimed;
} Board;

/* Result codes for board_play_move */
#define MOVE_OUT_OF_RANGE  -1
#define MOVE_NOT_ADJACENT  -2
#define MOVE_ALREADY_TAKEN -3

/* Clears every line and box. */
void board_init(Board *b);

/* Draws the line between (r1,c1) and (r2,c2) for player `p`.
 * Returns the number of boxes completed by this move (0, 1 or 2),
 * or a negative MOVE_* code if the move is invalid (board unchanged). */
int board_play_move(Board *b, int r1, int c1, int r2, int c2, char p);

/* 1 when every box has an owner. */
int board_is_full(const Board *b);

/* Number of boxes owned by player p. */
int board_count(const Board *b, char p);

#endif
