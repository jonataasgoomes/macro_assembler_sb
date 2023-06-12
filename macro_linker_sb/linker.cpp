#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <unordered_map> // Esta linha já inclui unordered_multimap

using namespace std;

using namespace std;







std::vector<int> ligador(const std::vector<std::string>& nomesArquivos) {
    std::vector<int> codigoLigado;
    vector<int>fator;
    std::unordered_map<std::string, int> tabelaDef;
    std::vector<pair<std::string, int>> tabelaDefGlobal;
    std::vector<pair<std::string,int>>tabelaUsoGlobal;
    int numeroArquivo = 0;

    for (const std::string& nomeArquivo : nomesArquivos) {
        std::ifstream arquivoObjeto(nomeArquivo);
        std::string linha;
        if (arquivoObjeto.is_open()) {
            std::cout << "Arquivo: " << nomeArquivo << std::endl;
            std::string secaoAtual = "";

            while (getline(arquivoObjeto, linha)) {
                if (linha == "USO") {
                    secaoAtual = "USO";
                    fator.push_back(codigoLigado.size());
                    std::cout << "USO:" << std::endl;
                } else if (linha == "DEF") {
                    secaoAtual = "DEF";
                    
                    std::cout << "DEF:" << std::endl;
                } else if (linha == "RELATIVO") {
                    secaoAtual = "RELATIVO";
                    std::cout << "RELATIVO:" << std::endl;
                } else if (linha == "CODE") {
                    secaoAtual = "CODE";
                    std::cout << "CODE:" << std::endl;
                } else {
                    if (secaoAtual == "CODE") {
                        std::stringstream ss(linha);
                        int codigo;
                        while (ss >> codigo) {
                            codigoLigado.push_back(codigo);
                            std::cout << codigo << " ";
                        }
                        std::cout << "\n\nTAMANHO CODE: " << codigoLigado.size() <<"\n\n";
                    } else if (secaoAtual == "DEF") {
                        std::stringstream ss(linha);
                        std::string simbolo;
                        int valor;
                        ss >> simbolo >> valor;
                        tabelaDef[simbolo] = valor;
                        tabelaDefGlobal.push_back(make_pair(simbolo, valor + fator[numeroArquivo])); 
                        std::cout << simbolo << " " << valor << std::endl;



                    } else if (secaoAtual == "USO") {
                        std::stringstream ss(linha);
                        std::string simbolo;
                        int endereco;
                        ss >> simbolo >> endereco;
                        tabelaUsoGlobal.push_back(make_pair(simbolo, endereco + fator[numeroArquivo]));
                        std::cout << simbolo << " " << endereco << std::endl;



                    }
                    else if (secaoAtual == "RELATIVO") {
                        std::istringstream iss(linha);
                        int valor;
                        while (iss >> valor) {
                            std::cout << valor << " ";
                        }
                        std::cout << std::endl;
                    } 
                }
            }
            std::cout << "\n";
            numeroArquivo++;
            arquivoObjeto.close();
        }
        std::cout << "\n";
    }

    std::cout << "Tabela de DEF global:" << std::endl;
    for (const auto& def : tabelaDefGlobal) {
        std::cout << def.first << " " << def.second << std::endl;
    }
    std::cout << "\n";

    std::cout << "Tabela de USO global:" << std::endl;
    for (const auto& uso : tabelaUsoGlobal) {
        std::cout << uso.first << " " << uso.second << std::endl;
    }
    std::cout << "\n";


    for (const auto& uso : tabelaUsoGlobal) {
    int indice = uso.second;
    std::string simbolo = uso.first;

    for (const auto& def : tabelaDefGlobal) {
        if (def.first == simbolo) {
            int valor = def.second;
            codigoLigado[indice] = valor;
            break;
        }
    }
}





        std::cout << "\nResultado CODE:\n";
        for (const auto& codigo : codigoLigado) {
            std::cout << codigo << " ";
        }
        std::cout << std::endl;



    return codigoLigado;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> nomesArquivos;

    if (argc > 1 && argc <= 5) {
        for (int i = 1; i < argc; i++) {
            std::string nomeArquivo = argv[i];
            if (nomeArquivo.substr(nomeArquivo.find_last_of(".") + 1) == "obj") {
                nomesArquivos.push_back(nomeArquivo);
            } else {
                std::cout << "Erro: Arquivo '" << nomeArquivo << "' não tem a extensão .obj!\n";
                return 1;
            }
        }
    } else {
        std::cout << "Erro: Forneça até 4 nomes de arquivos .obj como argumentos!\n";
        return 1;
    }

    std::vector<int> codigoLigado = ligador(nomesArquivos);

    return 0;
}
