#pragma once
#include "Node.hpp"
#include "Component.hpp"
#include "Resistor.hpp"
#include "VoltageSource.hpp"

namespace ecim {
    class Probe {
        Node* m_Node;
        Component* m_Component;
    public:
        Probe(Node* n) : m_Node(n), m_Component(nullptr) {}
        Probe(Component* c) : m_Node(nullptr), m_Component(c) {}
        
        double Voltage() const { 
            return m_Node ? m_Node->Voltage : 0.0; 
        }
        
        double Current() const {
            if (!m_Component) return 0.0;
            
            // For resistors, use Ohm's law: I = (V1 - V2) / R
            if (auto resistor = dynamic_cast<Resistor*>(m_Component)) {
                return resistor->GetCurrent();
            }
            
            // For voltage sources, the current is stored after solving
            if (auto voltageSource = dynamic_cast<VoltageSource*>(m_Component)) {
                return voltageSource->GetCurrent();
            }
            
            return 0.0;
        }
    };
}
