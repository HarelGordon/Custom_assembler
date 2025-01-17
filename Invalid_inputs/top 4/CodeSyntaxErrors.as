; missing comma
MAIN: add    r3    LIST
;-----------------------------

; illegal comma
add ,   r3  ,  LIST
;-----------------------------

; consecutive comma
add  r3  ,  ,  LIST
;-----------------------------

; consecutive comma and missing comma ?
add ,  , r3    LIST
;-----------------------------

; extra text
add  r3 ,LIST fd
;-----------------------------

; too many arguments
add  r3 ,LIST ,r4
;-----------------------------

; too little arguments
add  r3
;-----------------------------

; ??
add  r1 		,r3 ,
;-----------------------------

;invalid string
STR: .string "abcd" fdf
;-----------------------------

; duplicate label
MAIN: add    r3 ,   LIST
;-----------------------------

; invalid label name - starts with Integer
9main: prn #7
;-----------------------------

; invalid label name - reserved word (command)
string: sub r1, r4
;-----------------------------

; invalid label name - macro name
macr        m_macr
cmp r3, #-6
bne END
   endmacr
m_macr: sub r1, r4
;-----------------------------


LIST: .data 6, -9
    .data -100
    .extern K

LIST: .data 8





