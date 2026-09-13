; handler para las interrupciones gatilladas por la linea proceed del bus SIO

	.export _proceed_handler
	.import _proceed_flag

_proceed_handler:
	LDA #$01
	STA _proceed_flag
	PLA
	RTI
