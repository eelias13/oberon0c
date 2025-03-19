function downloadJson(jsonData, fileName) {
    const jsonString = JSON.stringify(jsonData, null, 4);
    const blob = new Blob([jsonString], { type: "application/json" });
    const link = document.createElement("a");
    link.href = URL.createObjectURL(blob);
    link.download = fileName;
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
}

function downloadText(text, fileName) {
    const blob = new Blob([text], { type: "text/plain" });
    const link = document.createElement("a");
    link.href = URL.createObjectURL(blob);
    link.download = fileName;
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
}



function init() {
    setupEditor();
}


function setupEditor() {
    window.require.config({ paths: { 'vs': 'monaco-editor/min/vs' } });
    window.MonacoEnvironment = { getWorkerUrl: () => 'monaco-editor/min/vs/base/worker/workerMain.js' };

    window.require(['vs/editor/editor.main'], function () {
        monaco.languages.register({ id: 'oberon0' });

        // Define syntax highlighting
        monaco.languages.setMonarchTokensProvider('oberon0', {
            keywords: ['MODULE', 'BEGIN', 'END', 'VAR', 'PROCEDURE', 'RETURN', 'IF', 'THEN', 'ELSE', 'WHILE', 'DO'],
            tokenizer: {
                root: [
                    [/\b(?:MODULE|BEGIN|END|VAR|PROCEDURE|RETURN|IF|THEN|ELSE|WHILE|DO)\b/, 'keyword'],
                    [/[a-zA-Z_][\w\d]*/, 'identifier'],
                    [/\d+/, 'number'],
                    [/[+\-*/=]/, 'operator'],
                    [/".*?"/, 'string'],
                    [/\(\*[\s\S]*?\*\)/, 'comment'],
                ]
            }
        });

        // Add basic auto-completion for Oberon-0
        monaco.languages.registerCompletionItemProvider('oberon0', {
            provideCompletionItems: function () {
                return {
                    suggestions: [
                        { label: 'MODULE', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'MODULE ' },
                        { label: 'BEGIN', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'BEGIN ' },
                        { label: 'END', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'END ' },
                        { label: 'VAR', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'VAR ' },
                        { label: 'PROCEDURE', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'PROCEDURE ' },
                        { label: 'RETURN', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'RETURN ' },
                        { label: 'IF', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'IF ' },
                        { label: 'THEN', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'THEN ' },
                        { label: 'ELSE', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'ELSE ' },
                        { label: 'WHILE', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'WHILE ' },
                        { label: 'DO', kind: monaco.languages.CompletionItemKind.Keyword, insertText: 'DO ' }
                    ]
                };
            }
        });
        const editor = monaco.editor.create(document.getElementById('editor'), {
            value: getCode(),
            language: 'oberon0',
            theme: 'vs-dark'
        });

        document.getElementById("code").onclick = () => {
            downloadText(editor.getValue(), "default.Mod")
        };

        Module().then(instance => {
            document.getElementById("tokens").onclick = () => {
                const scanner = instance.cwrap('scanner', 'number', ['string']);
                let ptr = scanner(editor.getValue());
                let data = instance.UTF8ToString(ptr);
                downloadJson(JSON.parse(data));
            };

            document.getElementById("ast").onclick = () => {
                const parse = instance.cwrap('parse', 'number', ['string']);
                let ptr = parse(editor.getValue());
                let data = instance.UTF8ToString(ptr);
                downloadJson(JSON.parse(data), "ast.json");

            };

            document.getElementById("checked-ast").onclick = () => {
                const check = instance.cwrap('check', 'number', ['string']);
                let ptr = check(editor.getValue());
                let data = instance.UTF8ToString(ptr);
                downloadJson(JSON.parse(data), "checkedAst.json");

            };
        });
    });

    document.getElementById("theme").onchange = () => {
        monaco.editor.setTheme(document.getElementById("theme").value);
    };






}

function getCode() {

    return `(* Oberon-0 implementations of various sort algorithms. *)
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


(* Applies the quick-sort algorithm to the global array. *)
PROCEDURE QuickSort;

    PROCEDURE QSort(l, r: INTEGER);
    VAR i, j, x: INTEGER;
    BEGIN
        i := l;
        j := r;
        x := a[(r + l) DIV 2];
        WHILE i <= j DO
            WHILE a[i] < x DO i := i + 1 END;
            WHILE x < a[j] DO j := j - 1 END;
            IF i <= j THEN
                Swap(a[i], a[j]);
                i := i + 1;
                j := j - 1
            END
        END;
        IF l < j THEN QSort(l, j) END;
        IF i < r THEN QSort(i, r) END
    END QSort;

BEGIN
    QSort(0, Dim - 1)
END QuickSort;

(* Main program. *)
BEGIN
    Init;
    QuickSort
END Sort0.`;
}


