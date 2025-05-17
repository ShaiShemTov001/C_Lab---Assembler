;this file is for checking all the errors regarding text errors
;extraneous text:
    rts abc
prn 5 abc
mov @r1, @r4 abc
.string "myNameIsShai" abc
    .entry LABEL abc

;line deviation:

    MAIN:    .data    -6 ,  -7  , 1 , 2 , 3 , 4 , 5 , 6 ,  4 , 3 , 1 ,  2 , 2 , 3 , 4 , 5 , 3 , 2 , 4 , 5 , 6 , 7 , 6 , 5 , 4 , 3 , 2 , 1 , 2 , 4

;macro name deviation:

    mcro macroistolongforthisfrojectsoitprintserror

;label name deviation:

    LABELISTOLONGFORTHISPROJECTSOITPRINTSERROR:

    ;text out of apostrophes:

    .string absd"
    .string "abscd" efg
    .string abc "defg"

;no closing apostrophes:

    .string "abcdf