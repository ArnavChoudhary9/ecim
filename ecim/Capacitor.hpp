#pragma once

#include "Component.hpp"

namespace ecim {
    class Capacitor : public Component {
        double m_Capacitance = 0.0;
        double m_Voltage = 0.0;      // Voltage across capacitor at previous timestep
        double m_Current = 0.0;       // Current through capacitor

    public:
        Capacitor(double capacitance);
        void Stamp(Eigen::MatrixXd &G, Eigen::VectorXd &I, double dt, int vsIndex, double time = 0.0) override;
        void UpdateState();
        double GetCurrent() const;
        void SetCurrent(double current);
    };
}
