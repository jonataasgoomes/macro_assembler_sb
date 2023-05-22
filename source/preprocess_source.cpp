#include <preprocess_source.h>

void processarArquivo(const std::string& arquivo) {
    std::ifstream arquivo_original(arquivo);
    if (!arquivo_original.is_open()) {
        // Imprime uma mensagem de erro se não for possível abrir o arquivo
        std::cout << "Erro ao abrir o arquivo: " << arquivo << " Use ./montador <nome_arquivo>" << std::endl;
        return;
    }

    std::ofstream arquivo_processado(arquivo + "_processado");

    std::string linha;

    while (std::getline(arquivo_original, linha)) {
        size_t pos = linha.find(';');
        if (pos != std::string::npos) {
            linha = linha.substr(0, pos);
        }

        for (char& c : linha) {
            c = std::toupper(static_cast<unsigned char>(c));
        }

        std::string linha_limpa;
        bool espaco_anterior = false;

        for (char c : linha) {
            if (c == ' ' || c == '\t') {
                if (!espaco_anterior) {
                    linha_limpa += ' ';
                }
                espaco_anterior = true;
            } else {
                linha_limpa += c;
                espaco_anterior = false;
            }
        }

        size_t pos_dois_pontos = linha_limpa.find(':');
        if (pos_dois_pontos != std::string::npos) {
            size_t pos_anterior = pos_dois_pontos - 1;
            while (pos_anterior > 0 && (linha_limpa[pos_anterior] == ' ' || linha_limpa[pos_anterior] == '\t')) {
                linha_limpa.erase(pos_anterior, 1);
                pos_anterior--;
            }
        }

        if (!linha_limpa.empty()) {
            arquivo_processado << linha_limpa << std::endl;
        }
    }

    arquivo_original.close();
    arquivo_processado.close();
}