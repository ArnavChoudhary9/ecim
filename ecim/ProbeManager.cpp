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
        m_HeaderWritten.push_back(false);  // CSV header not yet written
        
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
            
            if (config.format == ProbeOutputFormat::CSV) {
                // CSV format
                if (!m_HeaderWritten[i]) {
                    WriteCSVHeader(i, out);
                    m_HeaderWritten[i] = true;
                }
                WriteCSVData(i, probe, time, out);
            } else {
                // Standard format
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
        m_HeaderWritten.clear();
    }

    void ProbeManager::WriteCSVHeader(size_t probeIndex, std::ostream& out) {
        const ProbeConfig& config = m_Configs[probeIndex];
        
        // Write column headers
        out << "time";
        
        switch (config.mode) {
            case ProbeMode::Voltage:
                if (!config.label.empty()) {
                    out << "," << config.label << "_voltage";
                } else {
                    out << ",voltage";
                }
                break;
                
            case ProbeMode::Current:
                if (!config.label.empty()) {
                    out << "," << config.label << "_current";
                } else {
                    out << ",current";
                }
                break;
                
            case ProbeMode::Both:
                if (!config.label.empty()) {
                    if (config.node) {
                        out << "," << config.label << "_voltage";
                    }
                    if (config.component) {
                        out << "," << config.label << "_current";
                    }
                } else {
                    if (config.node) {
                        out << ",voltage";
                    }
                    if (config.component) {
                        out << ",current";
                    }
                }
                break;
        }
        
        out << std::endl;
    }

    void ProbeManager::WriteCSVData(size_t probeIndex, Probe* probe, double time, std::ostream& out) {
        const ProbeConfig& config = m_Configs[probeIndex];
        
        // Set precision for CSV output
        out << std::fixed << std::setprecision(9);
        
        // Write time
        out << time;
        
        // Write probe data based on mode
        switch (config.mode) {
            case ProbeMode::Voltage:
                out << "," << probe->Voltage();
                break;
                
            case ProbeMode::Current:
                out << "," << probe->Current();
                break;
                
            case ProbeMode::Both:
                if (config.node) {
                    out << "," << probe->Voltage();
                }
                if (config.component) {
                    out << "," << probe->Current();
                }
                break;
        }
        
        out << std::endl;
    }
}
