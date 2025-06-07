#!/bin/sh
set -e

for src in project*.c; do
  [ -e "$src" ] || continue
  echo "Compiling $src"
  gcc "$src" -lm -lncurses -o /tmp/test_bin
done

echo "All files compiled successfully."
