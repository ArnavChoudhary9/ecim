#include "CustomVoltageSource.hpp"

namespace ecim {
    CustomVoltageSource::CustomVoltageSource(std::function<double(double)> voltageFunction) 
        : m_VoltageFunction(voltageFunction) {}

    double CustomVoltageSource::GetVoltage(double time) const {
        return m_VoltageFunction(time);
    }
}
