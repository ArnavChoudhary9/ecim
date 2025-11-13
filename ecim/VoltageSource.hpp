#pragma once

#include "Component.hpp"

namespace ecim {
    class VoltageSource : public Component {
        double m_Voltage = 0.0;

    public:
        VoltageSource(double voltage) : m_Voltage(voltage) {}

        void Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, int vsIndex) override {
            int i = (m_Node1 && m_Node1->Id > 0) ? m_Node1->Id - 1 : -1;
            int j = (m_Node2 && m_Node2->Id > 0) ? m_Node2->Id - 1 : -1;
            

            // KCL rows
            // +1 at n1, -1 at n2
            if (i >= 0) G(i, vsIndex) += 1.0;
            if (j >= 0) G(j, vsIndex) -= 1.0;

            // Voltage constraint row +1 * V_i -1 * V_j = V_source
            if (i >= 0) G(vsIndex, i) += 1.0;
            if (j >= 0) G(vsIndex, j) -= 1.0;

            // Set the voltage source value in I
            I(vsIndex) += m_Voltage;
        }
    };
}
