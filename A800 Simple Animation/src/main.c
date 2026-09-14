#include <atari.h>
#include <conio.h>
#include <string.h>

// Búfer estático de 4KB para garantizar que dentro de él
// exista una dirección alineada a un bloque de 2048 bytes (2KB)
static unsigned char pmg_buffer[0x1000];
static unsigned char* pmg_memory;

void init_pmg(void) {
    // 1. Alineamos el puntero manualmente a un límite de 2048 bytes
    pmg_memory = (unsigned char*)(((unsigned int)pmg_buffer + 2047) & ~2047);

    // 2. Limpiamos los 2KB de la memoria PMG
    memset(pmg_memory, 0, 0x0800);

    // 3. Dirección base de PMG en ANTIC (página alta)
    ANTIC.pmbase = ((unsigned int)pmg_memory) >> 8;

    // 4. Habilitar Players y Missiles en GTIA y registro sombra de OS
    GTIA_WRITE.gractl = 3; 
    OS.sdmctl = 0x2E; 

    // 5. Asignar el color al Player 0 (colpm0)
    GTIA_WRITE.colpm0 = 0x28;
}

void draw_sprite(int y) {
    // Player 0 está en el offset 0x0400 de la memoria PMG (resolución simple)
    unsigned char* player0 = &pmg_memory[0x0400];

    // Limpiamos la columna del Player 0
    memset(player0, 0, 256);

    // Dibujamos la forma de 8x8 en la posición Y
    player0[y + 0] = 0b00111100;
    player0[y + 1] = 0b01111110;
    player0[y + 2] = 0b11011011;
    player0[y + 3] = 0b11111111;
    player0[y + 4] = 0b11111111;
    player0[y + 5] = 0b11011011;
    player0[y + 6] = 0b01000010;
    player0[y + 7] = 0b00111100;
}

void main(void) {
    unsigned char x = 120;
    unsigned char y = 60;

    cursor(0);
    clrscr();
    
    init_pmg();
    draw_sprite(y);

    while (1) {
        // Posición horizontal del Player 0 en GTIA
        GTIA_WRITE.hposp0 = x;

        if (kbhit()) {
            switch (OS.ch) {
                case KEY_UP:
                case KEY_W:
                    if (y > 16) y -= 2;
                    draw_sprite(y);
                    break;
                case KEY_DOWN:
                case KEY_S:
                    if (y < 210) y += 2;
                    draw_sprite(y);
                    break;
                case KEY_LEFT:
                case KEY_A:
                    if (x > 48) x -= 2;
                    break;
                case KEY_RIGHT:
                case KEY_D:
                    if (x < 200) x += 2;
                    break;
            }
            OS.ch = 255;
        }

        waitvsync();
    }
}