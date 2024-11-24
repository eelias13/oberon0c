MODULE Square;
VAR
  num, result: INTEGER;

PROCEDURE CalcSquare(x: INTEGER);
BEGIN
  result := x * x
END CalcSquare;

BEGIN
  num := 4;
  CalcSquare(num);
  OUT.Int(result, 0)
END Square.