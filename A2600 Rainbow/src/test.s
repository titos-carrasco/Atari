    .SETCPU "6502"
    .INCLUDE "atari2600.inc"

    .SEGMENT "STARTUP"

    .SEGMENT "CODE"

Reset:
    ; limpia la RAM y los registros TIA
    ldx #0
    lda #0
Limpiar:
    sta 0,x
    inx
    bne Limpiar

    lda #$30
    sta Color1
    sta Color2  

    ; comenzamos con el beam apagado y nos sincronizamos al inicio del frame
    lda #%01000010
    sta VBLANK
    lda #2
    sta VSYNC

FrameLoop:
    ; ----------------------------------------------------------------------
    ; sincronizacion de video (3 lineas)
    ; cada instruccion detiene la cpu hasta el inicio de la siguiente linea
    ; ----------------------------------------------------------------------
    sta WSYNC
    sta WSYNC
    sta WSYNC

    ; indica al TV que comenzaremos a dibujar
    lda #0
    sta VSYNC

    ; encendemos el beam
    lda #%01000000
    sta VBLANK

    ; ----------------------------------------------------------------------
    ; vertical blank (37 lineas)
    ; podemos mostrar algo consumiendo las 37 lineas
    ; ----------------------------------------------------------------------

    ; mostramos algo
    jsr ShowNumbers

    ; ----------------------------------------------------------------------
    ; contenido del frame  (192 lineas)
    ; ----------------------------------------------------------------------

    ldx #192
loop:
    lda Color2
    sta COLUBK
    inc Color2

    ; esperamos la siguiente scanline
    dex 
    sta WSYNC
    bne loop

    ; ----------------------------------------------------------------------
    ; overscan (30 lineas)
    ; ----------------------------------------------------------------------

    ; apagamos el beam
    lda #%01000010
    sta VBLANK

    ; aqui podemos procesar cuidando el tiempo que disponem

    ; rotamos los colores
    inc Color1
    lda Color1
    sta Color2

    ; ----------------------------------------------------------------------
    ; indicamos al TV que termine el frame e inicie el siguiente
    ; ----------------------------------------------------------------------
    lda #2
    sta VSYNC

    jmp FrameLoop

.proc ShowNumbers
    ; debemos utilizar 37 scanlines

    ; seteamos el color de fondo
    lda #$00
    sta COLUBK

    ; usaremos el color blanco para el playfield
    lda #$0F
    sta COLUPF

    ; en Stella no vemos las primeras 23 lineas del vertical blank
    ldx #23
loop:
    sta WSYNC
    dex
    bne loop

    ; utilizaremos el playfield para ocupar las 14 lineas que nos quedan
    lda #%00000111
    sta PF1
    sta WSYNC
    sta WSYNC

    lda #%00000101
    sta PF1
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC

    lda #%00000111
    sta PF1
    sta WSYNC
    sta WSYNC

    ; limpiamos el playfield
    lda #$00    
    sta PF1

    rts
.endproc

    .SEGMENT "BSS"
Color1: 
    .byte 0
Color2: 
    .byte 0

    .SEGMENT "VECTORS"
    .word Reset          ; NMI
    .word Reset          ; RESET
    .word Reset          ; IRQ

