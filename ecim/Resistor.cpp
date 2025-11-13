#include "Resistor.hpp"

namespace ecim {
    Resistor::Resistor(double resistance) : m_Resistance(resistance) {}

    void Resistor::Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, int /* vsIndex */) {
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

    // Calculate current through resistor using Ohm's law: I = (V1 - V2) / R
    // Current flows from m_Node1 to m_Node2
    double Resistor::GetCurrent() const {
        if (!m_Node1 || !m_Node2) return 0.0;
        return (m_Node1->Voltage - m_Node2->Voltage) / m_Resistance;
    }
}
