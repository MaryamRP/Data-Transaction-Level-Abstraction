#include <systemc.h>
#include "layer.h"


const int numOfInputs1 = 4;
const int numOfNeurons1 = 2;
const int numOfInputs2 = 2;
const int numOfNeurons2 = 5;

SC_MODULE(partitionNetwork)
{
    // Define signals and variables
    sc_vector<sc_signal<sc_int<8>>> tempLayerInputs{ "tempLayerInputs", numOfInputs1 };
    sc_vector<sc_signal<sc_int<8>>> layerInputs1{ "layerInputs1", numOfInputs1 };
    sc_vector<sc_signal<sc_int<8>>> layerInputs2{ "layerInputs2", numOfInputs2 };
    sc_vector<sc_vector<sc_signal<sc_int<8>>>> layerWeights1{ "layerWeights1", numOfNeurons1 };
    sc_vector<sc_vector<sc_signal<sc_int<8>>>> layerWeights2{ "layerWeights2", numOfNeurons2 };
    sc_vector<sc_signal<sc_int<8>>> layerResults1{ "layerResults1", numOfNeurons1 };
    sc_vector<sc_signal<sc_int<8>>> layerResults2{ "layerResults2", numOfNeurons2 };
    
    sc_int<8>* tempInput1;
    sc_int<8>* tempInput2;
    bool checkGet1, checkGet2;
    bool checkPut1, checkPut2;

    // Define instantiation
    spatialLayer<numOfInputs1, numOfNeurons1>* layer1;
    spatialLayer<numOfInputs2, numOfNeurons2>* layer2;

    interLayerChannel<sc_int<8>, sc_int<8 * numOfInputs1>, numOfInputs1, numOfNeurons1>* ILC1;
    interLayerChannel<sc_int<8>, sc_int<8 * numOfInputs2>, numOfInputs2, numOfNeurons2>* ILC2;

	SC_CTOR(partitionNetwork)
	{
        ILC1 = new interLayerChannel<sc_int<8>, sc_int<8 * numOfInputs1>, numOfInputs1, numOfNeurons1>;
        ILC2 = new interLayerChannel<sc_int<8>, sc_int<8 * numOfInputs2>, numOfInputs2, numOfNeurons2>;

        for (int i = 0; i < numOfNeurons1; ++i) {
            layerWeights1[i].init(numOfInputs1);
        }
        for (int i = 0; i < numOfNeurons2; ++i) {
            layerWeights2[i].init(numOfInputs2);
        }

        // Spatial layer 1
        layer1 = new spatialLayer<numOfInputs1, numOfNeurons1 >("Layer_Instance1");
        for (int i = 0; i < numOfInputs1; i++) {
            layer1->layerInputs[i](layerInputs1[i]);
        }
        for (int j = 0; j < numOfNeurons1; j++) {

            for (int i = 0; i < numOfInputs1; i++) {
                layer1->layerWeights[j][i](layerWeights1[j][i]);
            }           
            layer1->layerResults[j](layerResults1[j]);
        }

        // Spatial layer 2
        layer2 = new spatialLayer<numOfInputs2, numOfNeurons2>("Layer_Instance2");
        for (int i = 0; i < numOfInputs2; i++) {
            layer2->layerInputs[i](layerInputs2[i]);
        }
        for (int j = 0; j < numOfNeurons2; j++) {

            for (int i = 0; i < numOfInputs2; i++) {
                layer2->layerWeights[j][i](layerWeights2[j][i]);
            }
            layer2->layerResults[j](layerResults2[j]);
        }

		SC_THREAD(layer_ev);

	}
	void layer_ev();
};