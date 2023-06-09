#include "preprocess_source.h"


    struct Simbolo {
        std::string simbolo;
        int valor;
        bool definido;
        std::vector<int> lista;
    };

    Simbolo criarSimbolo(std::string simbolo, int valor, bool definido, std::vector<int> lista) {
        Simbolo novoSimbolo;
        novoSimbolo.simbolo = simbolo;
        novoSimbolo.valor = valor;
        novoSimbolo.definido = definido;
        novoSimbolo.lista = lista;
        return novoSimbolo;
    }

    struct Instrucao {
        std::string opSimb;
        std::string opN;
        int sizeWord;
    };


    void genMem(std::vector<string>& mem, const std::string& valor){
            // 12 29 10 29 4 28 11 30 3 28 11 31 10 29 2 31 11 31 13 31 9 30 29 10 29 7 4 14 2 0 0 0 
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


    void montador(const std::string& nomeArquivo) {
        std::vector<Simbolo> listaDeSimbolos;
        std::vector<string> memoria;
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
            {"SPACE", "0", 1},
            {"CONST", "", 1}
        };

        std::ifstream file(nomeArquivo);

        if (!file) {
            std::cerr << "Erro ao abrir o arquivo." << std::endl;
            return;
        }

        std::string line;
        int contadorEnd = 0;  // Inicializa o contadorEnd de endereços
        int contadorLinhas = 0;  // Inicializa o contador de linhas
        std::cout << std::endl;
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
                        std::cout << std::endl;
                        // Ignorar a linha com a instrução "SECTION"
                        continue;
                    }


                    if (tokens.size() >= 2)
                    {
                        if (tokens[1] == "CONST"){

                        genMem(memoria,tokens[2]);


                        tokens[0].pop_back();

                        
                        for (auto& simbolo : listaDeSimbolos) { //procura o rotulo na lista de simbolos
                            if (simbolo.simbolo == tokens[0]) {// se encontrar o simbolo igual o rotulo entra no if
                                simbolo.valor = contadorEnd;
                                simbolo.definido = true;
                               
                                break;
                            }
                        }
                        contadorEnd++;
                        continue;

                        }
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
                        if (tokens.size() >= 3){
                            operando2 = tokens[2];
                        }

                        }
                    }
                    


/*                          cout << std::endl;
                            cout<<"Contador: "<<contadorEnd<<endl;
                            cout<<"rotulo: "<<rotulo<<endl;
                            cout<<"Operador: "<<operador<< endl;
                            cout<<"Operando1: "<<operando<< endl;
                            cout<<"Operando2: "<<operando2 << endl;
                            cout << std::endl;*/




                    if (!operador.empty()) {// existe operador ?
                        bool found = false;
                        for (const auto& instrucao : instrucoes) {//procura o operador na lista de instruções
                            if (instrucao.opSimb == operador) {
                                genMem(memoria,instrucao.opN);
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            std::cerr << "Erro na linha " << contadorLinhas << ": Operador inválido - Erro:." << std::endl;
                            continue;
                        }
                    }


                    if (!rotulo.empty()) {// rotulo não está vazio
                        rotulo.pop_back();// retira o : do rotulo

                        bool rotuloEncontrado = false;  
                        
                        for (auto& simbolo : listaDeSimbolos) { //procura o rotulo na lista de simbolos
                            if (simbolo.simbolo == rotulo) {// se encontrar o simbolo igual o rotulo entra no if
                                simbolo.valor = contadorEnd;
                                simbolo.definido = true;
                                rotuloEncontrado = true;
                                break;
                            }
                        }
                        

                            if (!rotuloEncontrado) {//rotulo ainda não definido ? 
                                Simbolo novoSimbolo = criarSimbolo(rotulo, contadorEnd, true, {-1});
                                listaDeSimbolos.push_back(novoSimbolo);
                            }




                    }

                    if (!operando.empty()) {//operando não está vazio

                        // Verifica se o operando já existe na lista de símbolos

                        bool operandoEncontrado = false;

                        for (auto& simbolo : listaDeSimbolos) {                                     //procura o operando na lista de simbolos
                            if (simbolo.simbolo == operando) {                                      //operando encontrado
                                if (!simbolo.definido) {//simbolo não definido ?
                                    simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 1);   //adiciona o valor do contador na lista de simbolos
                                    genMem(memoria,"-"); //gera instrução de maquina  
                                }else{
                                    genMem(memoria,to_string(simbolo.valor));
                                }
                                operandoEncontrado = true;
                                
                            }
                        }


                        for (auto& simbolo : listaDeSimbolos) {                                     // procura o operando na lista de simbolos
                            if (simbolo.simbolo == operando2) {                                     //operando 2 existe
                                if (!simbolo.definido) {                                            // simbolo definido == false
                                    simbolo.lista.insert(simbolo.lista.begin(), contadorEnd + 2);
                                    genMem(memoria,"-"); //gera instrução de maquina
                                }
                                operandoEncontrado = true;
                                break;
                            }
                        }

                        if (!operandoEncontrado) {// operando não encontrado
                            // Adiciona um novo símbolo à lista de símbolos
                            Simbolo novoSimbolo = criarSimbolo(operando, 0, false, {contadorEnd + 1, -1}); //cria o simbolo
                            listaDeSimbolos.push_back(novoSimbolo); //adiciona na tabela de simbolo
                            genMem(memoria,"-"); //gera instrução de maquina
                            if (operador == "COPY"){//se a instrução for COPY ela tera um segundo operando
                                Simbolo novoSimbolo = criarSimbolo(operando2, 0, false, {contadorEnd + 2, -1});// cria o simbolo do 2 operando
                                listaDeSimbolos.push_back(novoSimbolo);//adiciona o segundo operando
                                genMem(memoria,"-");//gera instrução de maquina
                            }

                        }
                    }


                    

                    // Atualiza o contadorEnd de acordo com a lógica de incremento adequada
                    if (operador == "STOP") {
                        std::cout << std::endl;
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

            for (auto& simbolo : listaDeSimbolos) {
                attRef(memoria, simbolo.valor, simbolo.lista);
            }
            std::cout << std::endl;

            for (std::vector<int>::size_type i = 0; i < memoria.size(); i++) {
                std::cout << memoria[i] << " ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
            

        std::cout << std::left << std::setw(8) << "Simbolo"
                  << std::setw(8) << "Valor"
                  << std::setw(10) << "Def"
                  << "Lista" << std::endl;
                  std::cout << std::endl;
                  

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
        std::cout << std::endl;
    }


