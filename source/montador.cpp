#include "preprocess_source.h"
#include "asm_lib.h"

struct Simbolo {
    std::string simbolo;
    int valor;
    bool definido;
    std::vector<int> lista;
    bool externo;
};

Simbolo criarSimbolo(std::string simbolo, int valor, bool definido, std::vector<int> lista, bool externo) {
    Simbolo novoSimbolo;
    novoSimbolo.simbolo = simbolo;
    novoSimbolo.valor = valor;
    novoSimbolo.definido = definido;
    novoSimbolo.lista = lista;
    novoSimbolo.externo = externo;
    return novoSimbolo;
}

struct Definicao {
    std::string simbolo;
    int valor;
};

Definicao criarDefinicao(std::string simbolo, int valor) {
    Definicao novaDefinicao;
    novaDefinicao.simbolo = simbolo;
    novaDefinicao.valor = valor;
    return novaDefinicao;
}

struct Uso {
    std::string simbolo;
    int valor;
};

Uso criarUso(std::string simbolo, int valor) {
    Uso novoUso;
    novoUso.simbolo = simbolo;
    novoUso.valor = valor;
    return novoUso;
}

struct Instrucao {
    std::string opSimb;
    std::string opN;
    int sizeWord;
};

struct Diretiva {
    std::string nome;
    int operando;
};

void genMem(std::vector<std::string>& mem, const std::string& valor) {
    mem.push_back(valor);
}

void attRef(std::vector<std::string>& mem, int valor, const std::vector<int>& lista) {
    for (int i : lista) {
        if (i == -1) {
            break;
        }

        if (i < static_cast<int>(mem.size())) {
            mem[i] = std::to_string(valor);
        } else {
            std::cout << "Erro: Referência inválida." << std::endl;
        }
    }
}

void attDef(std::vector<Simbolo>& listaDeSimbolos, std::vector<Definicao>& listaDeDef) {
    for (const auto& simbolo : listaDeSimbolos) {
        for (auto& definicao : listaDeDef) {
            if (definicao.simbolo == simbolo.simbolo) {
                definicao.valor = simbolo.valor;
                break;
            }
        }
    }
}

void anotarUso(std::vector<Uso>& tabelaDeUso, const std::string& simbolo, int posicao) {
    Uso novoUso = criarUso(simbolo, posicao);
    tabelaDeUso.push_back(novoUso);
}

void montador(const std::string& nomeArquivo) {
    std::vector<Simbolo> listaDeSimbolos;
    std::vector<Definicao> listaDeDef;
    std::vector<Uso> listaDeUso;
    std::vector<int> relativos;
    std::vector<std::string> memoria;
    std::vector<Instrucao> instrucoes = {
        {"ADD", "1", 2},
        {"SUB", "2", 2},
        {"MUL", "3", 2},
        {"DIV", "4", 2},
        {"JMP", "5", 2},
        {"JMPN", "6", 2},
        {"JMPP", "7", 2},
        {"JMPZ", "8", 2},
        {"COPY", "9", 3},
        {"LOAD", "10", 2},
        {"STORE", "11", 2},
        {"INPUT", "12", 2},
        {"OUTPUT", "13", 2},
        {"STOP", "14", 1},
    };

    std::vector<Diretiva> diretivas = {
        {"CONST", 1},
        {"SPACE", 1},
        {"EXTERN", 1},
        {"PUBLIC", 0},
        {"BEGIN", 0},
        {"END", 0},
        {"SECTION", 1}
    };

    std::ifstream file(nomeArquivo);

    if (!file) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return;
    }

    std::string line;
    int contadorEnd = 0;
    int contadorLinhas = 0;
    bool sectionTextFound = false;
    while (std::getline(file, line)) {
        contadorLinhas++;
        if (!line.empty()) {
            std::istringstream iss(line);
            std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                            std::istream_iterator<std::string>{}};
            //std::cout << "(" << contadorLinhas << ") " << contadorEnd << ": " << line << std::endl;

            std::string rotulo;
            std::string operador;
            std::string operando;
            std::string operando2;

            if (!tokens.empty()) {
                if (tokens[0] == "SECTION") {
                    if (tokens[1] == "TEXT") {
                        sectionTextFound = true;
                    }
                    continue;
                }

                if (tokens.size() >= 2) {
                    if (tokens[1] == "CONST") {
                        if (tokens[2].substr(0, 2) == "0X") {
                            tokens[2] = std::to_string(hexaParaDecimal(tokens[2]));
                            genMem(memoria, tokens[2]);
                        } else {
                            genMem(memoria, tokens[2]);
                        }

                        tokens[0].pop_back();

                        for (auto& simbolo : listaDeSimbolos) {
                            if (simbolo.simbolo == tokens[0]) {
                                simbolo.valor = contadorEnd;
                                simbolo.definido = true;
                                break;
                            }
                        }
                        contadorEnd++;
                        continue;
                    }
                }

                if (!tokens[0].empty() && tokens[0].back() == ':') {
                    rotulo = tokens[0];
                    if (tokens.size() > 1) {
                        operador = tokens[1];
                    }
                    if (tokens.size() > 2) {
                        operando = tokens[2];
                    }
                } else {
                    operador = tokens[0];
                    if (tokens.size() > 1) {
                        operando = tokens[1];
                        if (tokens.size() >= 3) {
                            operando2 = tokens[2];
                        }
                    }
                }

                if (!rotulo.empty()) {
                    rotulo.pop_back();
                }

                bool found = false;
                for (const auto& instrucao : instrucoes) {
                    if (instrucao.opSimb == operador) {
                        genMem(memoria, instrucao.opN);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    for (const auto& diretiva : diretivas) {
                        if (diretiva.nome == rotulo) {
                            found = true;
                            break;
                        } else if (diretiva.nome == operador) {
                            if (operador == "SPACE") {
                                if (!operando.empty()) {
                                    genMem(memoria, operando);
                                    found = true;
                                } else {
                                    genMem(memoria, "00");
                                    found = true;
                                }
                                continue;
                            }
                            if (operador == "BEGIN") {
                                found = true;
                                continue;
                            }
                            found = true;
                            break;
                        }
                    }
                }

                if (!found) {
                    std::cerr << "Erro na linha " << contadorLinhas << ": Operador inválido." << std::endl;
                    continue;
                }

                if (!rotulo.empty()) {
                    bool rotuloEncontrado = false;

                    for (auto& simbolo : listaDeSimbolos) {
                        if (simbolo.simbolo == rotulo) {
                            simbolo.valor = contadorEnd;
                            simbolo.definido = true;
                            rotuloEncontrado = true;
                            break;
                        }
                    }

                    if (!rotuloEncontrado) {
                        if (rotulo == "EXTERN") {
                            Simbolo novoSimbolo = criarSimbolo(operador, 0, true, {-1}, true);
                            listaDeSimbolos.push_back(novoSimbolo);
                        } else {
                            Simbolo novoSimbolo = criarSimbolo(rotulo, contadorEnd, true, {-1}, false);
                            listaDeSimbolos.push_back(novoSimbolo);
                        }
                    }
                }

                if (!operando.empty()) {

                    bool operandoEncontrado = false;
                    for (auto& simbolo : listaDeSimbolos) {

                        if (simbolo.simbolo == operando) {
                            if (!simbolo.definido) {
                                simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 1);
                                genMem(memoria, "-");
                                relativos.push_back(contadorEnd + 1);

                            } else {
                                if (simbolo.externo) {
                                    anotarUso(listaDeUso, operando, contadorEnd + 1);
                                }
                                

                                if (operador == "PUBLIC") {
                                    Definicao novaDefinicao = criarDefinicao(operando, 0);
                                    listaDeDef.push_back(novaDefinicao);
                                } else {
                                    genMem(memoria, std::to_string(simbolo.valor));
                                    
                                }
                                relativos.push_back(contadorEnd + 1);
                            }
                            operandoEncontrado = true;
                        }

                    }


                    for (auto& simbolo : listaDeSimbolos) {
                        if (simbolo.simbolo == operando2) {
                            if (!simbolo.definido) {
                                simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 2);
                                genMem(memoria, "-");
                            }
                            
                            operandoEncontrado = true;
                            break;
                        }
                    }

                    if (!operandoEncontrado) {
                        Simbolo novoSimbolo = criarSimbolo(operando, 0, false, {contadorEnd + 1, -1}, false);
                        listaDeSimbolos.push_back(novoSimbolo);

                        if (operador == "PUBLIC") {
                            Definicao novaDefinicao = criarDefinicao(operando, 0);
                            listaDeDef.push_back(novaDefinicao);
                        } else {
                            genMem(memoria, "-");
                        }

                        if (operador == "COPY") {
                            Simbolo novoSimbolo = criarSimbolo(operando2, 0, false, {contadorEnd + 2, -1}, false);
                            listaDeSimbolos.push_back(novoSimbolo);
                            genMem(memoria, "-");
                        }
                    }

                    
                }

                

                if (operador == "BEGIN" || rotulo == "EXTERN" || operador == "PUBLIC") {
                    contadorEnd = 0;
                } else if (operador == "STOP") {
                    contadorEnd += 1;
                } else if (operador == "COPY") {
                    contadorEnd += 3;
                } else if (operador == "CONST" || operador == "SPACE") {
                    if (operador == "SPACE" && (!operando.empty())) {
                        contadorEnd += std::stoi(operando);
                    } else {
                        contadorEnd += 1;
                    }
                } else {
                    contadorEnd += 2;
                }
            }
        }
    }

    file.close();

    if (!sectionTextFound) {
        std::cerr << "ERRO SEMANTICO SECTION TEXT NAO ENCONTRADO" << std::endl;
    }

    for (auto& simbolo : listaDeSimbolos) {
        attRef(memoria, simbolo.valor, simbolo.lista);
    }

    attDef(listaDeSimbolos, listaDeDef);

   std::cout << std::endl;

    /*std::cout << std::left << std::setw(10) << "Simbolo"
              << std::setw(10) << "Valor"
              << std::setw(12) << "Def"
              << std::setw(10) << "Externo"
              << "Lista" << std::endl;
    std::cout << std::endl;

    for (const auto& simbolo : listaDeSimbolos) {
        std::cout << std::setw(10) << simbolo.simbolo
                  << " " << std::setw(8) << simbolo.valor
                  << " " << std::setw(12) << (simbolo.definido ? "true" : "false")
                  << " " << std::setw(8) << (simbolo.externo ? "true" : "false")
                  << " ";

        for (const auto& num : simbolo.lista) {
            std::cout << num << " ";
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;*/

    std::cout << "USO" << std::endl;
    for (const auto& uso : listaDeUso) {
        std::cout << uso.simbolo << " " << uso.valor << std::endl;
    }

    std::cout << "DEF" << std::endl;
    for (const auto& definicao : listaDeDef) {
        std::cout << definicao.simbolo << " " << definicao.valor << std::endl;
    }

    std::cout << "RELATIVOS" << std::endl;
    for (std::vector<int>::size_type i = 0; i < relativos.size(); i++) {
        std::cout << relativos[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "CODE" << std::endl;
    for (std::vector<int>::size_type i = 0; i < memoria.size(); i++) {
        std::cout << memoria[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;



    // Escrever o arquivo objeto com a extensão .obj
    std::size_t found = nomeArquivo.find("_processado.asm");
    std::string nomeArquivoObj;
    if (found != std::string::npos) {
        nomeArquivoObj = nomeArquivo.substr(0, found) + ".obj";
    } else {
        nomeArquivoObj = nomeArquivo + ".obj";
    }
    std::ofstream arquivoObj(nomeArquivoObj);

    if (!arquivoObj) {
        std::cerr << "Erro ao criar o arquivo objeto." << std::endl;
        return;
    }

    arquivoObj << "USO" << std::endl;
    for (const auto& uso : listaDeUso) {
        arquivoObj << uso.simbolo << " " << uso.valor << std::endl;
    }

    arquivoObj << "DEF" << std::endl;
    for (const auto& definicao : listaDeDef) {
        arquivoObj << definicao.simbolo << " " << definicao.valor << std::endl;
    }

    arquivoObj << "RELATIVO" << std::endl;
    std::sort(relativos.begin(), relativos.end());
    relativos.erase(std::unique(relativos.begin(), relativos.end()), relativos.end());

    for (std::vector<int>::size_type i = 0; i < relativos.size(); i++) {
        arquivoObj << relativos[i] << " ";
    }
    arquivoObj << std::endl;

    arquivoObj << "CODE" << std::endl;
    for (std::vector<int>::size_type i = 0; i < memoria.size(); i++) {
        arquivoObj << memoria[i] << " ";
    }
    arquivoObj << std::endl;

    arquivoObj.close();
}