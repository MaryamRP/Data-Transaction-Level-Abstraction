#include <systemc.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

template<int numOfInputs>
SC_MODULE(spatialNeuron) {
    sc_vector<sc_in<sc_int<8>>> inputs{ "inputs", numOfInputs };
    sc_vector<sc_in<sc_int<8>>> weights{ "weights", numOfInputs };

    sc_vector<sc_out<sc_int<8>>> result{ "result", 1 };
  
    sc_event neuronDone;
    sc_event neuronStart;
   
    SC_CTOR(spatialNeuron) {

        SC_THREAD(neuronController);
        sensitive << neuronStart;
    }
    void neuronController();
};
template<int numOfInputs>
void spatialNeuron<numOfInputs>::neuronController()
{
    int temp = 0;
    while (true) {

        int temp = 0;
        for (int i = 0; i <numOfInputs; i++) {
            temp += inputs[i].read() * weights[i].read();
        }

        // Apply ReLU activation function
        if (temp < 0) {
            temp = 0;
        }

        result[0].write(temp);

        neuronDone.notify();
        wait();
    }
}








