#include "systemc.h"

using namespace sc_core;

SC_MODULE(mux_banco_registradores) {

//Ha tres tipos de valores a serem salvos no banco de registradores:
	//Resultado da ULA
	//Leitura de memória
	//Valor imediato


	sc_in <sc_uint<2>> seletor;


	sc_in < sc_int<32> > resultado_ula;
	sc_in < sc_uint<32> > valor_memoria;
	sc_in < sc_uint<9> > imediato;




//ENTRADA NO BANCO DE REGISTRADORES
	sc_out <sc_int<32>> valor_saida;


	SC_CTOR(mux_banco_registradores) {

		SC_METHOD(func); 
		sensitive << seletor << resultado_ula << valor_memoria << imediato ; 
	}

	void func() {
		
		if(seletor.read() == 0) { //resultado da ULA
			valor_saida.write(resultado_ula.read());
		}
		
		else if(seletor.read() == 1){ //valor vindo da memoria
			valor_saida.write(valor_memoria.read());
		}
		else{ //valor imediato

			valor_saida.write(imediato.read());
		}
		
	}

};