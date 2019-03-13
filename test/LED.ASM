;***************************************************************************
;**                                                                       **
;**  Ejemplo: parpadeo de un led                                          **
;**                                                                       **
;***************************************************************************

        ORG     $0000           ;direccion de comienzo en RAM

ini
        LDAA    #$00   
        STAA    $1004           ;carga en portB el valor de A
        LDY     #$3             ;carga en Y el valor FFFF, mi contador
	BSR	dec

	LDAA	#$01
	STAA	$1004
	LDY	#$3
	BSR	dec

	BRA	ini
;--------------------------------------------------------

dec     DEY                     ;decrementa
        CPY     #0              ;compara (resta) Y-0
        BNE     dec             ;salta si no es igual (z=0)
        RTS


        END                     ;directiva final para el programa ensamblador


