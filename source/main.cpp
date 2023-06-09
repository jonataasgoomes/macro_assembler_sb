#include "preprocess_source.h"


int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 5) {
        cout << "Uso: ./montador <nome_arquivo> <nome_arquivo> <nome_arquivo> <nome_arquivo>" << endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        cout << "Processando arquivo: " << argv[i] << endl;
        
        // Verifica se o arquivo não possui extensão
        string arquivo = argv[i];
        if (arquivo.find(".") == string::npos) {
            arquivo += ".asm"; // Adiciona a extensão .asm
        }
        
        processarArquivo(arquivo);
        
        if (argc == 2)
        {
            cout << "Montando arquivo direto: " <<arquivo<< endl;
            string arquivo_processado = arquivo.substr(0, arquivo.find_last_of(".")) + "_processado.asm";
            montador(arquivo_processado);
            cout << "Arquivo: "<<arquivo<<" Montado"<<endl;
        }
        else if(argc >= 2 || argc < 5)
        {   
            cout << "Montando o arquivo:" <<arquivo<<" para o ligador:"<< endl;
            string arquivo_processado = arquivo.substr(0, arquivo.find_last_of(".")) + "_processado.asm";
            montador(arquivo_processado);
        }
        else
        {
            cout << "Numero de arquivos incorreto" << endl;
        }

    }

    
    
    return 0;
}
