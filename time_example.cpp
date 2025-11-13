#include <iostream>
#include <iomanip>
#include "ecim/ecim.hpp"

using namespace ecim;

int main() {
    std::cout << "=== Time-Based RC Circuit Simulation ===\n\n";
    
    // Build an RC circuit: voltage source -> resistor -> capacitor -> ground
    // This creates an RC charging circuit
    CircuitBuilder ckt;

    Node* gnd = new Node();      // ground, id=0
    Node* node1 = new Node();    // between source and resistor
    Node* node2 = new Node();    // between resistor and capacitor

    // 5V source, 1kΩ resistor, 1mF capacitor
    VoltageSource* vs = new VoltageSource(5.0);
    Resistor* r = new Resistor(1000.0);        // 1kΩ
    Capacitor* c = new Capacitor(0.001);       // 1mF = 0.001F

    ckt.AddComponent(vs, node1, gnd);    // Voltage source
    ckt.AddComponent(r, node1, node2);   // Resistor
    ckt.AddComponent(c, node2, gnd);     // Capacitor

    // Simulation parameters
    double deltaTime = 0.0001;  // 0.1ms timestep
    double totalTime = 0.01;    // Simulate for 10ms
    int printEvery = 10;        // Print every 10 steps (1ms)

    std::cout << "Circuit: 5V source -> 1kΩ resistor -> 1mF capacitor -> ground\n";
    std::cout << "Time constant (τ = RC) = " << (1000.0 * 0.001) << " seconds = 1 second\n";
    std::cout << "Timestep: " << deltaTime << " s\n";
    std::cout << "Duration: " << totalTime << " s\n\n";

    std::cout << std::setw(12) << "Time (s)" 
              << std::setw(15) << "V_cap (V)" 
              << std::setw(15) << "V_R (V)"
              << std::setw(15) << "I (A)" << "\n";
    std::cout << std::string(57, '-') << "\n";

    int step = 0;
    double time = 0.0;
    
    while (time <= totalTime) {
        // Step the simulation
        ckt.Step(deltaTime);
        time = ckt.GetCurrentTime();

        // Print results at intervals
        if (step % printEvery == 0) {
            Probe pCap(node2);  // Voltage across capacitor
            Probe pR(node1);    // Voltage at resistor input
            Probe pCurrent(r);  // Current through resistor
            
            std::cout << std::fixed << std::setprecision(6)
                      << std::setw(12) << time
                      << std::setw(15) << pCap.Voltage()
                      << std::setw(15) << pR.Voltage()
                      << std::setw(15) << pCurrent.Current() << "\n";
        }
        
        step++;
    }

    std::cout << "\n=== Simulation Complete ===\n";
    std::cout << "The capacitor voltage should approach 5V exponentially.\n";
    std::cout << "After 1 time constant (1s), voltage ≈ 3.16V (63% of 5V)\n";

    return 0;
}
