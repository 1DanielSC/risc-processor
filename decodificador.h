#include "systemc.h"

using namespace sc_core;
SC_MODULE(decodificador) {


	sc_in<sc_uint<32>> instrucao;
	sc_in<bool> clk;
	sc_in<bool> enable;

	sc_out < sc_uint<5> > opcode;
	sc_out <sc_uint<9>> op1;
	sc_out <sc_uint<9>> op2;
	sc_out <sc_uint<9>> op3;



	SC_CTOR(decodificador) {

		SC_METHOD(func); 
		sensitive << clk.pos();
		//sensitive << instrucao; 
	}

	void func() {
		if(enable.read()){
			decodificar_inteiro(instrucao.read());
		}
		
	}

	//tuple<int,int,int,int> decodificar_inteiro(int x)
	void decodificar_inteiro(sc_uint<32> x){
		sc_uint<5> p1=x%(1<<5);
		x/=(1<<5);
	
		sc_uint<9> p2=x%(1<<9);
		x/=(1<<9);

		sc_uint<9> p3=x%(1<<9);
		x/=(1<<9);
		
		sc_uint<9> p4=x%(1<<9);
		
		opcode.write(p1);
		op1.write(p2);
		op2.write(p3);
		op3.write(p4);
		//return make_tuple(p1,p2,p3,p4);
	}

};