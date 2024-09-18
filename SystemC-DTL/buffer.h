#include <systemc.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

template<int size>
SC_MODULE(buffer) {
    sc_vector<sc_in<sc_int<8>>> bufferInputs{ "inputs", size };
    sc_vector<sc_out<sc_int<8>>> bufferOutputs{ "weights", size };
    
    sc_event bufferDone;
    sc_event bufferStart;

    SC_CTOR(buffer) {

        SC_THREAD(bufferFunc);
        sensitive << bufferStart;
    }
    void bufferFunc();
};

template<int size>
void buffer<size>::bufferFunc()
{ 
    while (true) {

        for (int i = 0; i < size; i++) {
            bufferOutputs[i].write(bufferInputs[i].read());
        }

        bufferDone.notify();
        wait();
    }
}