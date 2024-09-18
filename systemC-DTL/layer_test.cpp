#include <systemc.h>
#include "layer_test.h"

void partitionNetwork::layer_ev()
{
    while (true) {

        for (int j = 0; j < numOfNeurons1; j++) {
            for (int i = 0; i < numOfInputs1; ++i) {
                layerWeights1[j][i].write(i * j + 1); 
            }
        }

        for (int j = 0; j < numOfNeurons2; j++) {
            for (int i = 0; i < numOfInputs2; ++i) {
                layerWeights2[j][i].write(j + 1); 
            }
        }

        for (int i = 0; i < numOfInputs1; ++i) {
            tempLayerInputs[i].write(i); 
            wait(SC_ZERO_TIME);
            checkPut1 = ILC1->checkPut();
            if (checkPut1) {
                ILC1->put(i, tempLayerInputs[i].read());
            }
        }

        for (int i = 0; i < numOfNeurons1; ++i) {
            checkGet1 = ILC1->checkGet();
            if (checkGet1) {
                tempInput1 = ILC1->get(i);
            }

            for (int k = 0; k < numOfInputs1; ++k) {
                layerInputs1[k].write(tempInput1[k]);
                wait(SC_ZERO_TIME);
            }
        }

        layer1->layerStart.notify(SC_ZERO_TIME);
        wait(layer1->layerDone);

        for (int i = 0; i < numOfInputs2; ++i) {
            checkPut2 = ILC2->checkPut();
            if (checkPut2) {
                ILC2->put(i, layerResults1[i].read());
            }
        }

        for (int i = 0; i < numOfNeurons2; ++i) {

            checkGet2 = ILC2->checkGet();
            if (checkGet2) {
                tempInput2 = ILC2->get(i);
            }

            for (int k = 0; k < numOfInputs2; ++k) {
                layerInputs2[k].write(tempInput2[k]);
                wait(SC_ZERO_TIME);
            }
        }

        layer2->layerStart.notify(SC_ZERO_TIME);
        wait(layer2->layerDone);

        wait();    
    }
}
