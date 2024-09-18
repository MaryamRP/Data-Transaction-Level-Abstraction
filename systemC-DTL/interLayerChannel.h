#include <atomic>
#include "interfaceClasses.h"

template <class T, class P, int numOfInitiators, int numOfTargets>
class interLayerChannel : public put_if<T>, public get_if<T, P> {
    std::atomic<int> comingFrom, goingTo;
    std::atomic<int> cntInit, cntTarg;
    T dataPlaced[numOfInitiators];
    sc_int<8> dataVector[numOfInitiators];

    bool initPlaced[numOfInitiators], targetReceived[numOfTargets];
    bool allInitPut, allTarGet;

public:
    interLayerChannel() : comingFrom(-1), goingTo(-1), cntInit(0), cntTarg(0), allInitPut(false), allTarGet(true)
    {
        for (int i = 0; i < numOfInitiators; ++i) {
            initPlaced[i] = false;
        }
        for (int i = 0; i < numOfTargets; ++i) {
            targetReceived[i] = false;
        }
    }

    ~interLayerChannel() {}

    typename put_if<T>::putStatus put(int initiator, T data) override {
        if (allInitPut) {
            return put_if<T>::putStatus::putBlocked;
        }
        else if (initPlaced[initiator]) {
            return put_if<T>::putStatus::putDuplicate;
        }
        else {
            comingFrom.store(initiator);
            dataPlaced[comingFrom.load()] = data;
            wait(SC_ZERO_TIME);
            initPlaced[comingFrom.load()] = true;
            cntInit.fetch_add(1);
            if (cntInit.load() == numOfInitiators) {
                allInitPut = true;
                cntInit = ATOMIC_VAR_INIT(0);
                allTarGet = false;
            }
            return put_if<T>::putStatus::putSuccess;
        }
    }

    bool checkPut() override {
        if (allTarGet && !allInitPut) {
            return true;
        }
        else {
            return false;
        }

    };

    bool checkGet() override {
        if (allInitPut && !allTarGet) {
            return true;
        }
        else {
            return false;
        }
    };

    sc_int<8>* get(int target) override {
        if (allInitPut && !targetReceived[target]) {
            goingTo.store(target);
            targetReceived[goingTo.load()] = true;
            cntTarg.fetch_add(1);
            if (cntTarg.load() == numOfTargets) {
                allTarGet = true;
                allInitPut = false;
            }
            for (int i = 0; i < numOfInitiators; ++i) {
                dataVector[i] = dataPlaced[i];
            }
        }
        return dataVector;
    }









   /* void get(int target, P& data) override {
        //cout<< "allInitPut= "<< allInitPut<<"  , targetReceived = " << targetReceived <<endl;
        if (allInitPut && !targetReceived[target]) {
            goingTo.store(target);
            targetReceived[goingTo.load()] = true;
            cntTarg.fetch_add(1);
            if (cntTarg.load() == numOfTargets) {
                allTarGet = true;
                allInitPut = false;
            }

            // Packing the data from all initiators
            for (int i = 0; i < numOfInitiators; ++i) {

                cout << "Get dataPlaced =  " << sc_lv<32> (dataPlaced) << endl;
                data.range((i + 1) * dataPlaced[i].length() - 1, i * dataPlaced[i].length()) = dataPlaced[i];
                cout << "Get data =  " << data << endl;

                dataVector[i]=(dataPlaced[i]);
                cout << "dataVector data =  " << dataVector[i] << endl;
               // dataBin.write(data);

            }
            
           // cout << "data =  " << data << endl;
        }
    }
    */


};
