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

    const std::vector<Node*>& CircuitBuilder::GetNodes() const {
        return m_Nodes;
    }

    double CircuitBuilder::GetCurrentTime() const {
        return m_CurrentTime;
    }

    // Time-based simulation: step forward by deltaTime
    void CircuitBuilder::Step(double deltaTime) {
        // Advance time first - we solve for the state at the new time
        m_CurrentTime += deltaTime;
        
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

        // Stamp resistors
        for (auto comp : m_Components) {
            if (auto resistor = dynamic_cast<Resistor*>(comp)) {
                SimulationState state{G, I, deltaTime, -1, m_CurrentTime};
                resistor->Stamp(state);
            }
        }

        // Stamp capacitors
        for (auto comp : m_Components) {
            if (auto capacitor = dynamic_cast<Capacitor*>(comp)) {
                SimulationState state{G, I, deltaTime, -1, m_CurrentTime};
                capacitor->Stamp(state);
            }
        }

        // Stamp inductors
        for (auto comp : m_Components) {
            if (auto inductor = dynamic_cast<Inductor*>(comp)) {
                SimulationState state{G, I, deltaTime, -1, m_CurrentTime};
                inductor->Stamp(state);
            }
        }

        // Stamp voltage sources
        int vsIndex = 0;
        for (auto comp : m_Components) {
            if (auto voltageSource = dynamic_cast<VoltageSource*>(comp)) {
                SimulationState state{G, I, deltaTime, (N > 0 ? N - 1 : 0) + vsIndex, m_CurrentTime};
                voltageSource->Stamp(state);
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
    }

    // Simulate for a given duration with specified timestep
    void CircuitBuilder::Simulate(double duration, double deltaTime) {
        double endTime = m_CurrentTime + duration;
        const double epsilon = deltaTime * 0.01; // Small tolerance for floating point comparison
        while (m_CurrentTime < endTime - epsilon) {
            Step(deltaTime);
        }
    }

    // Reset simulation time
    void CircuitBuilder::ResetTime() {
        m_CurrentTime = 0.0;
    }
}
