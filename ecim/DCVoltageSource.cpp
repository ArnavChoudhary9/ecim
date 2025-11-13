#include "DCVoltageSource.hpp"

namespace ecim {
    DCVoltageSource::DCVoltageSource(double voltage) 
        : m_Voltage(voltage) {}

    double DCVoltageSource::GetVoltage(double /* time */) const {
        return m_Voltage;
    }
}
