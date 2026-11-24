#ifndef GAME_H
#define GAME_H

/* Runs one full game. vs_bot = 0: two players, vs_bot = 1: player A vs bot (B).
 * Returns 0 when the game finished normally, -1 if input ended (EOF). */
int game_run(int vs_bot);

#endif
