#include "systemc.h"

//OPCODES

#define STOP 0
#define JUMP_INCONDICIONAL 1
#define JUMP_CONDICIONAL_ZERO 2
#define JUMP_CONDICIONAL_NEGATIVO 3
#define LOAD_IMMEDIATE 4
#define LEITURA_MEMORIA 5
#define ESCRITA_MEMORIA 6

using namespace sc_core;

SC_MODULE(controle) {

	//CAMPOS DE INSTRUÇÃO ENVIADOS PELO DECODER
	sc_in < sc_uint<5> > opcode;
	sc_in < sc_uint<9> > op1; //fonte
	sc_in < sc_uint<9> > op2; //fonte
	sc_in < sc_uint<9> > op3; //destino

//-----------------------------------------------
	
	//SINAIS - PROGRAM COUNTER
	sc_out <bool> enable_pc;
	sc_out <bool> pc_jump;
	sc_out < sc_uint<9> > pc_jump_value;

//-----------------------------------------------
	
	//SINAIS - MEMORIA DE INSTRUCOES
	sc_out <bool> enable_mem_instrucoes;

//-----------------------------------------------
	
	//SINAIS - ULA
	sc_in <bool> zero;
	sc_in <bool> negativo;
	sc_out <bool> enable_ula;
	sc_out <bool> reset_zn;
	sc_out < sc_uint<5> > opcode_ula;

//-----------------------------------------------

	//SINAIS - BANCO DE REGISTRADORES
	sc_out <bool> enable_banco_reg;
	sc_out <bool> write_banco_reg;
	sc_out <sc_uint<2>> seletor_mux_banco_reg;

//-----------------------------------------------

	//SINAIS - MEMORIA DE DADOS

	sc_out <bool> enable_mem_dados;
	sc_out <bool> write_mem_dados;


		//O seletor para obter o OP que tiver o endereco de memoria
	sc_out <bool> seletor_mux_mem_dados; 
		//true : leitura (obter o OP1 - endereco de memoria)
		//false : escrita (obter o OP3 - endereco de memoria)
		

//-----------------------------------------------

//SINAIS - MUX BANCO DE REGISTRADORES - INSTRUÇÃO LI
	sc_out <sc_uint<9>> valor_imediato;
	sc_out <sc_uint<9>> endereco_imediato;


//SINAIS - REGISTRADORES DE PIPELINE
	sc_out <bool> pipeline_reg_enable;
	sc_out <bool> pipeline_reg_write;



	sc_in<bool> clk;

	bool pipeline_reset = false;
	int estado_atual = 0;

	SC_CTOR(controle) {

		SC_METHOD(func); 
		sensitive << clk.pos(); 
			//troca de estado é feita na borda de subida do clock
	}

	void func() {
		switch(estado_atual){

			case 0:	//fetch instrução 

				enable_pc.write(true); //incrementar
				enable_mem_instrucoes.write(true);
				estado_atual = 1;
				break;

			case 1: //obter instrução da memoria

				enable_pc.write(false); //parar de incrementar
				estado_atual = 2;
				break;

			case 2:
				if(pipeline_reset){
					estado_atual = 0;
					pipeline_reset = false;
				}
				else{
					//load nos registradores de pipeline
					pipeline_reg_enable.write(true);
					pipeline_reg_write.write(true);

					//incrementar o contador (proxima instrucao)
					enable_pc.write(true);

					estado_atual = 3;
				}

				break;


			case 3: //LEITURA DO OPCODE PARA SABER QUAL INSTRUÇÃO
				//Com a leitura do opcode, o controlador irá
					// ativar o componente do datapath responsável pela operação
				

				//colocar nova instrucao no pipeline
				enable_mem_instrucoes.write(false);
				pipeline_reg_enable.write(true);
				pipeline_reg_write.write(true);
				enable_pc.write(false);



				if(opcode.read() == JUMP_INCONDICIONAL){
					pc_jump.write(true);
					enable_pc.write(false);
					pc_jump_value.write(op3);
					estado_atual = 8;

					pipeline_reset = true; //restart pipeline
				}
				//JZ
				else if(opcode.read() == JUMP_CONDICIONAL_ZERO){

					if(zero.read()){
						pc_jump.write(true);
						pc_jump_value.write(op3);
						reset_zn.write(true); //resetar as flags Zero e Negativo
						estado_atual = 8;
						pipeline_reset = true; //restart pipeline
					}
					
				}

				//JN
				else if(opcode.read() == JUMP_CONDICIONAL_NEGATIVO){
					if(negativo.read()){
						pc_jump.write(true);
						pc_jump_value.write(op3);
						reset_zn.write(true); //resetar as flags Zero e Negativo
						estado_atual = 8;
						pipeline_reset = true; //restart pipeline
					}
				}

				else if(opcode.read() == LEITURA_MEMORIA || opcode.read() == LOAD_IMMEDIATE){
					
					enable_banco_reg.write(true);
					write_banco_reg.write(true);

					if(opcode.read() == LOAD_IMMEDIATE){
						seletor_mux_banco_reg.write(2);
						valor_imediato.write(op1);
						endereco_imediato.write(op3);

						//As 5 etapas acima ocorrem no mesmo clock
						//MUX funciona no mesmo clock assim como armazenamento de valor no banco


						estado_atual = 9;
					}

					else if(opcode.read() == LEITURA_MEMORIA){
							//1ª parte - obter o valor armazenado na memoria

						enable_mem_dados.write(true);
						write_mem_dados = false;

						//Seletor para leitura
						//O valor false vai fazer com que se pegue o
								//valor do OP1
						seletor_mux_mem_dados.write(true);


						//A memoria de dados é sensível aos sinais mandados,
							//Então, ela já estará buscando o valor requisitado


						//seletor para selecionar o valor da saida da memoria de dados
							//e salva-lo no banco de registradores

						
						seletor_mux_banco_reg.write(1); //valor para Leitura de memoria

						//Como a memoria de dados é envolvida,
								//então é necessário mais um ciclo de clock (ida para o estado 10)
						//A segunda parte (estado 10) é salvar o valor obtido na memoria


						estado_atual = 10;
					}
					
				}

				else if(opcode.read() == ESCRITA_MEMORIA){
					//ESCRITA
					//OBTER O VALOR DO OPERANDO - 1ª parte
					enable_banco_reg.write(true);
					write_banco_reg.write(false);

					//Endereco para escrita
						//O endereco de memoria ou esta no OP1
								//ou esta no OP3
					//O valor false vai fazer com que se pegue o
							//valor do OP3
					seletor_mux_mem_dados.write(false);

					estado_atual = 12;
				}
				else if(opcode.read() == STOP){
					sc_stop();
				}

				//ULA
				else if(opcode != sc_uint<5>(0)){ //OPERAÇÃO DA ULA

					//A ULA recebe tambem o OPCODE e será ativada para
						//executar a operação

					//Sinais para ativacao da ULA (executar a operacao)
					enable_banco_reg.write(true);
					write_banco_reg.write(false);

					seletor_mux_banco_reg.write(0);
					estado_atual = 6; //SALVAR RESULTADO

				}

			case 6: //WRITE ULA RESULT
				//Os sinais abaixo vai fazer com que 
						//o resultado seja armazenado
				
				enable_banco_reg.write(true);
				write_banco_reg.write(true);

				enable_mem_dados.write(false); 
	
				pipeline_reg_enable.write(false); //parar a propagação do pipeline

				estado_atual = 9; //Proximo estado
				break;

			case 8: //o PC ja leu a instrucao do jump
						//entao precisamos fechar o sinal para que 
						//ele nao fique aberto toda hora
				pc_jump.write(false);
				estado_atual = 2;
				break;


			case 9: //FECHAR OS CANAIS ABERTOS - FINALIZAÇÃO

				enable_banco_reg.write(false);
				enable_mem_dados.write(false);


				pipeline_reg_enable.write(false); //parar a propagação do pipeline

				estado_atual = 2; //ja temos a proxima instrucao do pipeline

				break;
				//stop pipeline
					//memoria de instrucoes travada


			case 10: //SALVAR RESULTADO DA LEITURA NO REGISTRADOR
				//2ª parte - salvar o valor no registrador
				//Sinais para esta execução:

				enable_banco_reg.write(true);
				write_banco_reg.write(true);

				pipeline_reg_enable.write(false); //parar a propagação do pipeline

				estado_atual = 9;
				break;


			case 12://EXECUTAR ESCRITA MEMORIA  - 2ª parte
				//Obtivemos o valor do operando na 1ª parte
					//Estado para armazená-lo na memória

				enable_mem_dados.write(true);
				write_mem_dados.write(true);


				pipeline_reg_enable.write(false); //parar a propagação do pipeline
					//Evitar a escrita de um valor antigo

				estado_atual = 9;

				break;
		
		}
	}

};