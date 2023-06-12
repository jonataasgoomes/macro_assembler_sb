#include <string>
#include <vector>

#include "asm_lib.h"


int hexaParaDecimal(const std::string& numeroHexadecimal) {
    int numeroDecimal = 0;
    int base = 1;

    // Começando da última posição (menos significativa) até a primeira posição (mais significativa)
    for (int i = numeroHexadecimal.length() - 1; i >= 0; i--) {
        // Verificando se o caractere é um dígito hexadecimal válido
        if (numeroHexadecimal[i] >= '0' && numeroHexadecimal[i] <= '9') {
            numeroDecimal += (numeroHexadecimal[i] - '0') * base;
        }
        // Verificando se o caractere é uma letra hexadecimal válida (A-F ou a-f)
        else if (numeroHexadecimal[i] >= 'A' && numeroHexadecimal[i] <= 'F') {
            numeroDecimal += (numeroHexadecimal[i] - 'A' + 10) * base;
        }
        // Verificando se o caractere é uma letra hexadecimal válida (a-f)
        else if (numeroHexadecimal[i] >= 'a' && numeroHexadecimal[i] <= 'f') {
            numeroDecimal += (numeroHexadecimal[i] - 'a' + 10) * base;
        }

        base *= 16; // Avança para o próximo dígito hexadecimal de ordem superior
    }

    return numeroDecimal;
}



