#include <preprocess_source.h>

void processarArquivo(const std::string& nomeArquivo) {
  std::ifstream arquivo(nomeArquivo);

  if (arquivo.is_open()) {
    std::string linha;
    std::string nomeArquivoProcessado = nomeArquivo + "_processado";
    std::ofstream arquivoProcessado(nomeArquivoProcessado);

    if (arquivoProcessado.is_open()) {
      std::string rotulo;
      bool concatenarLinha = false;

      while (std::getline(arquivo, linha)) {
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

        // Remover espaços antes do caractere ':'
        size_t pos = linhaSemEspacos.find(':');
        if (pos != std::string::npos) {
          size_t espacosAntesDoRotulo = linhaSemEspacos.find_last_not_of(' ', pos - 1);
          if (espacosAntesDoRotulo != std::string::npos) {
            linhaSemEspacos.erase(espacosAntesDoRotulo + 1, pos - espacosAntesDoRotulo - 1);
          }

          // Verificar se a próxima linha deve ser concatenada
          concatenarLinha = true;
          rotulo = linhaSemEspacos;
          continue;
        }

        if (concatenarLinha) {
          // Concatenar linha atual com a anterior (rótulo)
          linhaSemEspacos = rotulo + linhaSemEspacos;
          concatenarLinha = false;
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

        // Escrever a linha transformada e truncada no arquivo processado
        arquivoProcessado << linhaSemEspacos << std::endl;
      }

      arquivoProcessado.close();
      std::cout << "Arquivo processado salvo como: " << nomeArquivoProcessado << std::endl;
    } else {
      std::cout << "Erro ao criar o arquivo processado." << std::endl;
    }

    arquivo.close();
  } else {
    std::cout << "Erro ao abrir o arquivo." << std::endl;
  }
}