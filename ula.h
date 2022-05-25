#include "systemc.h"

using namespace sc_core;

SC_MODULE(ula) {

	sc_in< sc_uint<5> > opcode;
	sc_in< sc_uint<32> > operando1;
	sc_in< sc_uint<32> > operando2;

	sc_in <bool> reset_zn;


	sc_out< bool > zero;
	sc_out< bool > negativo;

	sc_out<sc_uint<32>> resultado;

	SC_CTOR(ula) {

		SC_METHOD(computar); 
		sensitive << opcode << operando1 
			<< operando2 << reset_zn;
	}

	void computar() {

		if(reset_zn.read() == true){
			zero.write(false);
			negativo.write(false);
		}


		switch (opcode.read()) {

			//ADI��O
		case 6:
			resultado.write(operando1.read() + operando2.read());
			break;

			//SUBTRA��O
		case 7:
			resultado.write(operando1.read() - operando2.read());
			break;

			//XOR
		case 8: 
			resultado.write(operando1.read() ^ operando2.read());
			break;

			//AND
		case 9:
			resultado.write(operando1.read() & operando2.read());
			break;

			//OR
		case 10:
			resultado.write(operando1.read() | operando2.read());
			break;

			//NOT
		case 11:
			resultado.write(~ operando1.read());
			break;

			//CMP
		case 12: 
			zero.write(operando1.read() == operando2.read());
			negativo.write(operando1.read() < operando2.read());
			break;


		default:
			resultado.write(0);
		}


/*
		if (this->resultado == 0) {
				this->zero = 1;
				this->negativo = 0;
		}
		else if (resultado > 0) {
			this->zero = 0;
			this->negativo = 0;
		}
		else {
			this->zero = 0;
			this->negativo = 1;
		}

	}
*/
};