;this file is for checking all the errors regarding already declared, undefined, undeclared

;undefined macro declaration:
    mcroa m1

;already defined label(the top 3 are valid):
LABEL: mov @r1, @r3
    .extern W
    .entry X
LABEL: mov @r2, @r4
    .extern W
    .entry X

;label before extern or entry (warning):
LABEL2: .entry LABEL3

;command undefined:
    abc @r1, @r4

    ;undeclare label operand:
    mov LABELUNDECLARED, @r4