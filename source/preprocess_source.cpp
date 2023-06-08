#include "preprocess_source.h"
void processarArquivo(const std::string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return;
    }

    string linha;
    string nomeArquivoProcessado = nomeArquivo.substr(0, nomeArquivo.find_last_of('.')) + "_processado.asm";
    ofstream arquivoProcessado(nomeArquivoProcessado);

    if (!arquivoProcessado.is_open()) {
        cout << "Erro ao criar o arquivo processado." << endl;
        return;
    }

    string rotulo;
    bool rotuloEncontrado = false;

    while (std::getline(arquivo, linha)) {
        // Remover espaços e tabulações repetidos
        string linhaSemEspacos;
        bool espacoAnterior = false;

        for (char c : linha) {
            if (isspace(c)) {
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
        if (linhaSemEspacos.find_first_not_of(' ') == string::npos) {
            continue; // Ignorar linha vazia e passar para a próxima
        }

        // Encontrar a posição do ponto e vírgula (;)
        size_t posPontoEVirgula = linhaSemEspacos.find(';');

        // Transformar a linha para maiúsculas
        for (char& c : linhaSemEspacos) {
            c = std::toupper(c);
        }

        // Remover tudo após o ponto e vírgula (;)
        if (posPontoEVirgula != string::npos) {
            linhaSemEspacos = linhaSemEspacos.substr(0, posPontoEVirgula);
        }

        // Remover espaços em branco finais
        size_t ultimoNaoEspaco = linhaSemEspacos.find_last_not_of(' ');
        if (ultimoNaoEspaco != string::npos) {
            linhaSemEspacos.erase(ultimoNaoEspaco + 1);
        } else {
            linhaSemEspacos.clear();
        }

        // Checar se encontramos um rótulo
        size_t pos = linhaSemEspacos.find(':');
        if (pos != string::npos && pos == linhaSemEspacos.size() - 1) {
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
    cout << "Arquivo processado salvo como: " << nomeArquivoProcessado << endl;
    arquivo.close();
}