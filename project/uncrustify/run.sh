#!/bin/bash
#
#
#

function uncrustifyFolder()
{
    find $1 \( -name "*.cpp" -or -name "*.h" \) -exec uncrustify -c uncrustify-bsd-ksti.cfg -f {} -o {} \;
}

uncrustifyFolder "../source"
uncrustifyFolder "../lib"
uncrustifyFolder "../include"
