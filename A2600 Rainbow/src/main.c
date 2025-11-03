/*
https://www.taswegian.com/WoodgrainWizard/tiki-index.php?page=TIA
https://www.randomterrain.com/atari-2600-memories-tutorial-andrew-davie-07.html

3 Scanlines devoted to the vertical synchronization.
37 scanlines of vertical blank time.
192 (NTSC) or 242 (PAL) lines of actual picture.
30 scanlines of overscan.
*/

#include <atari2600.h>

void show_numbers()
{
    // debemos utilizar 37 scanlines
    unsigned char i;

    // seteamos el color de fondo
    TIA.colubk = 0x00;

    // usaremos color blanco para el playfield
    TIA.colupf = 0x0F;

    // en Stella no vemos las primeras 23 lineas del vertical blank
    for (i = 0; i < 23; i++)
        TIA.wsync = 0x00;

    // utilizaremos el playfield para ocupar las 14 lineas que nos quedan
    TIA.pf1 = 0b00000111;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000111;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000101;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000111;
    TIA.wsync = 0x00;
    TIA.pf1 = 0b00000111;
    TIA.wsync = 0x00;

    // limpiamos el playfield
    TIA.pf1 = 0b00000000;
}

void main()
{
    unsigned char color1 = 0x30, color2 = 0x30, i = 0, n = 0;

    // comenzamos con el beam apagado y nos sincronizamos al inicio del frame
    TIA.vblank = 0b01000010;
    TIA.vsync = 0x02;

    for (;;)
    {
        // ----------------------------------------------------------------------
        // sincronizacion de video (3 lineas)
        // cada instruccion detiene la cpu hasta el inicio de la siguiente linea
        // ----------------------------------------------------------------------
        TIA.wsync = 0x00;
        TIA.wsync = 0x00;
        TIA.wsync = 0x00;

        // indica al TV que comenzaremos a dibujar
        TIA.vsync = 0x00;

        // encendemos el beam
        TIA.vblank = 0b01000000;

        // ----------------------------------------------------------------------
        // vertical blank (37 lineas)
        // podemos mostrar algo consumiendo las 37 lineas
        // ----------------------------------------------------------------------
        show_numbers();

        // ----------------------------------------------------------------------
        // contenido del frame  (192 lineas)
        // ----------------------------------------------------------------------

        // generamos el arcoiris
        i = 0;
    loop:
        TIA.colubk = color2;
        color2 = (color2 + 1) & 0xFF;

        // esperamos la siguiente scanline
        ++i;
        TIA.wsync = 0x00;
        if (i < 192)
            goto loop;

        // ----------------------------------------------------------------------
        // overscan (30 lineas)
        // ----------------------------------------------------------------------

        // apagamos el beam
        TIA.vblank = 0b01000010;

        // aqui podemos procesar cuidando el tiempo que disponemos

        // rotamos los colores
        color1 = (color1 + 1) & 0xFF;
        color2 = color1;

        // ----------------------------------------------------------------------
        // indicamos al TV que termine el frame e inicie el siguiente
        // ----------------------------------------------------------------------
        TIA.vsync = 0x02;
    }
}