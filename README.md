# Trappable

Trappable is a modular Qt/C++ desktop application for scientific analysis of quadrupole mass filters using Mathieu stability diagrams.

## Features

- Interactive Mathieu stability diagram visualization
- Quadrupole mass filter analysis
- Modular C++ architecture with Qt GUI
- Cross-platform support (Windows focus)
- Standalone deployment with zero external dependencies

## Quick Start

## Building from Source

### Prerequisites

- **CMake** 3.16 or higher
- **C++17 compatible compiler** (Visual Studio 2022 recommended)
- **Qt 6.7+** installed at `C:\Qt\6.9.2\msvc2022_64\` (or modify CMakeLists.txt)
- **7-Zip** (for packaging)
- **Git** (for cloning)

### Simple Build Process

```cmd
# Clone the repository
git clone https://github.com/2ryan09/trappable.git
cd trappable

# Simple build and package
cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64 .
cmake --build build --config Release --target gui
cmake --build build --config Release --target package_simple

# Your standalone executable is now in: build/dist/

# Build the application (set environment variable to bypass Qt license check)
set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
cmake --build . --config Release --target gui

```

### What This Creates

- **Standalone executable**: `build/trappable_standalone/trappable.exe`
- **Zero dependencies**: All Qt libraries bundled automatically
- **Portable ZIP**: `build/trappable-YYYYMMDD-xxxxxxx-windows.zip` 
- **Ready to distribute**: Works on any Windows 10+ system

### Development Commands

```cmd
# Build just the application
cmake --build build --config Release --target gui

# Create standalone package (just 4 DLLs + 1 plugin)
cmake --build build --config Release --target package_simple

# Run tests
cd build && ctest --output-on-failure -C Release
```

## Project Architecture

```
trappable/
├── mathieu_lib/           # Pure C++17 computational core
├── gui/                   # Qt6 GUI components
│   ├── plot/             # Scientific plotting (QCustomPlot)
│   └── stability/        # Mathieu stability analysis
├── tests/                # GoogleTest unit tests
└── .github/workflows/    # Automated CI/CD
```

## Automated CI/CD

GitHub Actions automatically:
- ✅ **Tests** all code changes (Windows + core library tests)
- 🚀 **Builds** standalone executables on main branch
- 📦 **Packages** with Qt dependencies bundled
- ☁️ **Uploads** to Dropbox for distribution

### Dropbox Integration (Optional)

Add these GitHub repository secrets for automatic uploads:

1. Repository Settings → Secrets and variables → Actions
2. Add secrets:
   - `DROPBOX_APP_KEY` - Your Dropbox app key
   - `DROPBOX_APP_SECRET` - Your Dropbox app secret  
   - `DROPBOX_APP_SECRET` - Your Dropbox app secret
   - `DROPBOX_REFRESH_TOKEN` - Your Dropbox refresh token

Builds work without these secrets but won't upload to Dropbox.

## Troubleshooting

### "Could not find Qt6Widgets.dll"
**Solution**: Use the bundled package! The CMake build automatically includes all Qt libraries.

### Qt License Check Errors
If you see "Could not request license for qtframework":
```cmd
set QTFRAMEWORK_BYPASS_LICENSE_CHECK=1
cmake --build build --config Release --target package_zip
```

### Build Prerequisites
- **Qt 6.7+** must be installed at `C:\Qt\6.9.2\msvc2022_64\bin`
- **Visual Studio 2022** (Community Edition works)
- **7-Zip** for packaging (optional)

### Missing windeployqt
The build will warn if `windeployqt.exe` isn't found. Install Qt or adjust the path in `CMakeLists.txt`:
```cmake
set(QT_DEPLOY_PATH "C:/Your/Qt/Path/bin" CACHE PATH "Path to Qt deployment tools")
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make changes and test with `ctest`
4. Submit a pull request

All contributions are welcome! 🎉

