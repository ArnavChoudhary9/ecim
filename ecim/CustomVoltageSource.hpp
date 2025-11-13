#pragma once

#include "VoltageSource.hpp"
#include <functional>

namespace ecim {
    // Custom Voltage Source - user-defined voltage function
    // Allows arbitrary voltage waveforms via a callback function
    class CustomVoltageSource : public VoltageSource {
    private:
        std::function<double(double)> m_VoltageFunction;

    public:
        // Constructor takes a function that maps time -> voltage
        CustomVoltageSource(std::function<double(double)> voltageFunction);
        double GetVoltage(double time) const override;
    };
}
