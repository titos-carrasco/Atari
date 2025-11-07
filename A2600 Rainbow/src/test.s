    .SETCPU "6502"
    .INCLUDE "atari2600.inc"

    .SEGMENT "STARTUP"

    .SEGMENT "CODE"

    ; ----------------------------------------------------------------------
    ; A2600 Rainbow
    ; ----------------------------------------------------------------------
.proc Main
    jsr Init

GameLoop:
    jsr VerticalSync
    jsr VerticalBlank
    jsr HorizontalBlank
    jsr OverScan

    jmp GameLoop
.endproc

    ; ----------------------------------------------------------------------
    ; Inicializamos el programa
    ; ----------------------------------------------------------------------
.proc Init
    ; establecemos los colores
    lda #$30
    sta Color1
    sta Color2  

    rts
.endproc

    ; ----------------------------------------------------------------------
    ; VERTICAL SYNC: 3 lineas scan
    ; ----------------------------------------------------------------------
.proc VerticalSync
    ; iniciamos la sincronizacion vertical (inicio del frame)
    lda #2
    sta VSYNC

    ; debemos sincronizarnos con 3 lineas scan
    sta WSYNC
    sta WSYNC
    sta WSYNC

    ; finalizamos la sincronizacion vertical
    lda #0
    sta VSYNC

    rts
.endproc

    ; ----------------------------------------------------------------------
    ; VERTICAL BLANK: 37 lineas scan (37*76 = 2812 ciclos de CPU, 2812/64 = 43.9375)
    ; ----------------------------------------------------------------------
.proc VerticalBlank
    ; preparamos el timer que nos avisara cuando se nos agote el tiempo
    lda #43
    sta TIM64T

    ; aqui se puede procesar

    ; nos sincronizamos a la ultima linea scan del vertical blank
loop:
    sta WSYNC
    lda INTIM
    bne loop
    sta WSYNC

    rts
.endproc

    ; ----------------------------------------------------------------------
    ; HORIZONTAL BLANK: 192 lineas scan
    ; ----------------------------------------------------------------------
.proc HorizontalBlank
    ; coloreamos el arcoriris
    ldx #192
loop:
    lda Color2
    sta COLUBK
    inc Color2

    ; esperamos la siguiente linea scan
    dex 
    sta WSYNC
    bne loop

    rts
.endproc

    ; ----------------------------------------------------------------------
    ; OVERSCAN: 30 lineas scan (30*76 = 2280 ciclos de CPU, 2280/64 = 35.625)
    ; ----------------------------------------------------------------------
.proc OverScan
    ; dejamos en negro el resto de las lineas scan
    lda #0
    sta COLUBK

    ; preparamos el timer que nos avisara cuando se nos agote el tiempo
    lda #36
    sta TIM64T

    ; rotamos los colores
    inc Color1
    lda Color1
    sta Color2

OverscanLoop:
    sta WSYNC
    lda INTIM
    bne OverscanLoop

    rts
.endproc

    ; ----------------------------------------------------------------------
    ; Data
    ; ----------------------------------------------------------------------
    .SEGMENT "BSS"
Color1: 
    .byte 0
Color2: 
    .byte 0

    ; ----------------------------------------------------------------------
    ; Vectores de interrupcion
    ; ----------------------------------------------------------------------
    .SEGMENT "VECTORS"
    .word Main          ; NMI
    .word Main          ; RESET
    .word Main          ; IRQ

