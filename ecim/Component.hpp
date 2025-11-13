#pragma once

#include "Eigen/Dense"
#include "Node.hpp"

namespace ecim {
    struct SimulationState {
        Eigen::MatrixXd &G;      // Conductance matrix
        Eigen::VectorXd &I;      // Current vector
        double dt;               // Time step
        int vsIndex;             // Voltage source index
        double time;             // Current simulation time
    };

    class Component {
    protected:
        Node *m_Node1 = nullptr, *m_Node2 = nullptr;
    
    public:
        virtual ~Component() {}
        
        void Connect(Node *node1, Node *node2) {
            m_Node1 = node1;
            m_Node2 = node2;
        }

        virtual void Stamp(SimulationState &state) = 0;
    };
}
