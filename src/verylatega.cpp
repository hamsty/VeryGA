#include "verylatega.h"


VerylateGA::VerylateGA(){
    contextp = new VerilatedContext();
    top = new Vveryga(contextp,"");
}

VerylateGA::~VerylateGA(){
    top->final();
    delete top;
    delete contextp;
}

int64_t VerylateGA::get_output(){
    return top->out;
}

void VerylateGA::set_input(int64_t input){
    top->in = input;
}

void VerylateGA::inc_time(int64_t time){
    contextp->timeInc(time);
}

void VerylateGA::eval(){
    top->eval();
}

int64_t VerylateGA::get_time(){
    return contextp->time();
}

void VerylateGA::stats_print_summary(){
    contextp->statsPrintSummary();
}