#include "systemc.h"
#include <vector>

using namespace sc_core;

SC_MODULE(banco_registradores) {


	sc_signal<sc_int<32>> * banco;


	sc_in <bool> enable;
	sc_in <bool> write;
	sc_in < sc_uint<9> > op1;
	sc_in < sc_uint<9> > op2;
	sc_in < sc_uint<9> > op3;

	sc_in <sc_int<32>> valor;


	sc_in<bool> clk;

	sc_out < sc_int<32> > valor_op1;
	sc_out < sc_int<32> > valor_op2;


	SC_CTOR(banco_registradores) {

		SC_METHOD(func); 
		sensitive << clk.pos();
		banco = new sc_signal<sc_int<32>>[512];

	}

	void func(){
		if(enable.read()){
			cout<<"ENTREI"<<endl;
			if(write.read()){
				cout<<"ENTREI NO WRITE "<<op3.read()<<" "<<valor.read()<<endl;
				banco[op3.read()]=(valor.read());
				
			}
			else{
				valor_op1.write(banco[op1.read()]);
				valor_op2.write(banco[op2.read()]);
			}
		}
	}

};
