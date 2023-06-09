#include "preprocess_source.h"
#include "ligador.h"


int main(int argc, char* argv[]) {

    std::vector<std::string> nomesArquivos;
    std::string arquivoObj;


    if (argc < 2 || argc > 5) {
        cout << "Uso: ./montador <nome_arquivo> <nome_arquivo> <nome_arquivo> <nome_arquivo>" << endl;
        return 1;
    }


    for (int i = 1; i < argc; i++) {
        cout << "Processando arquivo: " << argv[i] << endl;
        std::cout << std::endl;
        
        // Verifica se o arquivo não possui extensão
        string arquivo = argv[i];
        if (arquivo.find(".") == string::npos) {
            arquivo += ".asm"; // Adiciona a extensão .asm
        }
        
        processarArquivo(arquivo);
        
        if (argc == 2)
        {
            cout << "Montando arquivo: " <<arquivo<< endl;
            std::cout << std::endl;
            string arquivo_processado = arquivo.substr(0, arquivo.find_last_of(".")) + "_processado.asm";
            montador(arquivo_processado);
            std::cout << std::endl;
        }
        else if(argc >= 2 || argc < 5)
        {   
            cout << "Montado arquivos: " << arquivo << endl;
            string arquivo_processado = arquivo.substr(0, arquivo.find_last_of(".")) + "_processado.asm";
            montador(arquivo_processado);


            std::cout << std::endl;
            string arquivoObj = arquivo.substr(0, arquivo.find_last_of(".")) + ".obj";
            cout<< "NOME DO ARQUIVO: " << arquivoObj << endl;
            std::cout << std::endl;
            nomesArquivos.push_back(arquivoObj);

        }
        else
        {
            cout << "Numero de arquivos incorreto" << endl;
        }




    }


    for (const std::string& nomeArquivo : nomesArquivos) {
        std::cout << nomeArquivo << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
