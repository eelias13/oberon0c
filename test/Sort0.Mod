(* Oberon-0 implementations of various sort algorithms. *)
MODULE Sort0;

(* Length of the array to be sorted. *)
CONST Dim = 2 * 10;

(* Array type *)
TYPE INTARRAY = ARRAY Dim OF INTEGER;

(* Array to be sorted. *)
VAR a: INTARRAY;

(* Initializes the array. *)
PROCEDURE Init();
VAR i: INTEGER;
BEGIN
    i := 0;
    WHILE i < Dim DO
        a[i] := Dim-i;
        i := i + 1
    END
END Init;

(* Swaps the two values passed as var-parameters. *)
PROCEDURE Swap(VAR a, b: INTEGER);
VAR t: INTEGER;
BEGIN
    t := a;
    a := b; 
    b := t
END Swap;

(* Applies the insertion-sort algorithm the the global array. *)
PROCEDURE InsertionSort;
VAR i, j: INTEGER;
BEGIN
    i := 1;
    WHILE i < Dim DO
        j := i;
        WHILE (j > 0) & (a[j - 1] > a[j]) DO
            Swap(a[j], a[j - 1]);
            j := j - 1
        END;
        i := i + 1
    END
END InsertionSort;

(* Applies the selection-sort algorithm the the global array. *)
PROCEDURE SelectionSort;
VAR i, j: INTEGER;
    min: INTEGER;
BEGIN
    i := 0;
    WHILE i < Dim - 1 DO
        min := i;
        j := i + 1;
        WHILE  j < Dim DO
            IF a[j] < a[min] THEN min := j END;
            j := j + 1
        END;
        IF min # i THEN Swap(a[i], a[min]) END;
        i := i + 1
    END
END SelectionSort;
    
(* Applies the bubble-sort algorithm to the global array. *)
PROCEDURE BubbleSort;
VAR i, j: INTEGER;
BEGIN
    i := 0;
    WHILE i < Dim DO
        j := Dim - 1;
        WHILE j > i DO
            IF a[j - 1] > a[j] THEN Swap(a[j - 1], a[j]) END;
            j := j - 1
        END;
        i := i + 1
    END
END BubbleSort;


(* Main program. *)
BEGIN
    Init;
    QuickSort
END Sort0.