#ifndef DISPLAY_H
#define DISPLAY_H

#include "board.h"

/* Prints the grid of dots, lines and box owners. */
void display_board(const Board *b);

/* Prints both players' scores followed by a separator line. */
void display_scores(const Board *b);

/* Prints a line of asterisks. */
void display_separator(void);

/* Prints the final result (winner or draw). */
void display_winner(const Board *b);

#endif
