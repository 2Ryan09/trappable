# Trappable - AI Coding Instructions

## Project Overview
Trappable is a modular Qt/C++ desktop application for scientific analysis of quadrupole mass filters using Mathieu stability diagrams. The codebase follows a clear separation between computational core (`mathieu_lib`) and GUI components (`gui/`).

## Architecture Patterns

### Core Library Structure
- **`mathieu_lib/`**: Pure C++17 computational library with no Qt dependencies
- **`gui/`**: Qt6 application with modular component architecture
- Each GUI component is built as separate static libraries (see `gui/CMakeLists.txt`)
- Cross-platform deployment with automatic Qt bundling on Windows

### Key Component Boundaries
```
mathieu_lib (pure C++) → gui/MathieuBackend → gui/MathieuWindow
                      ↘ gui/stability/StabilityCalculator
                      ↘ gui/plot/StabilityRegionPlotter
```

### Testing Strategy
- GoogleTest for all components (both core lib and GUI)
- GUI tests disabled in CI environment via `DISABLED TRUE` when `ENV{CI}` is set
- Vectorized and end-to-end test separation (see `test_mathieu_vector.cpp`, `test_mathieu_e2e.cpp`)

## Development Workflows

### Build System (CMake)
```bash
# Standard build with Qt bundling
cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64 .
cmake --build build --config Release

# Create portable Windows distribution
cmake --build build --target package_zip --config Release
```

### Testing Patterns
- Run all tests: `cd build && ctest --output-on-failure -C Release`
- Unit tests use `EPSILON = 1e-9` for floating point comparisons
- Physics calculations tested against known mathematical relationships

### Qt-Specific Conventions
- `AUTOMOC ON` at project level, but explicitly `AUTOMOC OFF` for GoogleTest targets
- Resource files use Qt's qrc system (see `gui/icons.qrc`)
- Exception handling at application entry point (`gui/main.cpp`)

## Project-Specific Patterns

### Mathematical Computation Layer
- All physics calculations in `mathieu_lib` namespace
- Overloaded functions for single values and `std::vector` inputs
- Constants defined in `Constants.h` (e.g., `AVOGADRO_NUMBER`)
- `QuadrupoleParams` struct encapsulates physical parameters

### GUI Data Flow
- `MathieuBackend` acts as Qt-friendly wrapper around `mathieu_lib`
- Results exposed as formatted `QString` properties
- Stability calculations separated into dedicated `StabilityCalculator` namespace
- Custom plotting via QCustomPlot integration

### Windows Deployment
- Automatic `windeployqt` integration for portable distributions
- Git-based versioning: `trappable-{date}-{sha}-windows.zip`
- All Qt dependencies bundled for zero-install deployment

## Critical Files for Understanding
- `mathieu_lib/include/mathieu_lib/mathieu.h`: Core computational interface
- `gui/MathieuBackend.h`: Qt bridge layer
- `gui/stability/StabilityCalculator.h`: Stability analysis algorithms
- `CMakeLists.txt`: Build configuration and packaging logic
- `.github/workflows/ci.yml`: Windows-focused CI/deployment pipeline

## External Dependencies
- **Qt6**: Widgets, PrintSupport for GUI
- **QCustomPlot**: Scientific plotting (bundled in `gui/plot/QCustomPlot/`)
- **GoogleTest**: Testing framework (fetched via CMake)
- **7-Zip**: Windows packaging (external tool)