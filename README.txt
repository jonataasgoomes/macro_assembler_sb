Alunos:
140146407 - Jônatas Gomes Pinto Júnio
130115428 - ITALO FRANKLIN CARDOSO VAZ

Código feito no editor sublime text 3 usando GCC no terminal.

Testado em WINDOWS E MAC - Abrir terminal na pasta com arquivo make.

MAKE - para compilar

MONTADOR ENTRADAS 1 até 4 no formato:
./montador NOME1 NOME2 NOME3 NOME4   -> saída OBJ

    instrucoes usadas
        {"ADD", "1", 2},
        {"SUB", "2", 2},
        {"MUL", "3", 2},
        {"DIV", "4", 2},
        {"JMP", "5", 2},
        {"JMPN", "6", 2},
        {"JMPP", "7", 2},
        {"JMPZ", "8", 2},
        {"COPY", "9", 3},
        {"LOAD", "10", 2},
        {"STORE", "11", 2},
        {"INPUT", "12", 2},
        {"OUTPUT", "13", 2},
        {"STOP", "14", 1},
  

    diretivas usadas {
        {"CONST", 1},
        {"SPACE", 1},
        {"EXTERN", 1},
        {"PUBLIC", 0},
        {"BEGIN", 0},
        {"END", 0},
        {"SECTION", 1}
 