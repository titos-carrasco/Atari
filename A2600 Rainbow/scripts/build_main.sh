DIR=obj
FILE=main
CFG=/usr/share/cc65/cfg/atari2600.cfg
INC=/usr/share/cc65/asminc/atari2600.inc

rm -f $DIR/$FILE.*
[ -d $DIR ] || mkdir $DIR

cc65 -t atari2600 -T -Cl -Oir -o $DIR/$FILE.s $FILE.c && \
ca65 -t atari2600 -o $DIR/$FILE.o $DIR/$FILE.s && \
ld65  -C $CFG -m $DIR/$FILE.map -Ln $DIR/$FILE.labels -vm $DIR/$FILE.o --lib atari2600.lib -o $DIR/$FILE.bin && \
stella -format ntsc $DIR/$FILE.bin
