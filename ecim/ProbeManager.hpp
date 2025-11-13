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

    enum class ProbeOutputFormat {
        Standard,   // Human-readable format: "t=0.001s [Label] V=5.0V"
        CSV         // CSV format: "0.001,5.0"
    };

    struct ProbeConfig {
        ProbeMode mode = ProbeMode::Voltage;
        bool continuous = false;              // Enable continuous streaming
        std::ostream* stream = &std::cout;    // Output stream (default: stdout)
        Node* node = nullptr;                 // Node to probe (for voltage)
        Component* component = nullptr;       // Component to probe (for current)
        std::string label = "";               // Optional label for output
        ProbeOutputFormat format = ProbeOutputFormat::Standard; // Output format
    };

    class ProbeManager {
        std::vector<Probe*> m_Probes;
        std::vector<ProbeConfig> m_Configs;
        std::vector<bool> m_HeaderWritten;  // Track if CSV header has been written

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
        
    private:
        // Write CSV header for a probe
        void WriteCSVHeader(size_t probeIndex, std::ostream& out);
        
        // Write CSV data row for a probe
        void WriteCSVData(size_t probeIndex, Probe* probe, double time, std::ostream& out);
    };
}
