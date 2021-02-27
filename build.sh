if gcc -g -pipe `sourcefiles ./src c` -o./out/6502 `pkg-config ncurses --cflags --libs` ; then
    ./out/6502 $@
else
    echo "Unable to run, build failed"
fi