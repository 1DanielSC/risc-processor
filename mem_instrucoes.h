#include "systemc.h"


using namespace sc_core;

SC_MODULE(mem_instrucoes) {
	

	sc_signal<sc_uint<32>> * instrucoes;

	sc_in <bool> enable;
	sc_in <sc_uint<32>> endereco_instrucao;


	sc_out <sc_uint<32>> instrucao;

	sc_in<bool> clk;

	SC_CTOR(mem_instrucoes) {
		SC_METHOD(func); 
		sensitive << clk.pos(); 
		instrucoes = new sc_signal<sc_uint<32>>[512];(512);

	}

	void func() {
		if(enable.read()){
			//cout<<"instrucao "<<" "<<endereco_instrucao.read()<<" "<<instrucoes[endereco_instrucao.read()]<<endl;
			instrucao.write(instrucoes[endereco_instrucao.read()]);
		}
	}

	

};