#pragma once

#include <vector>
#include "Component.hpp"
#include "Node.hpp"
#include "ProbeManager.hpp"

namespace ecim {
    class VoltageSource;
    class Resistor;
    class Capacitor;
    class Inductor;

    class CircuitBuilder {
        std::vector<Component*> m_Components;
        std::vector<Node*> m_Nodes;
        double m_CurrentTime = 0.0;
        ProbeManager m_ProbeManager;

    public:
        ~CircuitBuilder();
        void AddComponent(Component *component, Node *node1, Node *node2);
        const std::vector<Node*>& GetNodes() const;
        double GetCurrentTime() const;
        void Step(double deltaTime);
        void Simulate(double duration, double deltaTime);
        void ResetTime();
        
        // Probe management
        ProbeManager& GetProbeManager();
        Probe* AddProbe(const ProbeConfig& config);
    };
}
