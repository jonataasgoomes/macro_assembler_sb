    #include "preprocess_source.h"

    struct Simbolo {
        std::string simbolo;
        int valor;
        bool definido;
        std::vector<int> lista;
    };

    struct Instrucao {
        std::string opSimb;
        std::string opN;
        int sizeWord;
    };

    Simbolo criarSimbolo(std::string simbolo, int valor, bool definido, std::vector<int> lista) {
        Simbolo novoSimbolo;
        novoSimbolo.simbolo = simbolo;
        novoSimbolo.valor = valor;
        novoSimbolo.definido = definido;
        novoSimbolo.lista = lista;
        return novoSimbolo;
    }

    void montador(const std::string& nomeArquivo) {
        std::vector<Simbolo> listaDeSimbolos;

        std::vector<Instrucao> instrucoes = {
            {"ADD", "01", 2},
            {"SUB", "02", 2},
            {"MUL", "03", 2},
            {"DIV", "04", 2},
            {"JMP", "05", 2},
            {"JMPN", "06", 2},
            {"JMPP", "07", 2},
            {"JMPZ", "08", 2},
            {"COPY", "09", 3},
            {"LOAD", "10", 2},
            {"STORE", "11", 2},
            {"INPUT", "12", 2},
            {"OUTPUT", "13", 2},
            {"STOP", "14", 1},
            {"SPACE", "00", 1},
            {"CONST", " ", 1}
        };

        std::ifstream file(nomeArquivo);

        if (!file) {
            std::cerr << "Erro ao abrir o arquivo." << std::endl;
            return;
        }

        std::string line;
        int contadorEnd = 0;  // Inicializa o contadorEnd de endereços
        int contadorLinhas = 0;  // Inicializa o contador de linhas

        while (std::getline(file, line)) {
            contadorLinhas++;
            if (!line.empty()) {
                std::istringstream iss(line);
                std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                                std::istream_iterator<std::string>{}};
                std::cout << "(" << contadorLinhas << ") " << contadorEnd << ": " << line << std::endl;                               
                

                std::string rotulo;
                std::string operador;
                std::string operando;
                std::string operando2;

                if (!tokens.empty()) {
                    if (tokens[0] == "SECTION") {
                        // Ignorar a linha com a instrução "SECTION"
                        continue;
                    }








                    // Verifica se o primeiro elemento termina com ":" e o trata como rótulo
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
                        }
                    }
                    // Se operador é "CONST", ajusta operando e rotulo
                        if (operador == "CONST") {
                            operando = rotulo;
                            operando.pop_back();
                        }



                    if (!operador.empty()) {
                        bool found = false;
                        for (const auto& instrucao : instrucoes) {
                            if (instrucao.opSimb == operador) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            std::cerr << "Erro na linha " << contadorLinhas << ": Operador inválido." << std::endl;
                            // Continua para a próxima linha mesmo com erro
                            continue;
                        }
                    }

                    if (!rotulo.empty()) {
                        rotulo.pop_back();
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
                            Simbolo novoSimbolo = criarSimbolo(rotulo, contadorEnd, true, {-1});
                            listaDeSimbolos.push_back(novoSimbolo);
                        }
                    }


                    // Tratamento especial para a instrução "COPY"
                    if (operador == "COPY") {
                        cout << tokens[1] << " " << tokens[3] <<endl;
                        if (tokens.size() >= 3) {
                            operando = tokens[1];
                            operando2 = tokens[3];

                            // Adicione o operando à lista de símbolos, se necessário
                            bool operandoEncontrado = false;
                            for (auto& simbolo : listaDeSimbolos) {
                                if (simbolo.simbolo == operando) {
                                    if (!simbolo.definido) {
                                        simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 1);
                                    }
                                    operandoEncontrado = true;
                                    break;
                                }
                            }

                            if (!operandoEncontrado) {
                                Simbolo novoSimbolo = criarSimbolo(operando, 0, false, {contadorEnd + 1, -1});
                                listaDeSimbolos.push_back(novoSimbolo);
                            }

                            // Adicione o operando2 à lista de símbolos, se necessário
                            bool operando2Encontrado = false;
                            for (auto& simbolo : listaDeSimbolos) {
                                if (simbolo.simbolo == operando2) {
                                    if (!simbolo.definido) {
                                        simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 2);
                                    }
                                    operando2Encontrado = true;
                                    break;
                                }
                            }

                            if (!operando2Encontrado) {
                                Simbolo novoSimbolo = criarSimbolo(operando2, 0, false, {contadorEnd + 2, -1});
                                listaDeSimbolos.push_back(novoSimbolo);
                            }
                        } else {
                            std::cerr << "Erro na linha " << contadorLinhas << ": Instrução COPY incompleta." << std::endl;
                            // Continua para a próxima linha mesmo com erro
                            continue;
                        }
                    }








                    
                    if (!operando.empty()) {
                        // Verifica se o operando já existe na lista de símbolos
                        bool operandoEncontrado = false;
                        for (auto& simbolo : listaDeSimbolos) {
                            if (simbolo.simbolo == operando) {
                                // Atualiza apenas o valor do contador de endereços + 1 na cabeça da lista
                                if (!simbolo.definido) {
                                    simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 1);
                                }
                                operandoEncontrado = true;
                                break;
                            }
                        }

                        if (!operandoEncontrado) {
                            // Adiciona um novo símbolo à lista de símbolos
                            Simbolo novoSimbolo = criarSimbolo(operando, 0, false, {contadorEnd + 1, -1});
                            listaDeSimbolos.push_back(novoSimbolo);

                        }
                    }
                    

                    // Atualiza o contadorEnd de acordo com a lógica de incremento adequada
                    if (operador == "STOP") {
                        contadorEnd += 1;
                    } else if (operador == "COPY") {
                        contadorEnd += 3;
                    } else if (operador == "CONST" || operador == "SPACE") {
                        contadorEnd += 1;
                    } else {
                        contadorEnd += 2;
                    }
                }
            }
        }

        file.close();

        std::cout << std::left << std::setw(8) << "Simbolo"
                  << std::setw(8) << "Valor"
                  << std::setw(10) << "Def"
                  << "Lista" << std::endl;

        for (const auto& simbolo : listaDeSimbolos) {
            std::cout << std::setw(8) << simbolo.simbolo
                      << " " << std::setw(8) << simbolo.valor
                      << " " << std::setw(10) << (simbolo.definido ? "true" : "false")
                      << " ";

            for (const auto& num : simbolo.lista) {
                std::cout << num << " ";
            }

            std::cout << std::endl;
        }
    }