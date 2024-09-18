#include <systemc.h>
#include <chrono>
#include "layer_test.h" // Assuming the Layer module is defined in "Layer.h"

int sc_main(int argc, char* argv[]) {
    partitionNetwork partitionNetwork("partitionNetwork");
    auto start = std::chrono::high_resolution_clock::now();
    sc_start(1, SC_NS);
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the execution time of sc_start
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";

    // Print the execution time
    //cout << "Execution time of sc_start: " << execution_time << endl;
    //cout << "hiiiiiiiiii == "  << endl;
    return 0;
}
