#include "test_framework.hpp"
#include "../ecim/ecim.hpp"
#include <cmath>

using namespace ecim;
using namespace TestFramework;

void runBasicComponentTests(TestRunner& runner) {
    // Test Node creation and initialization
    runner.runTest("Node: Creation and ID assignment", [](TestRunner& r) {
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        r.assertTrue(node1->Id >= 0, "Node ID should be non-negative");
        r.assertTrue(node2->Id > node1->Id, "Node IDs should be sequential");
        r.assertEqual(node1->Voltage, 0.0, 1e-9, "Initial voltage should be zero");
        
        delete node1;
        delete node2;
    });

    // Test Resistor creation
    runner.runTest("Resistor: Creation and properties", [](TestRunner& r) {
        Resistor* resistor = new Resistor(1000.0);
        r.assertNotNull(resistor, "Resistor should be created");
        delete resistor;
    });

    // Test Resistor current calculation
    runner.runTest("Resistor: Current calculation (Ohm's law)", [](TestRunner& r) {
        Node* node1 = new Node();
        Node* node2 = new Node();
        node1->Voltage = 5.0;
        node2->Voltage = 0.0;
        
        Resistor* resistor = new Resistor(1000.0);
        resistor->Connect(node1, node2);
        
        double current = resistor->GetCurrent();
        r.assertEqual(current, 0.005, 1e-9, "Current should be V/R = 5/1000 = 0.005A");
        
        delete resistor;
        delete node1;
        delete node2;
    });

    // Test VoltageSource creation
    runner.runTest("VoltageSource: Creation and properties", [](TestRunner& r) {
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        r.assertNotNull(vs, "VoltageSource should be created");
        r.assertEqual(vs->GetCurrent(), 0.0, 1e-9, "Initial current should be zero");
        r.assertEqual(vs->GetVoltage(0.0), 5.0, 1e-9, "DC voltage should be constant");
        r.assertEqual(vs->GetVoltage(100.0), 5.0, 1e-9, "DC voltage should be constant over time");
        delete vs;
    });

    // Test VoltageSource current tracking
    runner.runTest("VoltageSource: Current tracking", [](TestRunner& r) {
        DCVoltageSource* vs = new DCVoltageSource(10.0);
        vs->SetCurrent(0.002);
        r.assertEqual(vs->GetCurrent(), 0.002, 1e-9, "Current should be settable");
        delete vs;
    });

    // Test Capacitor creation
    runner.runTest("Capacitor: Creation and properties", [](TestRunner& r) {
        Capacitor* cap = new Capacitor(0.001);
        r.assertNotNull(cap, "Capacitor should be created");
        delete cap;
    });

    // Test Inductor creation
    runner.runTest("Inductor: Creation and properties", [](TestRunner& r) {
        Inductor* ind = new Inductor(0.1);
        r.assertNotNull(ind, "Inductor should be created");
        delete ind;
    });

    // Test Component connection
    runner.runTest("Component: Node connection", [](TestRunner& r) {
        Node* node1 = new Node();
        Node* node2 = new Node();
        Resistor* resistor = new Resistor(100.0);
        
        resistor->Connect(node1, node2);
        // If we got here without crash, connection worked
        r.assertTrue(true, "Component should connect to nodes");
        
        delete resistor;
        delete node1;
        delete node2;
    });

    // Test Probe with Node
    runner.runTest("Probe: Node voltage measurement", [](TestRunner& r) {
        Node* node = new Node();
        node->Voltage = 3.3;
        
        Probe probe(node);
        r.assertEqual(probe.Voltage(), 3.3, 1e-9, "Probe should read node voltage");
        
        delete node;
    });

    // Test Probe with Component
    runner.runTest("Probe: Component current measurement", [](TestRunner& runner) {
        Node* n1 = new Node();
        Node* n2 = new Node();
        n1->Voltage = 10.0;
        n2->Voltage = 0.0;
        
        Resistor* res = new Resistor(1000.0);
        res->Connect(n1, n2);
        
        Probe probe(res);
        runner.assertEqual(probe.Current(), 0.01, 1e-9, "Probe should read component current");
        
        delete res;
        delete n1;
        delete n2;
    });

    // Test ACVoltageSource creation
    runner.runTest("ACVoltageSource: Creation and properties", [](TestRunner& r) {
        ACVoltageSource* vs = new ACVoltageSource(10.0, 50.0, 0.0); // 10V amplitude, 50Hz, 0 phase
        r.assertNotNull(vs, "ACVoltageSource should be created");
        r.assertEqual(vs->GetAmplitude(), 10.0, 1e-9, "Amplitude should be 10V");
        r.assertEqual(vs->GetFrequency(), 50.0, 1e-9, "Frequency should be 50Hz");
        r.assertEqual(vs->GetPhase(), 0.0, 1e-9, "Phase should be 0");
        delete vs;
    });

    // Test ACVoltageSource time-varying voltage
    runner.runTest("ACVoltageSource: Time-varying voltage", [](TestRunner& r) {
        ACVoltageSource* vs = new ACVoltageSource(10.0, 1.0, 0.0); // 10V, 1Hz, 0 phase
        
        // At t=0, sin(0) = 0
        r.assertEqual(vs->GetVoltage(0.0), 0.0, 1e-6, "At t=0, voltage should be 0V");
        
        // At t=0.25s (1/4 period), sin(π/2) = 1, voltage = 10V
        r.assertEqual(vs->GetVoltage(0.25), 10.0, 1e-3, "At t=0.25s, voltage should be 10V (peak)");
        
        // At t=0.5s (1/2 period), sin(π) = 0
        r.assertEqual(vs->GetVoltage(0.5), 0.0, 1e-3, "At t=0.5s, voltage should be 0V");
        
        // At t=0.75s (3/4 period), sin(3π/2) = -1, voltage = -10V
        r.assertEqual(vs->GetVoltage(0.75), -10.0, 1e-3, "At t=0.75s, voltage should be -10V (negative peak)");
        
        delete vs;
    });

    // Test ACVoltageSource with phase shift
    runner.runTest("ACVoltageSource: Phase shift", [](TestRunner& r) {
        const double PI = 3.14159265358979323846;
        ACVoltageSource* vs = new ACVoltageSource(5.0, 1.0, PI/2); // 5V, 1Hz, 90° phase
        
        // At t=0 with π/2 phase shift, sin(π/2) = 1, voltage = 5V
        r.assertEqual(vs->GetVoltage(0.0), 5.0, 1e-3, "With 90° phase, voltage should start at peak");
        
        delete vs;
    });

    // Test CustomVoltageSource with constant function
    runner.runTest("CustomVoltageSource: Constant function", [](TestRunner& r) {
        CustomVoltageSource* vs = new CustomVoltageSource([](double t) { return 7.5; });
        
        r.assertEqual(vs->GetVoltage(0.0), 7.5, 1e-9, "Custom constant should return 7.5V");
        r.assertEqual(vs->GetVoltage(100.0), 7.5, 1e-9, "Custom constant should be time-independent");
        
        delete vs;
    });

    // Test CustomVoltageSource with linear ramp
    runner.runTest("CustomVoltageSource: Linear ramp", [](TestRunner& r) {
        CustomVoltageSource* vs = new CustomVoltageSource([](double t) { return 2.0 * t; });
        
        r.assertEqual(vs->GetVoltage(0.0), 0.0, 1e-9, "At t=0, ramp should be 0V");
        r.assertEqual(vs->GetVoltage(1.0), 2.0, 1e-9, "At t=1, ramp should be 2V");
        r.assertEqual(vs->GetVoltage(5.0), 10.0, 1e-9, "At t=5, ramp should be 10V");
        
        delete vs;
    });

    // Test CustomVoltageSource with step function
    runner.runTest("CustomVoltageSource: Step function", [](TestRunner& r) {
        CustomVoltageSource* vs = new CustomVoltageSource([](double t) { 
            return t < 1.0 ? 0.0 : 10.0; 
        });
        
        r.assertEqual(vs->GetVoltage(0.0), 0.0, 1e-9, "Before step, voltage should be 0V");
        r.assertEqual(vs->GetVoltage(0.5), 0.0, 1e-9, "Before step, voltage should be 0V");
        r.assertEqual(vs->GetVoltage(1.0), 10.0, 1e-9, "After step, voltage should be 10V");
        r.assertEqual(vs->GetVoltage(2.0), 10.0, 1e-9, "After step, voltage should be 10V");
        
        delete vs;
    });

    // Test CustomVoltageSource with pulse wave
    runner.runTest("CustomVoltageSource: Pulse wave", [](TestRunner& r) {
        CustomVoltageSource* vs = new CustomVoltageSource([](double t) {
            double period = 1.0;
            double phase = fmod(t, period);
            return (phase < 0.5) ? 5.0 : 0.0;
        });
        
        r.assertEqual(vs->GetVoltage(0.0), 5.0, 1e-9, "First half of period should be 5V");
        r.assertEqual(vs->GetVoltage(0.25), 5.0, 1e-9, "First half of period should be 5V");
        r.assertEqual(vs->GetVoltage(0.5), 0.0, 1e-9, "Second half of period should be 0V");
        r.assertEqual(vs->GetVoltage(0.75), 0.0, 1e-9, "Second half of period should be 0V");
        r.assertEqual(vs->GetVoltage(1.0), 5.0, 1e-9, "Next period should repeat");
        
        delete vs;
    });
}
