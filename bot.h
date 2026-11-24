#ifndef BOT_H
#define BOT_H

#include "board.h"

/* Picks a move for the bot.
 * Writes a valid line that is not drawn yet into r1 c1 r2 c2.
 * Never changes the board. */
void bot_choose_move(const Board *b, int *r1, int *c1, int *r2, int *c2);

#endif
