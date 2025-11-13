#pragma once

#include "VoltageSource.hpp"

namespace ecim {
    // DC Voltage Source - constant voltage
    class DCVoltageSource : public VoltageSource {
    private:
        double m_Voltage;

    public:
        DCVoltageSource(double voltage);
        double GetVoltage(double time) const override;
    };
}
