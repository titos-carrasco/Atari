    .SETCPU "6502"
    .INCLUDE "atari2600.inc"

    .SEGMENT "CODE"

Reset:
    ; limpia la RAM y los registros TIA
    ldx #0
    lda #0
Limpiar:
    sta 0,x
    inx
    bne Limpiar

    ; inicializamos nuestras variables

    ; comenzamos con el beam apagado
    lda #%01000010
    sta VBLANK

InicioDeFrame:
    ; sincronizamos el inicio del frame
    lda #2
    sta VSYNC
    sta WSYNC
    sta WSYNC
    sta WSYNC
    lda #0
    sta VSYNC

    ; tenemos 37 scanlines para poner codigo
    lda #(37*76)/64 + 1
    sta TIM64T

    ; ***** colocar codigo aqui *****

    ; sincronizamos a la ultima scanline
SYNC37:
    lda INTIM
    bne SYNC37
    sta WSYNC

    ; encendemos el beam
    lda #%01000000
    sta VBLANK

    ; mostramos 192 scanlines de imagen
    ldy #192
    ldx #15
VB192:
    ; procesamos algo
    inx
    stx COLUBK

    ; sincronizamos con la siguiente scanline
    sta WSYNC
    dey
    bne VB192

    ; apagamos el beam
    lda #%01000010
    sta VBLANK

    ; tenemos 30 scanlines para poner codigo
    lda #(30*76)/64 + 1
    sta TIM64T

    ; ***** colocar codigo aqui *****

    ; sincronizamos a la ultima scanline
SYNC30:
    lda INTIM
    bne SYNC30
    sta WSYNC

    jmp InicioDeFrame

    .SEGMENT "VECTORS"
    .word Reset          ; NMI
    .word Reset          ; RESET
    .word Reset          ; IRQ

