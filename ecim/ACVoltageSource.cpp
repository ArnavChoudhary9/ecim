#include "ACVoltageSource.hpp"

namespace ecim {
    ACVoltageSource::ACVoltageSource(double amplitude, double frequency, double phase) 
        : m_Amplitude(amplitude), m_Frequency(frequency), m_Phase(phase) {}

    double ACVoltageSource::GetVoltage(double time) const {
        const double PI = 3.14159265358979323846;
        return m_Amplitude * std::sin(2.0 * PI * m_Frequency * time + m_Phase);
    }
}
