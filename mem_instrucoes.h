#include "systemc.h"


using namespace sc_core;

SC_MODULE(mem_instrucoes) {


	std::vector<sc_uint<32>> instrucoes;

	sc_in <bool> enable;
	sc_in <sc_uint<32>> endereco_instrucao;


	sc_out <sc_uint<32>> instrucao;

	sc_in<bool> clk;

	SC_CTOR(mem_instrucoes) {

		SC_METHOD(func); 
		sensitive << clk.pos(); 
		instrucoes = std::vector<sc_uint<32>>(512);

	}

	void func() {
		if(enable.read()){

			instrucao.write(instrucoes[endereco_instrucao.read()]);
		}
	}

	

};