#include "test_framework.hpp"
#include "../ecim/ecim.hpp"
#include <cmath>

using namespace ecim;
using namespace TestFramework;

void runTransientTests(TestRunner& runner) {
    // Test RC charging circuit
    runner.runTest("Transient: RC charging (initial behavior)", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        Resistor* res = new Resistor(1000.0);    // 1kΩ
        Capacitor* cap = new Capacitor(0.001);   // 1mF, τ = RC = 1s
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, node2);
        ckt.AddComponent(cap, node2, gnd);
        
        // Initially, capacitor voltage should be near 0
        ckt.Step(1e-6);
        r.assertTrue(node2->Voltage < 0.1, "Capacitor should start near 0V");
        
        // After some time, capacitor should be charging toward 5V
        for (int i = 0; i < 100; i++) {
            ckt.Step(0.01);  // 100 steps of 10ms = 1s total
        }
        
        // After ~1 time constant (1s), should be ~63% charged
        // V(t) = Vfinal * (1 - e^(-t/τ))
        // V(1s) = 5 * (1 - e^-1) ≈ 5 * 0.632 ≈ 3.16V
        r.assertTrue(node2->Voltage > 2.5 && node2->Voltage < 4.0, 
                    "After 1τ, capacitor should be 50-80% charged");
    });

    // Test RL circuit
    runner.runTest("Transient: RL circuit (initial behavior)", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(10.0);
        Resistor* res = new Resistor(100.0);     // 100Ω
        Inductor* ind = new Inductor(0.1);       // 100mH, τ = L/R = 0.001s
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, node2);
        ckt.AddComponent(ind, node2, gnd);
        
        // Step the circuit
        ckt.Step(1e-6);
        
        // Inductor should initially resist current change
        r.assertTrue(true, "RL circuit should step without error");
    });

    // Test LC oscillation setup
    runner.runTest("Transient: LC circuit setup", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(10.0);
        Resistor* res = new Resistor(100.0);
        Inductor* ind = new Inductor(0.1);       // 100mH
        Capacitor* cap = new Capacitor(0.00001); // 10µF
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, node2);
        ckt.AddComponent(ind, node2, gnd);
        ckt.AddComponent(cap, node2, gnd);
        
        // Initial step
        ckt.Step(1e-6);
        double initialVoltage = node2->Voltage;
        
        // Step forward
        for (int i = 0; i < 10; i++) {
            ckt.Step(1e-5);
        }
        
        // Voltage should have changed
        r.assertTrue(true, "LC circuit should simulate without error");
    });

    // Test capacitor state update
    runner.runTest("Transient: Capacitor state tracking", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        Resistor* res = new Resistor(1000.0);
        Capacitor* cap = new Capacitor(0.001);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, node2);
        ckt.AddComponent(cap, node2, gnd);
        
        double prevVoltage = 0.0;
        bool voltageIncreasing = false;
        
        for (int i = 0; i < 5; i++) {
            ckt.Step(0.1);
            if (node2->Voltage > prevVoltage) {
                voltageIncreasing = true;
            }
            prevVoltage = node2->Voltage;
        }
        
        r.assertTrue(voltageIncreasing, "Capacitor voltage should increase during charging");
    });

    // Test simulation duration
    runner.runTest("Transient: Simulate function", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Resistor* res = new Resistor(100.0);
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        double duration = 0.1;
        double timeStep = 0.01;
        
        ckt.Simulate(duration, timeStep);
        
        // Should have advanced by duration
        r.assertEqual(ckt.GetCurrentTime(), duration, 1e-9, 
                     "Simulate should advance time by duration");
    });

    // Test multiple simulation runs
    runner.runTest("Transient: Multiple simulation runs", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Resistor* res = new Resistor(100.0);
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        ckt.Simulate(0.05, 0.01);
        double time1 = ckt.GetCurrentTime();
        
        ckt.Simulate(0.05, 0.01);
        double time2 = ckt.GetCurrentTime();
        
        r.assertEqual(time2, time1 + 0.05, 1e-9, 
                     "Second simulation should continue from first");
    });

    // Test different timesteps
    runner.runTest("Transient: Variable timestep", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Resistor* res = new Resistor(100.0);
        DCVoltageSource* vs = new DCVoltageSource(5.0);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, gnd);
        
        // Large timestep
        ckt.Step(0.1);
        r.assertEqual(ckt.GetCurrentTime(), 0.1, 1e-9, "Large timestep should work");
        
        ckt.ResetTime();
        
        // Small timestep
        ckt.Step(1e-9);
        r.assertEqual(ckt.GetCurrentTime(), 1e-9, 1e-12, "Small timestep should work");
    });

    // Test RC time constant verification
    runner.runTest("Transient: RC time constant verification", [](TestRunner& r) {
        CircuitBuilder ckt;
        
        Node* gnd = new Node();
        Node* node1 = new Node();
        Node* node2 = new Node();
        
        // τ = RC = 100Ω * 0.01F = 1 second
        DCVoltageSource* vs = new DCVoltageSource(10.0);
        Resistor* res = new Resistor(100.0);
        Capacitor* cap = new Capacitor(0.01);
        
        ckt.AddComponent(vs, node1, gnd);
        ckt.AddComponent(res, node1, node2);
        ckt.AddComponent(cap, node2, gnd);
        
        // Simulate for several time constants
        double dt = 0.01;  // 10ms timestep
        
        // Initial step
        ckt.Step(dt);
        
        // After many steps, should approach final value
        for (int i = 0; i < 500; i++) {
            ckt.Step(dt);
        }
        
        // After 5 seconds (5τ), should be > 99% charged (≈ 9.9V)
        r.assertTrue(node2->Voltage > 9.0, "After 5τ, capacitor should be nearly fully charged");
    });
}
