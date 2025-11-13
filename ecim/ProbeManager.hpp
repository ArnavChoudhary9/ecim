#pragma once
#include "Probe.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <iostream>

namespace ecim {
    enum class ProbeMode {
        Voltage,    // Measure voltage
        Current,    // Measure current
        Both        // Measure both voltage and current
    };

    struct ProbeConfig {
        ProbeMode mode = ProbeMode::Voltage;
        bool continuous = false;           // Enable continuous streaming
        std::ostream* stream = &std::cout; // Output stream (default: stdout)
        Node* node = nullptr;              // Node to probe (for voltage)
        Component* component = nullptr;    // Component to probe (for current)
        std::string label = "";            // Optional label for output
    };

    class ProbeManager {
        std::vector<Probe*> m_Probes;
        std::vector<ProbeConfig> m_Configs;

    public:
        ~ProbeManager();
        
        // Add a probe with configuration
        Probe* AddProbe(const ProbeConfig& config);
        
        // Update all continuous probes (call this at each simulation step)
        void UpdateContinuousProbes(double time);
        
        // Get all probes
        const std::vector<Probe*>& GetProbes() const;
        
        // Clear all probes
        void Clear();
    };
}
