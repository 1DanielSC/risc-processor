#include "systemc.h"
#include <vector>

using namespace sc_core;

SC_MODULE(memoria_dados) {


	std::vector<sc_uint<32>> banco;

	sc_in <bool> enable;
	sc_in <bool> write;
	sc_in < sc_uint<9> > endereco;
	sc_in < sc_uint<32> > valor_entrada;


	sc_out <sc_uint<32>> valor_saida;


	sc_in<bool> clk;


	SC_CTOR(memoria_dados) {

		SC_METHOD(func); 
		sensitive << clk.pos(); 
		banco = std::vector<sc_uint<32>>(512);
	}

	void func() {
		if(enable.read()){
			
			if(write.read())
				banco[endereco.read()]=(valor_entrada.read());
			else{
				valor_saida.write(banco[endereco.read()]);
			}
		}
	}

};
