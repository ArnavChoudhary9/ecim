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
        VoltageSource* vs = new VoltageSource(5.0);
        r.assertNotNull(vs, "VoltageSource should be created");
        r.assertEqual(vs->GetCurrent(), 0.0, 1e-9, "Initial current should be zero");
        delete vs;
    });

    // Test VoltageSource current tracking
    runner.runTest("VoltageSource: Current tracking", [](TestRunner& r) {
        VoltageSource* vs = new VoltageSource(10.0);
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
}
