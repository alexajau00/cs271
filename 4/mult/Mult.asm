@2 //load r2
M=0 //set R2 to 0

@1 //load r1
D=M //get value of r1
@INFINITE_LOOP 
D;JEQ //if r1 = 0 jump to end

@0 //load r0
D=M //get value of r0
@INFINITE_LOOP
D;JEQ //if r0 = 0 jump to end

(LOOP)
	@0 //load r0
	D=M //load the value of r0
	@2 //load R2
	M=D+M //add r0 to r2

	@1 //load r1
	M=M-1 //r1 = r1 - 1
	
	D=M //load updated r1
	@LOOP
	D;JGT //if r1>0 then loop

(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP //infinite loop