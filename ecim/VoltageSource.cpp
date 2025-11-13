#include "VoltageSource.hpp"

namespace ecim {
    void VoltageSource::Stamp(SimulationState &state) {
        int i = (m_Node1 && m_Node1->Id > 0) ? m_Node1->Id - 1 : -1;
        int j = (m_Node2 && m_Node2->Id > 0) ? m_Node2->Id - 1 : -1;
        
        // KCL rows
        // +1 at n1, -1 at n2
        if (i >= 0) state.G(i, state.vsIndex) += 1.0;
        if (j >= 0) state.G(j, state.vsIndex) -= 1.0;

        // Voltage constraint row +1 * V_i -1 * V_j = V_source
        if (i >= 0) state.G(state.vsIndex, i) += 1.0;
        if (j >= 0) state.G(state.vsIndex, j) -= 1.0;

        // Set the voltage source value in I using time-dependent voltage
        state.I(state.vsIndex) += GetVoltage(state.time);
    }

    void VoltageSource::SetCurrent(double current) {
        m_Current = current;
    }

    double VoltageSource::GetCurrent() const {
        return m_Current;
    }
}
