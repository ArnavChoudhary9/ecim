#pragma once
#include "Node.hpp"
#include "Component.hpp"

namespace ecim {
    class Resistor;
    class VoltageSource;

    class Probe {
        Node* m_Node;
        Component* m_Component;
    public:
        Probe(Node* n);
        Probe(Component* c);
        double Voltage() const;
        double Current() const;
    };
}
