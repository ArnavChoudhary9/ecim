# ecim
Electronics Circuit Simulator written in C++

## Features

- Time-domain transient analysis using backward Euler method
- Support for basic components: Resistors, Capacitors, Inductors, Voltage Sources
- Node-based circuit construction
- Probes for measuring voltages and currents
- Comprehensive test suite

## Building

### Prerequisites
- C++17 compiler (GCC, Clang, MSVC)
- Make
- Eigen library (included in vendor/)

### Build the Main Application
```bash
cd build
make
```

### Build and Run Tests
```bash
# Using the test script (Windows)
run_tests.bat

# Or manually
cd build
make -f Makefile.tests
cd ..
.\bin\Debug\ecim_tests.exe
```

## Testing

The project includes comprehensive tests covering:
- Component functionality
- Circuit analysis
- Transient simulations

See `tests/README.md` for detailed testing documentation.

## License

See LICENSE file for details.

