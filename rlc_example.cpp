#include <iostream>
#include <iomanip>
#include "ecim/ecim.hpp"

using namespace ecim;

int main() {
    std::cout << "=== Time-Based RLC Circuit Simulation ===\n\n";
    
    // Build an RLC circuit for demonstrating oscillation
    CircuitBuilder ckt;

    Node* gnd = new Node();      // ground, id=0
    Node* node1 = new Node();    // source node
    Node* node2 = new Node();    // LC junction

    // Step voltage source (simulating switch closing at t=0)
    VoltageSource* vs = new VoltageSource(10.0);
    Resistor* r = new Resistor(100.0);         // 100Ω
    Inductor* l = new Inductor(0.1);           // 100mH = 0.1H
    Capacitor* c = new Capacitor(0.00001);     // 10µF = 0.00001F

    ckt.AddComponent(vs, node1, gnd);     // Voltage source
    ckt.AddComponent(r, node1, node2);    // Resistor
    ckt.AddComponent(l, node2, gnd);      // Inductor
    ckt.AddComponent(c, node2, gnd);      // Capacitor (parallel with L)

    // Simulation parameters
    double deltaTime = 0.00001;  // 10µs timestep
    double totalTime = 0.01;     // Simulate for 10ms
    int printEvery = 100;        // Print every 100 steps (1ms)

    std::cout << "Circuit: 10V source -> 100Ω -> (100mH || 10µF) -> ground\n";
    std::cout << "Timestep: " << deltaTime << " s\n";
    std::cout << "Duration: " << totalTime << " s\n\n";

    std::cout << std::setw(12) << "Time (s)" 
              << std::setw(15) << "V_LC (V)" 
              << std::setw(15) << "I_total (A)" << "\n";
    std::cout << std::string(42, '-') << "\n";

    int step = 0;
    double time = 0.0;
    
    while (time <= totalTime) {
        ckt.Step(deltaTime);
        time = ckt.GetCurrentTime();

        if (step % printEvery == 0) {
            Probe pLC(node2);   // Voltage across LC
            Probe pR(r);        // Current through resistor
            
            std::cout << std::fixed << std::setprecision(6)
                      << std::setw(12) << time
                      << std::setw(15) << pLC.Voltage()
                      << std::setw(15) << pR.Current() << "\n";
        }
        
        step++;
    }

    std::cout << "\n=== Simulation Complete ===\n";

    return 0;
}
