#include <systemc.h>
#include "processador.h"






//compile then run it with ./main < file.txt
int sc_main(int argc, char* argv[]) {
    
    processador PROCESSADOR("PROC");
    sc_signal<bool> clock;
    PROCESSADOR.sinal_clock(clock);
    
    
    
    int numberCycles=0;
	
    while (not sc_end_of_simulation_invoked()) {
		clock = 0;
        sc_start(1, SC_NS);

		clock = 1;
		sc_start(1, SC_NS);
		numberCycles++;
	}
    std::cout<< "Numero de ciclos: "<< numberCycles<<std::endl;
    std::cout<< "Banco de Registradores: "<<std::endl;
    for(int i=0;i<20;i++){
        std::cout<<PROCESSADOR.BANCO_REGISTRADORES.banco[i]<<" ";
    }
    cout<<endl;
    std::cout<< "Memoria de Instrucoes: "<<std::endl;
    for(int i=0;i<20;i++){
        std::cout<<PROCESSADOR.MEM_INSTRUCOES.instrucoes[i]<<" ";
    }
    cout<<endl;
    std::cout<< "Memoria de dados: "<<std::endl;
    for(int i=0;i<20;i++){
        std::cout<<PROCESSADOR.MEM_DADOS.banco[i]<<" ";
    }
    cout<<endl;
    return 0;
}
