#include "ProbeManager.hpp"
#include <iomanip>

namespace ecim {
    ProbeManager::~ProbeManager() {
        Clear();
    }

    Probe* ProbeManager::AddProbe(const ProbeConfig& config) {
        Probe* probe = nullptr;
        
        // Create probe based on what we're measuring
        if (config.node) {
            probe = new Probe(config.node);
        } else if (config.component) {
            probe = new Probe(config.component);
        } else {
            // Invalid config - must have either node or component
            return nullptr;
        }
        
        m_Probes.push_back(probe);
        m_Configs.push_back(config);
        
        return probe;
    }

    void ProbeManager::UpdateContinuousProbes(double time) {
        for (size_t i = 0; i < m_Probes.size(); ++i) {
            const ProbeConfig& config = m_Configs[i];
            
            if (!config.continuous || !config.stream) {
                continue;
            }
            
            Probe* probe = m_Probes[i];
            std::ostream& out = *config.stream;
            
            // Format output
            out << std::fixed << std::setprecision(6);
            out << "t=" << time << "s";
            
            if (!config.label.empty()) {
                out << " [" << config.label << "]";
            }
            
            // Output based on mode
            switch (config.mode) {
                case ProbeMode::Voltage:
                    out << " V=" << probe->Voltage() << "V";
                    break;
                    
                case ProbeMode::Current:
                    out << " I=" << probe->Current() << "A";
                    break;
                    
                case ProbeMode::Both:
                    if (config.node) {
                        out << " V=" << probe->Voltage() << "V";
                    }
                    if (config.component) {
                        out << " I=" << probe->Current() << "A";
                    }
                    break;
            }
            
            out << std::endl;
        }
    }

    const std::vector<Probe*>& ProbeManager::GetProbes() const {
        return m_Probes;
    }

    void ProbeManager::Clear() {
        for (auto probe : m_Probes) {
            delete probe;
        }
        m_Probes.clear();
        m_Configs.clear();
    }
}
