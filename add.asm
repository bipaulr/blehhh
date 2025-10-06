MEMORY ADDRESS	OBJECT CODE	    MNEMONICS
1000	        8A 26 00 11	    MOV AH,[1100]
1004	        8A 1E 01 11	    MOV BL,[1101]
1008	        C6 C0 00	    MOV AL,00
100B	        00 DC	        ADD AH,BL
100D	        73 02	        JNC 1011
100F	        FE C0	        INC AL
1011	        88 06 02 11	    MOV [1102],AL
1015	        88 26 03 11	    MOV [1103],AH
1019	        F4	            HLT