MEMORY ADDRESS	OBJECT CODE	    MNEMONICS
2000	        A1 00 11	    MOV AX, [1100]
2003	        8B 16 02 11	    MOV DX, [1102]
2007	        8B 1E 04 11	    MOV BX, [1104]
200B	        F7 F3	        DIV BX
200D        	A3 06 11	    MOV [1106], AX
2010        	89 16 08 11	    MOV [1108], DX
2014        	F4	            HLT