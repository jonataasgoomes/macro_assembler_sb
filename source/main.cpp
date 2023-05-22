
#include <preprocess_source.h>

int main(int argc, char* argv[])

{
    if (argc <2 || argc >5){
        std::cout <<"Usar: ./montador <nome_arquivo> <nome_arquivo> <nome_arquivo> <nome_arquivo>"<< std::endl;
        return 1;
    }
    for (int i = 1; i < argc; i++){
        std::cout << "Processando arquivo:" <<argv[i] << std::endl;
        processarArquivo(argv[i]);
        std::cout << "Arquivo processado:" <<argv[i]<<"_processado" << std::endl;
    }


    return 0;
}
