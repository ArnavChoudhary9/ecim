#pragma once

#include <vector>
#include "Component.hpp"
#include "Node.hpp"

namespace ecim {
    class VoltageSource;
    class Resistor;
    class Capacitor;
    class Inductor;

    class CircuitBuilder {
        std::vector<Component*> m_Components;
        std::vector<Node*> m_Nodes;
        double m_CurrentTime = 0.0;

    public:
        ~CircuitBuilder();
        void AddComponent(Component *component, Node *node1, Node *node2);
        void Solve();
        const std::vector<Node*>& GetNodes() const;
        double GetCurrentTime() const;
        void Step(double deltaTime);
        void Simulate(double duration, double deltaTime);
        void ResetTime();
    };
}
