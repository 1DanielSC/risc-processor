#include "systemc.h"

using namespace sc_core;

SC_MODULE(pc) {

	sc_uint<8> count;

	sc_in <bool> pc_jump;
	sc_in <bool> enable;//increment
	sc_in <sc_uint<9>> instrucao_jump;

	sc_in <bool> clk;

	sc_out< sc_uint<32> > endereco_instrucao;



	SC_CTOR(pc) {

		SC_METHOD(func); 
		sensitive << clk.pos(); 
		count = 0;
	}

	void func() {

		if(enable.read()){
			count++;
			endereco_instrucao.write(count);
		}
		else if(pc_jump.read()){
			count = instrucao_jump.read();
			endereco_instrucao.write(count);
		}

	}

};