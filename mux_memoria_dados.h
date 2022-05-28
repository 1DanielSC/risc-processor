#include "systemc.h"

using namespace sc_core;

SC_MODULE(mux_memoria_dados) {


	sc_in <bool> seletor;
	sc_in < sc_uint<9> > op1;
	sc_in < sc_uint<9> > op3;


	sc_out <sc_uint<9>> endereco_saida;





	SC_CTOR(mux_memoria_dados) {

		SC_METHOD(func); 
		sensitive << seletor << op1 << op3; 
	}

	void func() {
		//LEITURA DE MEMORIA
		if(seletor.read() == true) {
			endereco_saida.write(op1.read());
		}
			
		
		//ESCRITA NA MEMORIA
		else{
			endereco_saida.write(op3.read());
		}
			
		
	}

};