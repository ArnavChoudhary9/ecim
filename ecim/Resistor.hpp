#pragma once

#include "Component.hpp"

namespace ecim {
    class Resistor : public Component {
        double m_Resistance = 0.0;

    public:
        Resistor(double resistance) : m_Resistance(resistance) {}

        void Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, int /* vsIndex */) override {
            double G_val = 1.0 / m_Resistance;

            // Skip ground node (node ID 0)
            int i = (m_Node1 && m_Node1->Id > 0) ? m_Node1->Id - 1 : -1;
            int j = (m_Node2 && m_Node2->Id > 0) ? m_Node2->Id - 1 : -1;

            // Diagonal contributions
            if (i >= 0) G(i, i) += G_val;
            if (j >= 0) G(j, j) += G_val;

            // Off-diagonal contributions
            if (i >= 0 && j >= 0) {
                G(i, j) -= G_val;
                G(j, i) -= G_val;
            }

            // No current source contribution for resistors
        }
    };
}
