#pragma once

#include "VoltageSource.hpp"
#include <cmath>

namespace ecim {
    // AC Voltage Source - sinusoidal voltage
    // V(t) = amplitude * sin(2 * pi * frequency * t + phase)
    class ACVoltageSource : public VoltageSource {
    private:
        double m_Amplitude;  // Peak voltage (V)
        double m_Frequency;  // Frequency in Hz
        double m_Phase;      // Phase offset in radians

    public:
        ACVoltageSource(double amplitude, double frequency, double phase = 0.0);
        double GetVoltage(double time) const override;
        
        // Getters for AC parameters
        double GetAmplitude() const { return m_Amplitude; }
        double GetFrequency() const { return m_Frequency; }
        double GetPhase() const { return m_Phase; }
    };
}
