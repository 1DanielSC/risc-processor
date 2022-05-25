#include "systemc.h"
#include "decodificador.h"
#include "pc.h"
#include "pipeline_reg.h"
#include "controle.h"
#include "ula.h"
#include "mem_instrucoes.h"
#include "mux_mem_dados.h"
#include "mux_banco_registradores.h"
#include "mux_mem_dados.h"
#include "banco_registradores.h"

SC_MODULE(processador){

	pc PC;

	decodificador DECODIFICADOR;

	ula ULA;

	controle CONTROLE;

	mem_instrucoes MEM_INSTRUCOES;
	mem_dados MEM_DADOS;

	pipeline_reg PIPELINE_REG;

	banco_registradores BANCO_REGISTRADORES;

	mux_mem_dados MUX_MEM_DADOS;
	mux_banco_registradores MUX_BANCO_REGISTRADORES;



	sc_in<bool> sinal_clock;

//FLAGS
	sc_signal<bool> sinal_zero;
	sc_signal<bool> sinal_negativo;
	sc_signal<bool> sinal_reset_zn;

//ENABLES
	sc_signal<bool> sinal_enable_pc;
	sc_signal<bool> sinal_enable_mem_dados;
	sc_signal<bool> sinal_enable_mem_instrucoes;
	sc_signal<bool> sinal_enable_banco_reg;
	sc_signal<bool> sinal_enable_ula;
	sc_signal<bool> sinal_pipeline_reg_enable;

//WRITE
	sc_signal<bool> sinal_write_mem_dados;
	sc_signal<bool> sinal_write_banco_reg;
	sc_signal<bool> sinal_pipeline_reg_write;

//MUX - MEMORIA DE DADOS
	sc_signal<bool> sinal_seletor_mux_mem_dados;
	sc_signal <sc_uint<9>> sinal_mux_endereco_saida;


//BANCO DE REGISTRADORES
	sc_signal <sc_uint<32>> sinal_op1_valor;
	sc_signal <sc_uint<32>> sinal_op2_valor;


//PC
	sc_signal<bool> sinal_pc_jump;
	sc_signal< sc_uint<9> > sinal_instrucao_jump;
	sc_signal< sc_uint<32> > sinal_endereco_instrucao; //PC - MEM INSTRUCOES


	sc_signal<sc_uint<9>> sinal_pc_jump_value; //op3?
	sc_signal< sc_uint<32> > sinal_instrucao;


//MUX - BANCO DE REGISTRADORES
	sc_signal<sc_uint<2>> sinal_seletor_mux_banco_reg;
	sc_signal <sc_uint<32>> sinal_saida_memoria_dados;
	sc_signal <sc_uint<9>> sinal_valor_imediato;

	sc_signal <sc_uint<32>> sinal_mux_dados_valor_saida;


//ULA
	sc_signal< sc_uint<32> > sinal_ula_resultado;

//DECODER E REGISTRADOR PIPELINE
	sc_signal<sc_uint<5>> sinal_opcode_pipe; 
	sc_signal<sc_uint<9>> sinal_op1_pipe;
	sc_signal<sc_uint<9>> sinal_op2_pipe;
	sc_signal<sc_uint<9>> sinal_op3_pipe;

//REGISTRADOR PIPELINE E CONTROLE/ULA
	sc_signal<sc_uint<5>> sinal_opcode; 
	sc_signal<sc_uint<9>> sinal_op1;
	sc_signal<sc_uint<9>> sinal_op2;
	sc_signal<sc_uint<9>> sinal_op3;

	

	CTOR(processador){
		PC.pc_jump(sinal_pc_jump);
		PC.enable(sinal_enable_pc);
		PC.clk(sinal_clock);
		PC.instrucao_jump(sinal_instrucao_jump);
		PC.endereco_instrucao(sinal_endereco_instrucao);


		MEM_INSTRUCOES.endereco_instrucao(sinal_endereco_instrucao);
		MEM_INSTRUCOES.enable(sinal_enable_mem_instrucoes);	
		MEM_INSTRUCOES.instrucao(sinal_instrucao);
		MEM_INSTRUCOES.clk(sinal_clock);



		DECODIFICADOR.instrucao(sinal_instrucao);
		DECODIFICADOR.opcode(sinal_opcode_pipe);
		DECODIFICADOR.op1(sinal_op1_pipe);
		DECODIFICADOR.op2(sinal_op2_pipe);
		DECODIFICADOR.op3(sinal_op3_pipe);


		PIPELINE_REG.opcode_entrada(sinal_opcode_pipe);
		PIPELINE_REG.op1_entrada(sinal_op1_pipe);	
		PIPELINE_REG.op2_entrada(sinal_op2_pipe);	
		PIPELINE_REG.op3_entrada(sinal_op3_pipe);

		PIPELINE_REG.opcode_saida(sinal_opcode);
		PIPELINE_REG.op1_saida(sinal_op1);	
		PIPELINE_REG.op2_saida(sinal_op2);	
		PIPELINE_REG.op3_saida(sinal_op3);

		PIPELINE_REG.clk(sinal_clock);
		PIPELINE_REG.write(sinal_pipeline_reg_write);
		PIPELINE_REG.enable(sinal_pipeline_reg_enable);




		CONTROLE.clk(sinal_clock);

		CONTROLE.enable_mem_instrucoes(sinal_enable_mem_instrucoes);
		CONTROLE.enable_mem_dados(sinal_enable_mem_dados);
		CONTROLE.enable_banco_reg(sinal_enable_banco_reg);
		CONTROLE.enable_pc(sinal_enable_pc);
		CONTROLE.enable_ula(sinal_enable_ula);

		CONTROLE.opcode(sinal_opcode);
		CONTROLE.op1(sinal_op1);
		CONTROLE.op2(sinal_op2);
		CONTROLE.op3(sinal_op3);

		CONTROLE.opcode_ula(sinal_opcode);

		CONTROLE.zero(sinal_zero);
		CONTROLE.negativo(sinal_negativo);
		CONTROLE.reset_zn(sinal_reset_zn);
		
		CONTROLE.pc_jump(sinal_pc_jump);
		CONTROLE.pc_jump_value(sinal_pc_jump_value);


		CONTROLE.pipeline_reg_write(sinal_pipeline_reg_write);
		CONTROLE.pipeline_reg_enable(sinal_pipeline_reg_enable);

		CONTROLE.seletor_mux_mem_dados(sinal_seletor_mux_mem_dados);
		CONTROLE.seletor_mux_banco_reg(sinal_seletor_mux_banco_reg);

		CONTROLE.write_banco_reg(sinal_write_banco_reg);
		CONTROLE.write_mem_dados(sinal_write_mem_dados);

		CONTROLE.endereco_imediato(sinal_op3);
		CONTROLE.valor_imediato(sinal_op1);



		ULA.opcode(sinal_opcode);
		ULA.op1(sinal_op1_valor);
		ULA.op2(sinal_op2_valor);
		ULA.resultado(sinal_ula_resultado);
		ULA.zero(sinal_zero);
		ULA.negativo(sinal_negativo);
		ULA.reset_zn(sinal_reset_zn);



		MUX_MEM_DADOS.seletor(sinal_seletor_mux_mem_dados);



		MUX_BANCO_REGISTRADORES.seletor(sinal_seletor_mux_banco_reg);
		MUX_BANCO_REGISTRADORES.resultado_ula(sinal_ula_resultado);
		MUX_BANCO_REGISTRADORES.valor_memoria(sinal_saida_memoria_dados);
		MUX_BANCO_REGISTRADORES.imediato(sinal_valor_imediato);
		MUX_BANCO_REGISTRADORES.valor_saida(sinal_mux_dados_valor_saida);



		MUX_MEM_DADOS.seletor(sinal_seletor_mux_mem_dados);
		MUX_MEM_DADOS.op1(sinal_op1);
		MUX_MEM_DADOS.op3(sinal_op3);
		MUX_MEM_DADOS.endereco_saida(sinal_mux_endereco_saida);



		MEM_DADOS.clk(sinal_clock);
		MEM_DADOS.enable(sinal_enable_mem_dados);
		MEM_DADOS.write(sinal_write_mem_dados);
		MEM_DADOS.endereco_entrada(sinal_mux_endereco_saida);
		MEM_DADOS.valor_saida(sinal_saida_memoria_dados);
		MEM_DADOS.valor_entrada(sinal_op1_valor);




		BANCO_REGISTRADORES.clk(sinal_clock);
		BANCO_REGISTRADORES.enable(sinal_enable_banco_reg);
		BANCO_REGISTRADORES.write(sinal_write_banco_reg);
		BANCO_REGISTRADORES.op1(sinal_op1);
		BANCO_REGISTRADORES.op2(sinal_op2);
		BANCO_REGISTRADORES.op3(sinal_op3);

		BANCO_REGISTRADORES.valor(sinal_mux_dados_valor_saida);

		BANCO_REGISTRADORES.valor_op1(sinal_op1_valor);
		BANCO_REGISTRADORES.valor_op2(sinal_op2_valor);

	}

};
