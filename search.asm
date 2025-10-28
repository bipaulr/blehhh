DATA SEGMENT
    ARRA DB 11H,22H,33H,44H,55H
    MSG1 DB "FOUND$"
    MSG2 DB "NOT FOUND$"
    SE DB 33H
DATA ENDS

PRINT MACRO MSG
    MOV AH, 09H
    LEA DX, MSG
    INT 21H
ENDM

CODE SEGMENT
    ASSUME CS:CODE, DS:DATADATA SEGMENT
    STRING1 DB 11H,22H,33H,44H,55H
    STRING_LEN EQU $ - STRING1     
    
    MSG1 DB "FOUND AT POSITION: $"
    MSG2 DB "NOT FOUND$"
    SE DB 33H
DATA ENDS

PRINT MACRO MSG
    MOV AH, 09H
    LEA DX, MSG
    INT 21H
ENDM

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX
    
    MOV AL, SE          
    LEA SI, STRING1     
    MOV CX, STRING_LEN 
    MOV DL, 01H         
    
UP:
    MOV BL, [SI]        
    CMP AL, BL          
    JZ FO                
    
    INC SI             
    INC DL              
    
    DEC CX              
    JNZ UP             
    PRINT MSG2          
    JMP END1            
FO:
    PRINT MSG1          
    ADD DL, 30H         
    MOV AH, 02H          
    INT 21H              
END1:
    MOV AH, 4CH         
    INT 21H
CODE ENDS
END START
START:
    MOV AX, DATA
    MOV DS, AX
    MOV AL, SE          
    LEA SI, STRING1     
    MOV CX, 05H         
UP:
    MOV BL, [SI]         
    CMP AL, BL          
    JZ FO               
    INC SI              
    DEC CX            
    JNZ UP            
    PRINT MSG2          
    JMP END1            
FO:
    PRINT MSG1         
END1:
    MOV AH, 4CH         
    INT 21H
CODE ENDS
END START