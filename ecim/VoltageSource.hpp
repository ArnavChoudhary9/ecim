#pragma once

#include "Component.hpp"

namespace ecim {
    class VoltageSource : public Component {
        double m_Voltage = 0.0;
        double m_Current = 0.0; // Current through the voltage source

    public:
        VoltageSource(double voltage);
        void Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, double dt, int vsIndex) override;
        void SetCurrent(double current);
        double GetCurrent() const;
    };
}
