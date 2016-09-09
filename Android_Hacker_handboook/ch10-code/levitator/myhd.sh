#!/bin/sh
hexdump -e '"%07.7_ax " 4/4 "%8x " "  \|"' -e '16/1 "%_p" "|\n"' $*
