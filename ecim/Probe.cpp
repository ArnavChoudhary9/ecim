#include "Probe.hpp"
#include "Resistor.hpp"
#include "VoltageSource.hpp"

namespace ecim {
    Probe::Probe(Node* n) : m_Node(n), m_Component(nullptr) {}
    
    Probe::Probe(Component* c) : m_Node(nullptr), m_Component(c) {}
    
    double Probe::Voltage() const { 
        return m_Node ? m_Node->Voltage : 0.0; 
    }
    
    double Probe::Current() const {
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
}
