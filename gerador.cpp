#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include<vector>
#include<map>
#include <chrono>    


#define registerSize 512

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
int randint(int a, int b){return std::uniform_int_distribution<int>(a, b)(rng);}


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

std::vector<std::string> possibleOperations = {"AND","ADD","LI","OR","XOR","NOT","CMP","SUB","LD","ST","J","JN","JZ"};

std::string genInt(){
    return std::to_string(randint(0,registerSize-1));
}

std::string genInstruction(){
    std::string retorno="";
    std::string operation = possibleOperations[randint(0,possibleOperations.size()-1)];
    retorno+=operation;
    retorno+=" ";
    if(code[operation]<=5){
        for(int i=0;i<3;i++){
            retorno+=genInt();
            retorno+=" ";
        }
    }
    else if (code[operation]<=7){
        for(int i=0;i<2;i++){
            retorno+=genInt();
            retorno+=" ";
        }
    }
    else{
        retorno+=genInt();
        retorno+=" ";
    }
    return retorno;
}
std::string setMem(){
    return "memset "+std::to_string(randint(0,registerSize-1))+" "+std::to_string(randint(0,1000));
}
std::ofstream testCase;
void genRandomTestCase(int size) {
  
  
  for(int i=0;i<size;i++){
      testCase<<genInstruction()<<std::endl;
  }
  
}

void setMemoria(int size){
    for(int i=0;i<size;i++){
      testCase<<setMem()<<std::endl;
  }
}
int main(){
    std::string filename="testeCase.txt";
    testCase.open (filename);
    genRandomTestCase(20);
    setMemoria(20);
    testCase.close();
}
