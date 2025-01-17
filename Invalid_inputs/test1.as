f: 
    ; check operand kind 
    cmp #5, r1
    cmp #5
    cmp
    add *r5, f
    add *r5, #-996
    add *r5
    add
    lea f, *f
    lea r3, #5
    lea r3
    lea
    not r1
    not #0
    not
    not r1, *r1
    jmp f
    jmp *r1
    jmp r7
    jmp #-97
    jmp #-97, #3
    stop
    stop #5
    stop f, *r1
    red f
    red #9
    red #9, r1
    red
    prn r4
    prn
    prn r6, *r6

    ; check of known identifiers
    cmp fg, a
    cmp ad, f
    cmp f, ad







