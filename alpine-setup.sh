#!/bin/sh
# Run as root inside the Alpine VM, from the project folder:
#   sh alpine-setup.sh
# Installs build tools, builds the game, and makes it start on tty1 at boot
# instead of the login prompt.
set -e

apk update
apk add build-base gdb valgrind git

make clean
make
make install            # copies the binary to /usr/local/bin/dotsboxes

# Back up inittab once, then replace the tty1 getty (login) with the game.
[ -f /etc/inittab.bak ] || cp /etc/inittab /etc/inittab.bak
sed -i 's|^tty1::respawn:.*|tty1::respawn:/usr/local/bin/dotsboxes|' /etc/inittab

echo "tty1 line is now:"
grep '^tty1' /etc/inittab
echo "Done. Run 'reboot' to test: the game should appear without a login."
echo "To undo: cp /etc/inittab.bak /etc/inittab"
