.extern funcOne

.extern externalLabel1
.entry loopLabel

startLabel:  mov r1, *r2
            prn stringOne
            lea labelOne, r3



            sub r3, dataOne
            jsr funcOne



                add r1, *r2
                
    cmp r3, #-15
    bne funcCall
            prn r2
            clr r6
            mov r7, *r4
            sub stringTwo, labelTwo
            add r1, *r3
            mov dataOne, r5
            jsr funcThree
            stop

stringOne:  .string "Initial string test."
dataOne:    .data 1, -1, 3767, -3767


.entry funcCall
.extern externalLabel2

funcCall:   lea stringTwo, *r6
            inc *r6



            mov *r6, dataOne
            sub r1, *r4
            cmp r3, #-6
            bne loopLabel
labelThree: add r7, *r6



            clr kVar
            sub labelThree, *r2
            jmp finalStop

stringTwo:  .string "Another test string mixed with instr."
kVar:       .data 10, 20, -30, 40, 50

loopLabel:  prn #48
            lea stringThree, *r5
                sub *r4, r5
    jsr funcTwo
    inc r1
            inc r1
            cmp r2, #-20
            bne funcOne
            jsr funcTwo
            dec r3
            jmp loopLabel

.entry funcThree

funcOneLocal: clr r2
              lea stringOne, r3
              add r4, r3
              jsr funcThree
              prn *r1
                  add r1, *r2
    cmp r3, #-15
    bne funcCall

.entry funcOneLocal

labelFive:   .data 100, 200, 300

.extern externalLabel
.extern externalLabel4
.entry funcTwoLocal

funcTwoLocal: clr r5
              mov r1, r6
              add r2, *r3
              sub r4, r5
              jsr funcOneLocal
                  sub *r4, r5
funcTwo: jsr funcTwo
    inc r1



              mov r7, r6



              prn labelFour
              stop

labelOne:     .data 256, -512, 1024, -2048

stringThree:  .string "String inside the data section."
funcThreeLocal: jsr funcOneLocal
                prn *r6
                clr r2
                mov r3, labelSix
                sub r1, r2
                cmp r7, #-30
                bne funcTwoLocal
                    mov r2, *r3
    prn r4
    clr *r5
                stop

labelSix:   mov r1, *r3
    add r2, r3
    sub r4, *r5
    cmp r6, r7
    bne funcOne
    jsr funcTwo
    prn r7

.extern externalLabel5

              mov r2, labelTwo
              cmp r3, r4
              add r5, r6
              sub r7, r1
              jsr funcOneLocal
funcThree: dec r2
              prn *r3
              mov labelOne, r5
              jsr funcThreeLocal
              jmp loopLabel
              prn r6

labelTwo:     .data 5500, -5600, 5700, -5800, 5900

.extern externalLabel6
.entry funcTwo

stringFour:   prn labelTwo
              add r3, *r4

labelFour:   mov r2, *r5
              sub r7, r6
              prn r6

.extern externalLabel7
.extern funcFour

                  add r1, *r2
    cmp r3, #-15
    bne funcCall
              mov r5, labelFour
              add r2, r7
              prn stringFive
              sub r6, *r3
              jmp funcOneLocal

stringFive:   .string "Final string at the end."

.entry finalStop

.extern finalLabel

finalStop:    stop
	  prn r7



