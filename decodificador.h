#include "systemc.h"

using namespace sc_core;
SC_MODULE(decodificador) {


	sc_in<sc_uint<32>> instrucao;


	sc_out < sc_uint<5> > opcode;
	sc_out <sc_uint<9>> op1;
	sc_out <sc_uint<9>> op2;
	sc_out <sc_uint<9>> op3;



	SC_CTOR(decodificador) {

		SC_METHOD(func); 
		sensitive << instrucao; 
	}

	void func() {
		
		//int instrucao = instrucoes[instrucao_entrada.read()];
		//tie(opcode,op1,op2,endereco)=decodificar_inteiro(instrucao);
		decodificar_inteiro(instrucao.read());
		
	}

	//tuple<int,int,int,int> decodificar_inteiro(int x)
	void decodificar_inteiro(int x){
		int p1=x%(1<<5);

		opcode.write(p1);

		x/=(1<<5);
		int p2=x%(1<<9);

		op1.write(p2);

		x/=(1<<9);
		int p3=x%(1<<9);
		op2.write(p3);
		x/=(1<<9);
		int p4=x%(1<<9);
		op3.write(p4);
		//return make_tuple(p1,p2,p3,p4);
	}

};