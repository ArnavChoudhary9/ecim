#include "test_framework.hpp"
#include "../ecim/Node.hpp"  // For resetting Node::nextId

// Forward declarations
void runBasicComponentTests(TestFramework::TestRunner& runner);
void runCircuitTests(TestFramework::TestRunner& runner);
void runTransientTests(TestFramework::TestRunner& runner);

int main() {
    TestFramework::TestRunner runner;

    std::cout << "Running ECIM Test Suite\n";
    std::cout << "Starting component tests...\n";
    std::cout.flush();

    ecim::Node::nextId = 0;  // Reset before each test category
    try {
        runBasicComponentTests(runner);
        std::cout << "Component tests completed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception in component tests: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Starting circuit tests...\n";
    std::cout.flush();

    ecim::Node::nextId = 0;  // Reset before each test category
    try {
        runCircuitTests(runner);
        std::cout << "Circuit tests completed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception in circuit tests: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Starting transient tests...\n";
    std::cout.flush();

    ecim::Node::nextId = 0;  // Reset before each test category
    try {
        runTransientTests(runner);
        std::cout << "Transient tests completed.\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception in transient tests: " << e.what() << "\n";
        return 1;
    }

    int failedCount = runner.printResults();
    return failedCount > 0 ? 1 : 0;
}
