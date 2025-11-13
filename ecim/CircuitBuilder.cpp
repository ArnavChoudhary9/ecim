#include "CircuitBuilder.hpp"
#include "VoltageSource.hpp"
#include "Resistor.hpp"
#include "Capacitor.hpp"
#include "Inductor.hpp"
#include <algorithm>

namespace ecim {
    CircuitBuilder::~CircuitBuilder() {
        for (auto comp : m_Components) delete comp;
        for (auto node : m_Nodes) delete node;
    }

    void CircuitBuilder::AddComponent(Component *component, Node *node1, Node *node2) {
        component->Connect(node1, node2);
        m_Components.push_back(component);

        // Ensure nodes are tracked
        if (std::find(m_Nodes.begin(), m_Nodes.end(), node1) == m_Nodes.end()) {
            m_Nodes.push_back(node1);
        }
        if (std::find(m_Nodes.begin(), m_Nodes.end(), node2) == m_Nodes.end()) {
            m_Nodes.push_back(node2);
        }
    }

    void CircuitBuilder::Solve() {
        int N = 0;
        for (auto node : m_Nodes) {
            if (node->Id > N) N = node->Id;
        }
        N += 1; // Total number of nodes (assuming IDs start from 0 for ground)

        int M = 0; // Number of voltage sources
        for (auto comp : m_Components) {
            // Assuming Component has a method IsVoltageSource()
            if (dynamic_cast<VoltageSource*>(comp)) {
                M++;
            }
        }

        int matrixSize = (N > 0 ? N - 1 : 0) + M; // Exclude ground node
        Eigen::MatrixXd G = Eigen::MatrixXd::Zero(matrixSize, matrixSize);
        Eigen::VectorXd I = Eigen::VectorXd::Zero(matrixSize);

        // Stamp each resistor
        for (auto comp : m_Components) {
            if (auto resistor = dynamic_cast<Resistor*>(comp)) {
                resistor->Stamp(G, I, -1); // -1 indicates no voltage source index
            }
        }

        // Stamp each voltage source
        int vsIndex = 0;
        for (auto comp : m_Components) {
            if (auto voltageSource = dynamic_cast<VoltageSource*>(comp)) {
                voltageSource->Stamp(G, I, (N > 0 ? N - 1 : 0) + vsIndex);
                vsIndex++;
            }
        }

        // Solve the system G * V = I
        Eigen::VectorXd V = G.colPivHouseholderQr().solve(I);

        // Extract node voltages
        for (auto node : m_Nodes) {
            if (node->Id == 0) {
                node->Voltage = 0.0; // Ground node
            } else {
                int idx = node->Id - 1;
                if (idx >= 0 && idx < V.size()) {
                    node->Voltage = V(idx);
                }
            }
        }
        
        // Extract voltage source currents from solution vector
        vsIndex = 0;
        for (auto comp : m_Components) {
            if (auto voltageSource = dynamic_cast<VoltageSource*>(comp)) {
                int currentIdx = (N > 0 ? N - 1 : 0) + vsIndex;
                if (currentIdx >= 0 && currentIdx < V.size()) {
                    voltageSource->SetCurrent(V(currentIdx));
                }
                vsIndex++;
            }
        }
    }

    const std::vector<Node*>& CircuitBuilder::GetNodes() const {
        return m_Nodes;
    }

    double CircuitBuilder::GetCurrentTime() const {
        return m_CurrentTime;
    }

    // Time-based simulation: step forward by deltaTime
    void CircuitBuilder::Step(double deltaTime) {
        int N = 0;
        for (auto node : m_Nodes) {
            if (node->Id > N) N = node->Id;
        }
        N += 1; // Total number of nodes

        int M = 0; // Number of voltage sources
        for (auto comp : m_Components) {
            if (dynamic_cast<VoltageSource*>(comp)) {
                M++;
            }
        }

        int matrixSize = (N > 0 ? N - 1 : 0) + M;
        Eigen::MatrixXd G = Eigen::MatrixXd::Zero(matrixSize, matrixSize);
        Eigen::VectorXd I = Eigen::VectorXd::Zero(matrixSize);

        // Stamp resistors (same as DC)
        for (auto comp : m_Components) {
            if (auto resistor = dynamic_cast<Resistor*>(comp)) {
                resistor->Stamp(G, I, -1);
            }
        }

        // Stamp capacitors (transient)
        for (auto comp : m_Components) {
            if (auto capacitor = dynamic_cast<Capacitor*>(comp)) {
                capacitor->StampTransient(G, I, deltaTime);
            }
        }

        // Stamp inductors (transient)
        for (auto comp : m_Components) {
            if (auto inductor = dynamic_cast<Inductor*>(comp)) {
                inductor->StampTransient(G, I, deltaTime);
            }
        }

        // Stamp voltage sources
        int vsIndex = 0;
        for (auto comp : m_Components) {
            if (auto voltageSource = dynamic_cast<VoltageSource*>(comp)) {
                voltageSource->Stamp(G, I, (N > 0 ? N - 1 : 0) + vsIndex);
                vsIndex++;
            }
        }

        // Solve the system
        Eigen::VectorXd V = G.colPivHouseholderQr().solve(I);

        // Extract node voltages
        for (auto node : m_Nodes) {
            if (node->Id == 0) {
                node->Voltage = 0.0;
            } else {
                int idx = node->Id - 1;
                if (idx >= 0 && idx < V.size()) {
                    node->Voltage = V(idx);
                }
            }
        }

        // Extract voltage source currents
        vsIndex = 0;
        for (auto comp : m_Components) {
            if (auto voltageSource = dynamic_cast<VoltageSource*>(comp)) {
                int currentIdx = (N > 0 ? N - 1 : 0) + vsIndex;
                if (currentIdx >= 0 && currentIdx < V.size()) {
                    voltageSource->SetCurrent(V(currentIdx));
                }
                vsIndex++;
            }
        }

        // Update state of reactive components for next timestep
        for (auto comp : m_Components) {
            if (auto capacitor = dynamic_cast<Capacitor*>(comp)) {
                capacitor->UpdateState();
            }
            if (auto inductor = dynamic_cast<Inductor*>(comp)) {
                inductor->UpdateState();
            }
        }

        // Advance time
        m_CurrentTime += deltaTime;
    }

    // Simulate for a given duration with specified timestep
    void CircuitBuilder::Simulate(double duration, double deltaTime) {
        double endTime = m_CurrentTime + duration;
        while (m_CurrentTime < endTime) {
            Step(deltaTime);
        }
    }

    // Reset simulation time
    void CircuitBuilder::ResetTime() {
        m_CurrentTime = 0.0;
    }
}
