#include <iostream>
#include "ecim/ecim.hpp"

using namespace ecim; 

int main() {
    // Build a simple circuit: a 5V battery (voltage source) with two series resistors.
    // Nodes: ground (id 0), node1, node2.
    CircuitBuilder ckt;

    Node* gnd = new Node();  // ground, id=0
    Node* node1 = new Node(); 
    Node* node2 = new Node();

    // Components: Vsource between ground and node1, R1 between node1-node2, R2 between node2-ground
    DCVoltageSource* vs = new DCVoltageSource(5.0);
    Resistor* r1 = new Resistor(2000.0);
    Resistor* r2 = new Resistor(3000.0);
    
    ckt.AddComponent(vs, node1, gnd);    // +5V at node1 relative to ground
    ckt.AddComponent(r1, node1, node2);  // 2kΩ between node1 and node2
    ckt.AddComponent(r2, node2, gnd);    // 3kΩ between node2 and ground

    // Step the circuit to get initial voltages
    ckt.Step(1e-6);

    // Probes for voltages at node1 and node2
    Probe p1(node1), p2(node2);
    std::cout << "Voltage at Node " << node1->Id << ": " << p1.Voltage() << " V\n";
    std::cout << "Voltage at Node " << node2->Id << ": " << p2.Voltage() << " V\n";
    
    // Probes for currents through components
    Probe pVs(vs), pR1(r1), pR2(r2);
    std::cout << "\nCurrent through Voltage Source: " << pVs.Current() << " A\n";
    std::cout << "Current through R1 (2kΩ): " << pR1.Current() << " A\n";
    std::cout << "Current through R2 (3kΩ): " << pR2.Current() << " A\n";

    return 0;
}
