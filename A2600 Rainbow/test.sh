DIR=obj
FILE=test
CFG=/usr/share/cc65/cfg/atari2600.cfg

rm -f $DIR/$FILE.*
[ -d $DIR ] || mkdir $DIR

ca65 -t atari2600 -o $DIR/$FILE.o $FILE.s && \
ld65 -C $CFG -m $DIR/$FILE.map -Ln $DIR/$FILE.labels -vm $DIR/$FILE.o -o $DIR/$FILE.bin && \
stella -format ntsc $DIR/$FILE.bin