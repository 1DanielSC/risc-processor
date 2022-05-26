#include <systemc.h>
#include "processador.h"
#include <iostream>
#include<map>
#include<string>
#include <vector>
#include <sstream>


std::map<std::string,int> code={
    {"J",1},
    {"JZ",2},
    {"JN",3},
    {"LI",4},
    {"LD",5},
    {"ST",6},
    {"ADD",7},
    {"SUB",8},
    {"XOR",9},
    {"AND",10},
    {"OR",11},
    {"NOT",12},
    {"CMP",13},
};

int convertToInstruction(std::string operacao,int op1,int op2,int op3){
    int opcode=code[operacao];
    return opcode + (op1<<5) + (op2<<14) + (op3<<23);
}


//compile then run it with ./main < file.txt
int sc_main(int argc, char* argv[]) {
    
    processador PROCESSADOR("PROC");
    std::string line;
    int id=0;

    while(std::getline (std::cin,line)){
        std::stringstream ss(line); 
        std::string operation;
        ss>>operation;

        std::vector<int> operandos;
        int valor;
        while(ss>>valor){
            operandos.push_back(valor);
        }

        while(operandos.size()<3){
            operandos.push_back(0);
        }
        if(operation!="memset"){
            sc_uint<32> instrucao = convertToInstruction(operation,operandos[0],operandos[1],operandos[2]);
            PROCESSADOR.MEM_INSTRUCOES.instrucoes[id++]=instrucao;
        }
        else{
            int posicao = operandos[0];
            sc_uint<32> valor = operandos[1];
            PROCESSADOR.MEM_DADOS.banco[posicao]=valor;
        }
        
    }
    std::cout<<PROCESSADOR.MEM_DADOS.banco[4]<<std::endl;

    return 0;
}
