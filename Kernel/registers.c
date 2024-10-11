#include <registers.h>
#include <videoDriver.h>

extern uint64_t regs[];
extern uint8_t regsSaved;

int getRegisters(uint64_t registers[]){
    if(!regsSaved){
        return 0;
    }

    for(int i = 0; i < 18; i++){
        registers[i] = regs[i];
    }

    return 1;
}