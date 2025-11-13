#pragma once

#include "Eigen/Dense"
#include "Node.hpp"

namespace ecim {
    class Component {
    protected:
        Node *m_Node1 = nullptr, *m_Node2 = nullptr;
    
    public:
        virtual ~Component() {}
        
        void Connect(Node *node1, Node *node2) {
            m_Node1 = node1;
            m_Node2 = node2;
        }

        virtual void Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, double dt, int vsIndex) = 0;
    };
}
