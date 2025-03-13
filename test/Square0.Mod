MODULE Square;
VAR
  num, result: INTEGER;

PROCEDURE CalcSquare(x: INTEGER);
VAR a: INTEGER;
BEGIN
  a := 5;
  result := x * x
END CalcSquare;

BEGIN

  num := 4;
  CalcSquare(num)
END Square.