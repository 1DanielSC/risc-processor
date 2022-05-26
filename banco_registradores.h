#include "systemc.h"
#include <vector>

using namespace sc_core;

SC_MODULE(banco_registradores) {


	std::vector<sc_uint<32>> banco;

	sc_in <bool> enable;
	sc_in <bool> write;
	sc_in < sc_uint<9> > op1;
	sc_in < sc_uint<9> > op2;
	sc_in < sc_uint<9> > op3;

	sc_in <sc_uint<32>> valor;


	sc_in<bool> clk;

	sc_out < sc_uint<32> > valor_op1;
	sc_out < sc_uint<32> > valor_op2;


	SC_CTOR(banco_registradores) {

		SC_METHOD(func); 
		sensitive << clk.pos();
		banco = std::vector<sc_uint<32>>(512);
	}

	void func(){
		if(enable.read()){

			if(write.read()){
				banco[op3.read()]=(valor.read());
			
			}
			else{
				valor_op1.write(banco[op1.read()]);
				valor_op2.write(banco[op2.read()]);
			}
		}
	}

};
