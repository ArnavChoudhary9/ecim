#pragma once

#include "Component.hpp"

namespace ecim {
    // Abstract base class for all voltage sources
    class VoltageSource : public Component {
    protected:
        double m_Current = 0.0; // Current through the voltage source

    public:
        virtual ~VoltageSource() {}
        
        // Get the voltage at a specific time
        virtual double GetVoltage(double time) const = 0;
        
        void Stamp(SimulationState &state) override;
        void SetCurrent(double current);
        double GetCurrent() const;
    };
}
