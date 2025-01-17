; file fibonacci.ps

.entry MAIN
.entry FIBONACCI
.extern PRINT

MAIN:   mov #10, r0
         ; Number of Fibonacci terms to calculate
         ; Jump to the Fibonacci function
         jsr FIBONACCI
         ; Stop execution
         stop
         ; Print the value of the Fibonacci number
         prn r4
         ; Just a test of extern
         lea PRINT, r4


FIBONACCI: clr r1
         ; r1 = 0 (first Fibonacci number)
         ; r2 = 1 (second Fibonacci number)
         mov #1, r2
         ; r3 will be used to count terms
         clr r3
         ; r4 will hold the current Fibonacci number
         clr r4

         ; Jump to the FIBLOOP to start the loop
         jmp FIBLOOP

FIBLOOP: cmp r3, r0
         ; Compare the count with the number of terms
         ; If count >= number of terms, exit loop
         bne ENDFIB
         ; Print the current Fibonacci number
         prn r1
         ; r2 = r1 + r2 (next Fibonacci number)
         add r1, r2
         ; Store the result in r4
         mov r2, r4
         ; Update r2 to the new Fibonacci number
         mov r4, r2
         ; Update r2 to the old Fibonacci number
         sub r1, r2
         ; Increment the term count
         add #1, r3
          ; Repeat the loop
         jmp FIBLOOP

ENDFIB: rts
         ; Return from the function

