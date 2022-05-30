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

    
    std::cout<<std::endl;
    std::cout<< "Numero de ciclos de clock: "<< numberCycles<<std::endl;
    std::cout<<std::endl;
    
    std::cout<<"Banco de Registradores\tMemoria de Instrucoes\tMemoria de Dados" << std::endl;
    for(int i=0;i<20;i++)
        std::cout <<"["<<i<<"]: " <<PROCESSADOR.BANCO_REGISTRADORES.banco[i]<< "\t\t\t" 
            << "["<<i<<"]: " <<PROCESSADOR.MEM_INSTRUCOES.instrucoes[i]<< "\t\t\t\t"
            << "["<<i<<"]: " <<PROCESSADOR.MEM_DADOS.banco[i] << std::endl;

    cout<<endl;
    return 0;
}
