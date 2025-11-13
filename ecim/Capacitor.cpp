#include "Capacitor.hpp"

namespace ecim {
    Capacitor::Capacitor(double capacitance) 
        : m_Capacitance(capacitance), m_Voltage(0.0), m_Current(0.0) {}

    // For transient analysis using backward Euler method
    // The capacitor is modeled as: I = C * dV/dt
    // Using backward Euler: I(t) = C * (V(t) - V(t-dt)) / dt
    // Rearranging: V(t) = V(t-dt) + I(t) * dt / C
    // This is equivalent to: I(t) = (C/dt) * V(t) - (C/dt) * V(t-dt)
    // Which looks like a conductance (C/dt) with a current source -(C/dt)*V(t-dt)
    void Capacitor::Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, int /* vsIndex */) {
        // This is for DC analysis - capacitor acts as open circuit
        // No contribution to the system
    }

    void Capacitor::StampTransient(Eigen::MatrixXd &G, Eigen::VectorXd &I, double dt) {
        if (dt <= 0.0) return;

        double Geq = m_Capacitance / dt;  // Equivalent conductance
        double Ieq = Geq * m_Voltage;      // Equivalent current source

        int i = (m_Node1 && m_Node1->Id > 0) ? m_Node1->Id - 1 : -1;
        int j = (m_Node2 && m_Node2->Id > 0) ? m_Node2->Id - 1 : -1;

        // Add equivalent conductance (like resistor)
        if (i >= 0) G(i, i) += Geq;
        if (j >= 0) G(j, j) += Geq;
        if (i >= 0 && j >= 0) {
            G(i, j) -= Geq;
            G(j, i) -= Geq;
        }

        // Add equivalent current source
        if (i >= 0) I(i) += Ieq;
        if (j >= 0) I(j) -= Ieq;
    }

    void Capacitor::UpdateState() {
        // Update voltage across capacitor for next timestep
        if (m_Node1 && m_Node2) {
            m_Voltage = m_Node1->Voltage - m_Node2->Voltage;
        }
    }

    double Capacitor::GetCurrent() const {
        return m_Current;
    }

    void Capacitor::SetCurrent(double current) {
        m_Current = current;
    }
}
