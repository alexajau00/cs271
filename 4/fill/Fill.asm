(LOOP)
@KBD //load address of keyboard 
D=M //Set whether the keyboard is pressed
@WHITE
D;JEQ //jump to white if no key is pressed
@BLACK
D;JGT //jump to black if any key is pressed
@LOOP
0;JMP

(WHITE)
@color //var to store the curr color
M=0 //set color to white
@FILL 
0;JMP // jump to add the color

(BLACK)
@color //var to store the curr color
M=-1 //set color to black
@FILL
0;JMP //jump to add the color

(FILL)
@SCREEN //load address of the screen
D=A //load address of screen
@address //var to keep track of the pixel
M=D //address equals the screen

(FILL_SCREEN)
@color 
D=M //load the color
@address
A=M //load the address
M=D //draw the color on the pixel

@address
M=M+1 //move to next pixel
D=M //update address
@KBD
D=A-D //Keyboard - Address
@FILL_SCREEN
D;JGT //jump if address < keyboard
@LOOP
0;JMP // or else loop

