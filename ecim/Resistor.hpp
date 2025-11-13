#pragma once

#include "Component.hpp"

namespace ecim {
    class Resistor : public Component {
        double m_Resistance = 0.0;

    public:
        Resistor(double resistance);
        void Stamp(SimulationState &state) override;
        double GetCurrent() const;
    };
}
