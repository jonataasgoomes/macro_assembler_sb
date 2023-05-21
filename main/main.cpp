#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])

{
    if (argc < 2) {
        std::cout << "Erro: Numero de argumentos insuficiente, use./montador nomedoarquivo ." << std::endl;
        return 1;
    }

    std::string filename = argv[1];  // Obtém o nome do arquivo do primeiro argumento

    std::ifstream inputFile(filename); //Abre o arquivo em filename

     if (inputFile.is_open()) {
        std::cout << "Arquivo: "+filename+" foi aberto com sucesso" <<std::endl;

        std::string linha;

        while (std::getline(inputFile, linha)) {
            std::string linhaConvertida;

            for(char c:linha){
                if(std::isalpha(c)){
                    linhaConvertida += std::toupper(c);
                }else{
                    linhaConvertida += c;
                }

            }
            std::cout << linhaConvertida << std::endl;
        }
        // O arquivo foi aberto com sucesso
        // Realize as operações de leitura aqui

        inputFile.close(); // Fecha o arquivo após a leitura
    } else {
        // Houve um erro ao abrir o arquivo
        // Trate o erro adequadamente
        std::cout << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }



    return 0;
}
