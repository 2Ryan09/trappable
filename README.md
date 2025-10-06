# Trappable

Trappable is a modular Qt/C++ desktop application for scientific analysis of quadrupole mass filters using Mathieu stability diagrams.

## Features

- Interactive Mathieu stability diagram visualization
- Quadrupole mass filter analysis
- Modular C++ architecture with Qt GUI
- Cross-platform support (Windows, Linux)
- Portable deployment with bundled Qt libraries

## Downloads

### Pre-built Releases

The easiest way to get Trappable is to download a pre-built release:

1. Go to the [Releases page](https://github.com/2ryan09/trappable/releases)
2. Download the appropriate package for your system:
   - **Windows**: `trappable.zip` - Contains all Qt libraries, no installation required

### Windows Portable Distribution

The Windows portable distribution includes all necessary Qt libraries and can run on any Windows system without requiring Qt to be pre-installed.

Simply extract the ZIP file and run `gui.exe`.

## Building from Source

### Prerequisites

- **CMake** 3.16 or higher
- **C++17 compatible compiler**
- **Qt 6.5** or higher
- **Git** (for cloning the repository)

### Windows Build Instructions

```cmd
# Clone the repository
git clone https://github.com/2ryan09/trappable.git
cd trappable

# Create build directory
mkdir build
cd build

# Configure (Qt bundling is always enabled, export compile commands for clang-tidy)
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G "Visual Studio 17 2022" -A x64 ..

# Build the application (set environment variable to bypass Qt license check)
set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
cmake --build . --config Release --target gui

# Create portable ZIP package
cmake --build . --target package_zip --config Release
```

The portable distribution will be created as `build/trappable-{version}-{sha}-windows.zip` (e.g., `trappable-20241006-a1b2c3d4-windows.zip`) and can be distributed to any Windows system.

**Note**: This project is designed for Windows deployment only. Linux support has been removed to simplify the MVP.

### Build Options

The following CMake options are available:

- `BUILD_TESTS=ON/OFF` - Build test executables (default: ON)

**Note**: Qt deployment is always enabled for this Windows-focused MVP project.

## Development

### Running Tests

```cmd
# After building
cd build
ctest --output-on-failure
```

### Project Structure

```
trappable/
├── mathieu_lib/           # Core mathematical computations
├── gui/                   # Qt GUI application
│   ├── plot/             # Plotting components
│   └── stability/        # Stability analysis GUI
├── tests/                # Unit and integration tests
└── .github/workflows/    # CI/CD workflows
```

## Automated Builds

GitHub Actions automatically builds and tests the application on Windows for every commit and pull request. Builds on the main branch are uploaded to artifacts and optionally to Dropbox.

The workflow creates portable Windows distributions that include all Qt libraries and can run on any Windows system without Qt installation.

### Setting up Dropbox Deployment (Optional)

To enable automatic Dropbox uploads for builds, add these secrets to your GitHub repository:

1. Go to Repository Settings > Secrets and variables > Actions
2. Add the following repository secrets:
   - `DROPBOX_APP_KEY` - Your Dropbox app key
   - `DROPBOX_APP_SECRET` - Your Dropbox app secret  
   - `DROPBOX_REFRESH_TOKEN` - Your Dropbox refresh token

If these secrets are not configured, builds will still work but won't upload to Dropbox.

## Troubleshooting

### Windows: Qt License Check Errors

If you get errors like "Could not request license for qtframework" during build:

```cmd
# Set this environment variable before building
set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
cmake --build . --config Release --target gui
```

Or in PowerShell:
```powershell
$env:QTFRAMEWORK_BYPASS_LICENSE_CHECK = "1"
cmake --build . --config Release --target gui
```

This bypasses the Qt license check for open-source usage.

### Windows: Missing Qt Libraries

This should not happen with the current build system since Qt is always bundled. If you encounter DLL issues:

1. Make sure you're using the portable distribution from `build/trappable.zip`
2. Extract the entire ZIP contents to a folder and run `gui.exe` from there
3. Download the pre-built portable distribution from GitHub releases

