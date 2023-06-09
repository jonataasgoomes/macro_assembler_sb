#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>


void ligador(const std::vector<std::string>& nomesArquivos) {



    std::cout << "Argumento recebido: " endl;
    for (const auto& nomeArquivo : nomesArquivos) {
        std::cout << nomeArquivo << ' ';
    }
    std::cout << std::endl;


    std::cout << "ENTROU1" << std::endl;
    std::cout << "ENTROU1" << std::endl;
    std::cout << "ENTROU1" << std::endl;
    std::cout << "ENTROU1" << std::endl;
    std::cout << "ENTROU1" << std::endl;






    std::vector<int> codigoLigado;
    std::unordered_map<int, std::string> tabelaUso;
    std::unordered_map<std::string, int> tabelaDef;
    int fatorCorrecao = 0;

    std::ofstream arquivoSaida("codigo_maquina.txt"); // Abre o arquivo em modo de append

    for (const std::string& nomeArquivo : nomesArquivos) {
        std::ifstream arquivoObjeto(nomeArquivo);

        std::cout << nomeArquivo << ' ';
        std::string linha;

        if (arquivoObjeto.is_open()) {
            std::cout << "abriu arquivo com sucesso!\n";
            while (getline(arquivoObjeto, linha)) {
                std::cout << linha << ' ';
                if (linha == "USO") {
                    std::cout << "Entrou em USO sucesso!\n";
                    // Analisar a seção USO
                    while (getline(arquivoObjeto, linha) && linha != "DEF") {
                        std::stringstream ss(linha);
                        std::string simbolo;
                        int endereco;
                        ss >> simbolo >> endereco;
                        tabelaUso[endereco] = simbolo;
                    }
                } else if (linha == "DEF") {
                    std::cout << "Entrou em DEF com sucesso!\n";
                    // Analisar a seção DEF
                    while (getline(arquivoObjeto, linha) && linha != "RELATIVO") {
                        std::stringstream ss(linha);
                        std::string simbolo;
                        int valor;
                        ss >> simbolo >> valor;
                        tabelaDef[simbolo] = valor;
                    }
                } else if (linha == "RELATIVO") {
                    std::cout << "Entrou em REL com sucesso!\n";
                    // Analisar a seção RELATIVO
                    while (getline(arquivoObjeto, linha) && linha != "CODE") {
                        // Você pode implementar a lógica para a seção RELATIVO aqui, se necessário.
                    }
                } else if (linha == "CODE") {
                    std::cout << "Entrou em CODE com sucesso!\n";
                    // Analisar a seção CODE
                    while (getline(arquivoObjeto, linha)) {
                        std::stringstream ss(linha);
                        int codigo;
                        while (ss >> codigo) {
                            codigoLigado.push_back(codigo);
                        }
                    }
                }
            }
            arquivoObjeto.close();
        }
        
    }
    if (arquivoSaida.is_open()) {
        for (const int& codigo : codigoLigado) {
            std::cout << codigo << ' ';
            arquivoSaida << codigo << ' ';
        }
        std::cout << std::endl;
    } else {
        std::cout << "Erro ao criar o arquivo!\n";
    }
    arquivoSaida.close();

    for (std::vector<int>::size_type i = 0; i < codigoLigado.size(); i++) {
        int codigoAtual = codigoLigado[i];

        // Atualizar endereços relativos conforme as tabelas de uso e definição
        if (tabelaUso.count(i) != 0) {
            std::string simbolo = tabelaUso[i];
            if (tabelaDef.count(simbolo) != 0) {
                int enderecoAbsoluto = tabelaDef[simbolo] + fatorCorrecao;
                codigoAtual += enderecoAbsoluto;
            }
        }

        codigoLigado[i] = codigoAtual;
    }
}
