#pragma once

#include "Component.hpp"

namespace ecim {
    class Inductor : public Component {
        double m_Inductance = 0.0;
        double m_Current = 0.0;  // Current through inductor at previous timestep

    public:
        Inductor(double inductance);
        void Stamp(SimulationState &state) override;
        void UpdateState();
        double GetCurrent() const;
    };
}
