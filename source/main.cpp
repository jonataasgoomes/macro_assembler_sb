#include <preprocess_source.h>

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 5) {
        cout << "Uso: ./montador <nome_arquivo> <nome_arquivo> <nome_arquivo> <nome_arquivo>" << endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        cout << "Processando arquivo: " << argv[i] << endl;
        
        // Verifica se o arquivo n�o possui extens�o
        string arquivo = argv[i];
        if (arquivo.find(".") == string::npos) {
            arquivo += ".asm"; // Adiciona a extens�o .asm
        }
        
        processarArquivo(arquivo);
    }

    return 0;
}
