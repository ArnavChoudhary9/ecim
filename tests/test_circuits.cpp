#include "test_framework.hpp"
#include "../ecim/ecim.hpp"
#include <cmath>

using namespace ecim;
using namespace TestFramework;

void runCircuitTests(TestRunner& runner) {
    // Test simple voltage divider
    runner.runTest("Circuit: Voltage divider (two resistors)", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();     // ground (ID 0)
        Node* node1 = new Node();   // source (ID 1)
        Node* node2 = new Node();   // middle (ID 2)
        
        DCVoltageSource* vs = new DCVoltageSource(10.0);
        Resistor* r1 = new Resistor(1000.0);  // 1kΩ
        Resistor* r2 = new Resistor(1000.0);  // 1kΩ
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(r1, node1, node2);
        ckt.AddComponent(r2, node2, gnd);
        
        ckt.Step(1e-6);
        
        // In a 1:1 voltage divider, middle node should be 5V
        r.assertEqual(node2->Voltage, 5.0, 1e-3, "Voltage divider middle should be 5V");
        r.assertEqual(node1->Voltage, 10.0, 1e-3, "Source node should be 10V");
    });

    // Test unequal voltage divider
    runner.runTest("Circuit: Unequal voltage divider", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        Resistor* r1 = new Resistor(2000.0);  // 2kΩ
        Resistor* r2 = new Resistor(3000.0);  // 3kΩ
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(r1, node1, node2);
        ckt.AddComponent(r2, node2, gnd);
        
        ckt.Step(1e-6);
        
        // Vout = Vin * R2/(R1+R2) = 5 * 3000/5000 = 3V
        r.assertEqual(node2->Voltage, 3.0, 1e-3, "Voltage divider should be 3V");
    });

    // Test current through series resistors
    runner.runTest("Circuit: Series resistors current", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(10.0);
        Resistor* r1 = new Resistor(500.0);
        Resistor* r2 = new Resistor(500.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(r1, node1, node2);
        ckt.AddComponent(r2, node2, gnd);
        
        ckt.Step(1e-6);
        
        // Current = V/Rtotal = 10/1000 = 0.01A
        double i1 = r1->GetCurrent();
        double i2 = r2->GetCurrent();
        
        r.assertEqual(i1, 0.01, 1e-6, "Current through R1 should be 0.01A");
        r.assertEqual(i2, 0.01, 1e-6, "Current through R2 should be 0.01A");
    });

    // Test parallel resistors
    runner.runTest("Circuit: Parallel resistors", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        Resistor* r1 = new Resistor(1000.0);  // 1kΩ
        Resistor* r2 = new Resistor(1000.0);  // 1kΩ
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(r1, node1, gnd);
        ckt.AddComponent(r2, node1, gnd);
        
        ckt.Step(1e-6);
        
        // Each resistor sees 5V, so each draws 5mA
        // Total current from source = 10mA
        double i1 = r1->GetCurrent();
        double i2 = r2->GetCurrent();
        
        r.assertEqual(i1, 0.005, 1e-6, "Current through R1 should be 5mA");
        r.assertEqual(i2, 0.005, 1e-6, "Current through R2 should be 5mA");
    });

    // Test multiple voltage sources
    runner.runTest("Circuit: Multiple voltage sources", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        Node* node3 = new Node();
        
        DCVoltageSource* vs1 = new DCVoltageSource(10.0);
        DCVoltageSource* vs2 = new DCVoltageSource(5.0);
        Resistor* r1 = new Resistor(1000.0);
        
        ckt.AddComponent(vs1, node1, gnd);
        ckt.AddComponent(vs2, node2, gnd);
        ckt.AddComponent(r1, node1, node2);
        
        ckt.Step(1e-6);
        
        // Voltage across R1 = 10V - 5V = 5V
        // Current through R1 = 5V / 1000Ω = 5mA
        r.assertEqual(node1->Voltage, 10.0, 1e-3, "Node1 should be 10V");
        r.assertEqual(node2->Voltage, 5.0, 1e-3, "Node2 should be 5V");
        r.assertEqual(r1->GetCurrent(), 0.005, 1e-6, "Current should be 5mA");
    });

    // Test circuit with minimal components
    runner.runTest("Circuit: Empty circuit", [](TestRunner& r) {
        CircuitBuilder ckt;
        Node* gnd = new Node();
        Node* node1 = new Node();
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        ckt.AddComponent(vs, node1, gnd);
        
        // Should not crash
        ckt.Step(1e-6);
        r.assertEqual(node1->Voltage, 5.0, 1e-3, "Node1 should be 5V");
    });

    // Test time tracking
    runner.runTest("Circuit: Time tracking", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Resistor* res = new Resistor(100.0);
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        r.assertEqual(ckt.GetCurrentTime(), 0.0, 1e-9, "Initial time should be 0");
        
        ckt.Step(0.001);
        r.assertEqual(ckt.GetCurrentTime(), 0.001, 1e-9, "Time should advance by step");
        
        ckt.Step(0.001);
        r.assertEqual(ckt.GetCurrentTime(), 0.002, 1e-9, "Time should continue advancing");
        
        ckt.ResetTime();
        r.assertEqual(ckt.GetCurrentTime(), 0.0, 1e-9, "Time should reset to 0");
    });

    // Test AC voltage source in circuit
    runner.runTest("Circuit: AC voltage source", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        
        ACVoltageSource* vs = new ACVoltageSource(10.0, 1.0, 0.0); // 10V amplitude, 1Hz
        Resistor* res = new Resistor(100.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        // At t=0, voltage should be 0
        ckt.Step(1e-9);
        r.assertEqual(node1->Voltage, 0.0, 1e-3, "At t=0, AC voltage should be 0V");
        
        // Step to t=0.25s (1/4 period)
        ckt.ResetTime();
        ckt.Step(0.25);
        r.assertEqual(node1->Voltage, 10.0, 0.5, "At peak, AC voltage should be ~10V");
    });

    // Test Custom voltage source in circuit
    runner.runTest("Circuit: Custom voltage source with ramp", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        
        // Voltage ramps from 0 to 10V over 1 second
        CustomVoltageSource* vs = new CustomVoltageSource([](double t) { 
            return t * 10.0; 
        });
        Resistor* res = new Resistor(100.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        ckt.Step(0.0);
        r.assertEqual(node1->Voltage, 0.0, 1e-3, "At t=0, voltage should be 0V");
        
        ckt.Step(0.5);
        r.assertEqual(node1->Voltage, 5.0, 1e-3, "At t=0.5s, voltage should be 5V");
        
        ckt.Step(0.5);
        r.assertEqual(node1->Voltage, 10.0, 1e-3, "At t=1s, voltage should be 10V");
    });

    // Test Custom voltage source with step function
    runner.runTest("Circuit: Custom voltage source with step", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        
        // Step from 0V to 12V at t=1s
        CustomVoltageSource* vs = new CustomVoltageSource([](double t) { 
            return t < 1.0 ? 0.0 : 12.0; 
        });
        Resistor* res = new Resistor(1000.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        ckt.Step(0.5);
        r.assertEqual(node1->Voltage, 0.0, 1e-3, "Before step, voltage should be 0V");
        
        ckt.Step(0.6); // Now at t=1.1s
        r.assertEqual(node1->Voltage, 12.0, 1e-3, "After step, voltage should be 12V");
    });
}
