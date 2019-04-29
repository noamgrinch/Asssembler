; file ps.as

.entry LOOP 
.entry OMAR
.extern L3
MAIN:	mov  M1,M1[r2][r5]
	add STR,r2
LOOP:	jmp W
	prn #-5
	sub r1,r4
SANE:	inc LENGTH
	rts 
.entry LENGTH
	sub M1[r2][r4],r5
	lea OMAR,SANE
	mov #-9,r1 
.extern W
	bne L3
	lea M1[r7][r6],M1[r2][r5]
OMAR:	prn A
END:	stop 
STR: .string "abcdef"
LENGTH: .data 6,9,15
.string "woop wpp"
K:	.data 22
M1:	.mat[2][2] 1,2,3,4
X: .data 2
A: 	.string "wassup"
  .data 1,-111
