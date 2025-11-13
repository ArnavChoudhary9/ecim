#pragma once
#include "Node.hpp"

namespace ecim {
    class Probe {
        Node* m_Node;
    public:
        Probe(Node* n) : m_Node(n) {}
        
        double Voltage() const { 
            return m_Node ? m_Node->Voltage : 0.0; 
        }
    };
}
