#include <atari2600.h>

void init(){
}

void vblank(){
    // mostranos los numeros en la parte alta
    while(RIOT.intim > 15)

    TIA.colubk = 0x00;
    TIA.colupf = 0xF;
    TIA.vblank = 0b01000000;
    TIA.wsync = 0x00;

    TIA.pf1=0b00000111;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000111;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000111;
    TIA.wsync = 0x00;
    TIA.pf1=0b00000111;
    TIA.wsync = 0x00;

    TIA.vblank = 0b01000010;
    TIA.pf1=0b00000000;
}

void overscan(){
}

void main(){
    unsigned char color1=0x30, color2=0x30, i=0, n = 0;

    init();

    // comenzamos con el beam apagado
    TIA.vblank = 0b01000010;

    for(;;){
        // vertical Sync (3 lineas)
        TIA.vsync = 0x02;
        TIA.wsync = 0x00;
        TIA.wsync = 0x00;
        TIA.wsync = 0x00;
        TIA.vsync = 0x00;

        // vertical blank (37 lineas)
        RIOT.tim64t = (37*76)/64 + 1;
        vblank();
        while(RIOT.intim);
        TIA.wsync = 0x00;

        // contenido del frame  (192 lineas)
        i = 0;
        TIA.vblank = 0b01000000;
loop1:
        TIA.colubk = color2;
        color2 = color2 + 1;
        i = i + 1;
        TIA.wsync = 0x00;
        if(i<192) goto loop1;

        // apagamos el beam
        TIA.vblank = 0b01000010;

        // overscan (30 lineas)
        RIOT.tim64t = (30*76)/64 + 1;
        overscan();
        while(RIOT.intim);

        n = n + 1;
        if(n > 4)
            color1 = color1 + 1;
        color2 = color1;
    }
}