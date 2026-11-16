# Dots and Boxes — CMPS 270 Project (Sprint 1)

Two-player console Dots and Boxes in C, 4x5 boxes (5x6 dots).

## Build & run
```sh
make            # builds ./dotsboxes
./dotsboxes
make valgrind   # runs the game under Valgrind
make clean
```

## How to play
On your turn, type 4 numbers: `row1 col1 row2 col2` (e.g. `0 0 0 1`).
The dots must be next to each other. Completing a box puts your letter in it
and gives you another turn. When all 20 boxes are claimed, most boxes wins.

## Files
| File | Role |
|---|---|
| `board.h/.c` | Board data, move validation, drawing lines, box completion |
| `display.h/.c` | Printing the board, scores, winner |
| `game.h/.c` | Turn loop and input handling |
| `main.c` | Entry point, play-again loop |
| `Makefile` | Build, install, valgrind, clean |
| `alpine-setup.sh` | Alpine VM setup: tools, build, auto-start on boot |

## Board representation
- `horiz[5][5]`: line between dot (r,c) and (r,c+1)
- `vert[4][6]`: line between dot (r,c) and (r+1,c)
- `owner[4][5]`: 'A', 'B' or ' ' for each box

Checking whether a move completes a box is **O(1)**: a line touches at most
2 boxes, and each box needs 4 lookups.

## Alpine VM auto-start
Tested on Alpine Linux 3.24 (VirtualBox, installed to disk with `setup-alpine`).
A fresh Alpine has no git, so install it first. Inside the VM as root:
```sh
apk add git
git clone <repo-url> DotsAndBoxes
cd DotsAndBoxes
sh alpine-setup.sh
reboot
```
The script replaces the `tty1` login line in `/etc/inittab` with the game,
so the game shows directly at boot (no username/password). After "Play again? n"
init restarts the game. For a shell, press Alt+F2 and log in as root.
Backup is kept at `/etc/inittab.bak` (undo: `cp /etc/inittab.bak /etc/inittab`).
