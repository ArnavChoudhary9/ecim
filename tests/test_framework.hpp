// Simple test framework for ecim
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cmath>
#include "../ecim/Node.hpp"  // For resetting Node::nextId between tests

namespace TestFramework {
    struct TestResult {
        std::string name;
        bool passed;
        std::string message;
    };

    class TestRunner {
    private:
        std::vector<TestResult> results;
        std::string currentTest;

    public:
        void beginTest(const std::string& name) {
            currentTest = name;
        }

        void assertTrue(bool condition, const std::string& message = "") {
            if (!condition) {
                results.push_back({currentTest, false, message});
                throw std::runtime_error("Assertion failed: " + message);
            }
        }

        void assertFalse(bool condition, const std::string& message = "") {
            assertTrue(!condition, message);
        }

        void assertEqual(double actual, double expected, double tolerance = 1e-9, const std::string& message = "") {
            if (std::abs(actual - expected) > tolerance) {
                std::string msg = message + " (expected: " + std::to_string(expected) + 
                                ", actual: " + std::to_string(actual) + ")";
                results.push_back({currentTest, false, msg});
                throw std::runtime_error("Assertion failed: " + msg);
            }
        }

        void assertNotNull(void* ptr, const std::string& message = "") {
            assertTrue(ptr != nullptr, message);
        }

        void endTest(bool passed, const std::string& message = "") {
            results.push_back({currentTest, passed, message});
        }

        void runTest(const std::string& name, std::function<void(TestRunner&)> testFunc) {
            // Reset Node ID counter before each test to ensure ground is always node 0
            ecim::Node::nextId = 0;
            
            beginTest(name);
            std::cout << "  Running: " << name << "..." << std::flush;
            try {
                testFunc(*this);
                endTest(true, "OK");
                std::cout << " PASS\n";
            } catch (const std::exception& e) {
                std::cout << " FAIL\n";
                std::cerr << "    Error: " << e.what() << "\n";
                // Already recorded in assertion
            } catch (...) {
                endTest(false, "Unknown exception");
                std::cout << " FAIL (unknown exception)\n";
            }
        }

        int printResults() {
            int passed = 0;
            int failed = 0;

            std::cout << "\n========================================\n";
            std::cout << "Test Results\n";
            std::cout << "========================================\n";

            for (const auto& result : results) {
                if (result.passed) {
                    std::cout << "[PASS] " << result.name;
                    if (!result.message.empty() && result.message != "OK") {
                        std::cout << " - " << result.message;
                    }
                    std::cout << "\n";
                    passed++;
                } else {
                    std::cout << "[FAIL] " << result.name << " - " << result.message << "\n";
                    failed++;
                }
            }

            std::cout << "========================================\n";
            std::cout << "Total: " << (passed + failed) << " | Passed: " << passed 
                      << " | Failed: " << failed << "\n";
            std::cout << "========================================\n";

            return failed;
        }
    };
}
