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
    ckt.AddComponent(new VoltageSource(5.0), node1, gnd);    // +5V at node1 relative to ground
    ckt.AddComponent(new Resistor(2000.0), node1, node2);     // 2kΩ between node1 and node2
    ckt.AddComponent(new Resistor(3000.0), node2, gnd);       // 3kΩ between node2 and ground

    // Solve the circuit
    ckt.Solve();

    // Probes for voltages at node1 and node2
    Probe p1(node1), p2(node2);
    std::cout << "Voltage at Node " << node1->Id << ": " << p1.Voltage() << " V\n";
    std::cout << "Voltage at Node " << node2->Id << ": " << p2.Voltage() << " V\n";

    return 0;
}
