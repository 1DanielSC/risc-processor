#include <iostream>
#include<map>
#include<string>
#include <vector>
#include <sstream>


std::map<std::string,int> code={
    {"AND",0},
    {"OR",1},
    {"XOR",2},
    {"NOT",3},
    {"CMP",4},
    {"SUB",5},
    {"LD",6},
    {"ST",7},
    {"J",8},
    {"JN",9},
    {"JZ",10}
};

int convertToInstruction(std::string operacao,int op1,int op2,int op3){
    int opcode=code[operacao];
    return opcode + (op1<<5) + (op2<<14) + (op3<<23);
}


//compile then run it with ./main < file.txt
int main(){
    std::string line;
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
            int instrucao = convertToInstruction(operation,operandos[0],operandos[1],operandos[2]);
        }
        else{
            //seta espaço de memoria operandos[0] para operandos[1]
            std::cout<<operandos[0]<<" "<<operandos[1]<<std::endl;
        }
    }
}