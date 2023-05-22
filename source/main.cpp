
#include <preprocess_source.h>

int main(int argc, char* argv[])

{
    if (argc <2 || argc >5){
        cout <<"Usar: ./montador <nome_arquivo> <nome_arquivo> <nome_arquivo> <nome_arquivo>"<< std::endl;
        return 1;
    }
    for (int i = 1; i < argc; i++){
        cout << "Processando arquivo:" <<argv[i] << endl;
        processarArquivo(argv[i]);
        cout << "Arquivo processado:" <<argv[i]<<"_processado" << endl;
    }


    return 0;
}
