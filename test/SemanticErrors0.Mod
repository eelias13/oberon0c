MODULE SemanticErrors;

CONST Dim = 50;

TYPE
    INTARRAY = ARRAY Dim OF INTEGER;
    INT = INTEGER;
    STUDENT = RECORD ID,IQ,ECTS : INTEGER;
              POINTS : INTARRAY END;

VAR a : INTARRAY; b : FUN_TYPE; c : a;

PROCEDURE f(VAR a, b : INTEGER);
BEGIN
    a := a + b
END f;

PROCEDURE f(VAR a: INTEGER);
BEGIN
    a := a
END f;

PROCEDURE q(VAR i: INTEGER);
BEGIN
    i := a;
    a[5000000000] := 2
END f;

PROCEDURE g(VAR s, q: STUDENT);
VAR i: INTEGER;
BEGIN
    i := 0;
    i.f := 12;
    WHILE i DO
        s.IQ := s.IQ + 12;
        s.NAME := i;
        s.POINTS := 33 + 42 - 7 * (12 - 2)
    END;

    IF s = q THEN i := i - 1 END
END g;

BEGIN
    q(30,50);
    q(a);
    q;
    g(9,a)
END SemanticErrors.

