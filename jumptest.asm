		
		LD B, 1			; B is the possible prime

loop:	LD A, B 
		LD D, 2			; B is factor
sub:	LD A, B
		SUB D			; subtract D from A, store in A
		JR  z,Even		; If zero, even division
		JR  m,notEven	; if negative, not evenly division
		JR sub

		INC	D			; Increment factor checking
		LD A, D
		CP B
		JR C,sub		; If factor < possible prime, jump to subtract 
		SUB B
		JR 
Even:
		LD C, 0
		LD A, B			; put our prime back into A
		LD B, 19
		OTIR
		LD D, 0Ah
		OUT (C),D
        JP $0000


DispA:
		ld	c,-100
		call	Na1
		ld	c,-10
		call	Na1
		ld	c,-1
Na1:	ld	b,'0'-1
Na2:	inc	b
		add	a,c
		jr	c,Na2
		sub	c		;works as add 100/10/1
		push af		;safer than ld c,a
		ld	a,b		;char is in b
		ld  c,0
		out (c),b
		pop af		;safer than ld a,c
		ret

Divide:                          ; this routine performs the operation BC=HL/E
  ld a,e                         ; checking the divisor; returning if it is zero
  or a                           ; from this time on the carry is cleared
  ret z
  ld bc,-1                       ; BC is used to accumulate the result
  ld d,0                         ; clearing D, so DE holds the divisor
DivLoop:                         ; subtracting DE from HL until the first overflow
  sbc hl,de                      ; since the carry is zero, SBC works as if it was a SUB
  inc bc                         ; note that this instruction does not alter the flags
  jr nc,DivLoop                  ; no carry means that there was no overflow
  ret

string: DB "Goodbye ECE Honors!"
		DS 40



