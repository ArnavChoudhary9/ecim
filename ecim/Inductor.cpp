#include "Inductor.hpp"

namespace ecim {
    Inductor::Inductor(double inductance) 
        : m_Inductance(inductance), m_Current(0.0) {}

    // For transient analysis using backward Euler method
    // The inductor is modeled as: V = L * dI/dt
    // Using backward Euler: V(t) = L * (I(t) - I(t-dt)) / dt
    // Rearranging: I(t) = I(t-dt) + V(t) * dt / L
    // This is equivalent to: V(t) = (L/dt) * I(t) - (L/dt) * I(t-dt)
    // The inductor acts like a resistor (L/dt) with a voltage source -(L/dt)*I(t-dt)
    void Inductor::Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, double dt, int /* vsIndex */, double /* time */) {
        if (dt <= 0.0) return;

        double Req = m_Inductance / dt;  // Equivalent resistance
        double Veq = Req * m_Current;     // Equivalent voltage source

        int i = (m_Node1 && m_Node1->Id > 0) ? m_Node1->Id - 1 : -1;
        int j = (m_Node2 && m_Node2->Id > 0) ? m_Node2->Id - 1 : -1;

        // Add equivalent resistance (like resistor)
        double G_val = 1.0 / Req;
        if (i >= 0) G(i, i) += G_val;
        if (j >= 0) G(j, j) += G_val;
        if (i >= 0 && j >= 0) {
            G(i, j) -= G_val;
            G(j, i) -= G_val;
        }

        // Add equivalent voltage source (like current source)
        double Ieq = Veq / Req;  // Convert to current
        if (i >= 0) I(i) -= Ieq;
        if (j >= 0) I(j) += Ieq;
    }

    void Inductor::UpdateState() {
        // Update current through inductor for next timestep
        if (m_Node1 && m_Node2) {
            m_Current = (m_Node1->Voltage - m_Node2->Voltage) / (m_Inductance / 1e-6);  // Approximate
        }
    }

    double Inductor::GetCurrent() const {
        return m_Current;
    }
}
