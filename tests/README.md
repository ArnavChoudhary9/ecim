# ECIM Testing

This directory contains comprehensive tests for the ECIM circuit simulator.

## Test Structure

- `test_main.cpp` - Main test runner with simple test framework
- `test_components.cpp` - Tests for individual components (Resistor, Capacitor, Inductor, VoltageSource)
- `test_circuits.cpp` - Tests for complete circuits (voltage dividers, series/parallel circuits)
- `test_transient.cpp` - Tests for time-domain transient analysis

## Running Tests

### Windows (PowerShell/CMD)
```bash
run_tests.bat
```

### Linux/Mac
```bash
chmod +x run_tests.sh
./run_tests.sh
```

### Manual Build and Run
```bash
# Regenerate build files
premake5 gmake2

# Build tests
cd build
make config=debug_x64 ecim_tests

# Run tests
cd ..
./bin/Debug/ecim_tests
```

## Test Coverage

### Component Tests
- Node creation and ID assignment
- Resistor creation and Ohm's law calculations
- VoltageSource creation and current tracking
- Capacitor and Inductor creation
- Component-Node connections
- Probe voltage and current measurements

### Circuit Tests
- Voltage divider circuits (equal and unequal)
- Series resistor current calculations
- Parallel resistor configurations
- Multiple voltage sources
- Empty circuit handling
- Time tracking and reset

### Transient Analysis Tests
- RC charging circuits
- RL circuits
- LC oscillator circuits
- Capacitor state updates
- Simulation duration control
- Variable timestep handling
- RC time constant verification

## Test Framework

The tests use a simple custom testing framework with the following assertions:
- `assertTrue(condition, message)` - Assert a condition is true
- `assertFalse(condition, message)` - Assert a condition is false
- `assertEqual(actual, expected, tolerance, message)` - Assert values are equal within tolerance
- `assertNotNull(ptr, message)` - Assert pointer is not null

Tests are automatically discovered and run by the test runner.

## Exit Codes

- `0` - All tests passed
- `1` - One or more tests failed
