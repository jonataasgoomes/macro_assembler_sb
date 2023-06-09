#include "preprocess_source.h"

void processarArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cout << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return;
    }

    std::string linha;
    std::string nomeArquivoProcessado = nomeArquivo.substr(0, nomeArquivo.find_last_of('.')) + "_processado.asm";
    std::ofstream arquivoProcessado(nomeArquivoProcessado);

    if (!arquivoProcessado.is_open()) {
        std::cout << "Erro ao criar o arquivo processado." << std::endl;
        return;
    }

    std::string rotulo;
    bool rotuloEncontrado = false;

    while (std::getline(arquivo, linha)) {
        // Remover a vírgula do token "COPY"
        size_t posVirgula = linha.find(',');
        if (posVirgula != std::string::npos) {
            linha.erase(posVirgula, 1);
        }

        // Remover espaços e tabulações repetidos
        std::string linhaSemEspacos;
        bool espacoAnterior = false;

        for (char c : linha) {
            if (std::isspace(c)) {
                if (!espacoAnterior) {
                    linhaSemEspacos += ' ';
                    espacoAnterior = true;
                }
            } else {
                linhaSemEspacos += c;
                espacoAnterior = false;
            }
        }

        // Verificar se a linha está vazia (contém apenas espaços em branco)
        if (linhaSemEspacos.find_first_not_of(' ') == std::string::npos) {
            continue; // Ignorar linha vazia e passar para a próxima
        }

        // Encontrar a posição do ponto e vírgula (;)
        size_t posPontoEVirgula = linhaSemEspacos.find(';');

        // Transformar a linha para maiúsculas
        for (char& c : linhaSemEspacos) {
            c = std::toupper(c);
        }

        // Remover tudo após o ponto e vírgula (;)
        if (posPontoEVirgula != std::string::npos) {
            linhaSemEspacos = linhaSemEspacos.substr(0, posPontoEVirgula);
        }

        // Remover espaços em branco finais
        size_t ultimoNaoEspaco = linhaSemEspacos.find_last_not_of(' ');
        if (ultimoNaoEspaco != std::string::npos) {
            linhaSemEspacos.erase(ultimoNaoEspaco + 1);
        } else {
            linhaSemEspacos.clear();
        }

        // Checar se encontramos um rótulo
        size_t pos = linhaSemEspacos.find(':');
        if (pos != std::string::npos && pos == linhaSemEspacos.size() - 1) {
            rotulo = linhaSemEspacos;
            rotuloEncontrado = true;
            continue;
        }

        // Se encontramos um rótulo na linha anterior, concatenamos
        if (rotuloEncontrado) {
            linhaSemEspacos = rotulo + " " + linhaSemEspacos;  // Adicionar espaço após o rótulo
            rotuloEncontrado = false;
        }

        // Escrever a linha transformada e truncada no arquivo processado
        arquivoProcessado << linhaSemEspacos << std::endl;
    }

    arquivoProcessado.close();
    std::cout << "Arquivo processado salvo como: " << nomeArquivoProcessado << std::endl;
    arquivo.close();
}
