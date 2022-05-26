#include "systemc.h"
using namespace sc_core;




SC_MODULE(pipeline_reg){

	sc_in <bool> enable;
	sc_in <bool> write;

	sc_in <sc_uint<5>> opcode_entrada;
	sc_in <sc_uint<9>> op1_entrada;
	sc_in <sc_uint<9>> op2_entrada;
	sc_in <sc_uint<9>> op3_entrada;

//indo para o banco de registradores
	sc_out <sc_uint<5>> opcode_saida;
	sc_out <sc_uint<9>> op1_saida;
	sc_out <sc_uint<9>> op2_saida;
	sc_out <sc_uint<9>> op3_saida;


	sc_in <bool> clk;

	SC_CTOR(pipeline_reg){
		SC_METHOD(func);
		sensitive << clk.pos();
	}

	int op,fonte1,fonte2,destino;

	void func(){
		if(enable.read()){

			if(write.read()){


				op = opcode_entrada.read();
				fonte1 = op1_entrada.read();
				fonte2 = op2_entrada.read();
				destino = op3_entrada.read();

				opcode_saida.write(op);
				op1_saida.write(fonte1);
				op2_saida.write(fonte2);
				op3_saida.write(destino);

			}else{
				opcode_saida.write(op);
				op1_saida.write(fonte1);
				op2_saida.write(fonte2);
				op3_saida.write(destino);

			}

		}
	}

	
};