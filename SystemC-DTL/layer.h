#include <systemc.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include "neuron.h"
#include "buffer.h"
#include "interLayerChannel.h"


template<int numOfInputs, int numOfNeurons>
SC_MODULE(spatialLayer) {
    
    sc_vector<sc_in<sc_int<8>>>  layerInputs{ "layerInputs", numOfInputs };
    sc_vector<sc_out<sc_int<8>>> layerResults{ "layerResults", numOfNeurons };

    sc_vector<sc_vector<sc_in<sc_int<8>>>> layerWeights{ "layerWeights", numOfNeurons };
    sc_vector<sc_vector<sc_signal<sc_int<8>>>> bufferOutputs{ "bufferOutputs", numOfNeurons };

    sc_event layerStart;
    sc_event layerDone;
    sc_event_and_list neuronDoneEvents, bufferDoneEvents;

    buffer<numOfInputs>* buffer1[numOfNeurons];
    spatialNeuron<numOfInputs>* neuron1[numOfNeurons];

    SC_CTOR(spatialLayer) {

        // Second dimension for weights 1
        for (int i = 0; i < numOfNeurons; ++i) {
            layerWeights[i].init(numOfInputs); 
        }

        //buffer layer 1
        for (int i = 0; i < numOfNeurons; ++i) {
            bufferOutputs[i].init(numOfInputs); 
        }

        for (int j = 0; j < numOfNeurons; ++j) {
            buffer1[j] = new buffer<numOfInputs>(("buffer1_" + std::to_string((j))).c_str());
            for (int i = 0; i < numOfInputs; ++i) {
                buffer1[j]->bufferInputs[i](layerWeights[j][i]);
                buffer1[j]->bufferOutputs[i](bufferOutputs[j][i]);
            }
        }

        //Neuron Layer 1
        for (int j = 0; j < numOfNeurons; ++j) {
            neuron1[j] = new spatialNeuron<numOfInputs>(("neuron1_" + std::to_string((j))).c_str());
            for (int i = 0; i < numOfInputs; ++i) {
                neuron1[j]->inputs[i](layerInputs[i]);
                neuron1[j]->weights[i](bufferOutputs[j][i]);
            }
            neuron1[j]->result[0](layerResults[j]);
        }

        SC_THREAD(layerFunc);
        sensitive << layerStart;
    }
    void layerFunc();
};

template<int numOfInputs, int numOfNeurons>
void spatialLayer<numOfInputs, numOfNeurons> :: layerFunc() {
   
    while (true) {

        // Buffer events
        for (int i = 0; i < numOfNeurons; ++i) {
            buffer1[i]->bufferStart.notify(SC_ZERO_TIME);
        }

        // Wait for all bufferDone events to occur
        for (int i = 0; i < numOfNeurons; ++i) {
            bufferDoneEvents &= buffer1[i]->bufferDone;
        }
        wait(bufferDoneEvents);

        // Neuron events
        for (int i = 0; i < numOfNeurons; ++i) {
            neuron1[i]->neuronStart.notify(SC_ZERO_TIME);
        }

        // Wait for all neuronDone events to occur
        for (int i = 0; i < numOfNeurons; ++i) {
            neuronDoneEvents &= neuron1[i]->neuronDone;
        }
        wait(neuronDoneEvents);

        layerDone.notify();
        wait();
    }
}