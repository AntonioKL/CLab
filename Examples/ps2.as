MAIN: 		mov K[2-4],LENGTH
		add r2,STR
LOOP: 		jmp END
		prn #-5
		sub r1, r4
		inc K
		mov LOOP[1-13],r3
		bne LOOP
END: 		stop
STR: 		.string "abcdef"
LENGTH: 	.data 6,-9,15
K: 		.data 22
