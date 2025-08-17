# IW4x (Microsoft Store x64 Research)

![Alt text](.github/screenshot.png?raw=true)

## Getting Started

IW4x uses the [build2](https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml#preface) build system. You can use either source or binary distribution packages for the staged toolchain. Binary packages are available at:

* https://stage.build2.org/0/0.18.0-a.0/bindist/
* SHA256 checksums: https://stage.build2.org/0/toolchain-bindist.sha256

IW4x also requires a working C++ compiler toolchain. On Linux, install and use MinGW; on Windows, use MSVC. Installation and usage instructions are available at:

* https://www.mingw-w64.org/
* https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022

### Consumption

> [!NOTE]
> Consumption is primarily intended for users who wish to test the current development version. It is not recommended for general end-user installation.

#### Windows

```powershell
# Create the build configuration:
bpkg create -d iw4x-msvc cc                       ^
  config.cxx=cl                                   ^
  config.cc.coptions=/O2                          ^
  config.install.root=C:/path/to/iw4/game/folder/ ^
cd iw4x-msvc

# To build:
bpkg build libiw4x@https://github.com/wroyca/iw4x-x64.git#main

# To test:
bpkg test libiw4x

# To install:
bpkg install libiw4x

# To uninstall:
bpkg uninstall libiw4x

# To upgrade:
bpkg fetch
bpkg status libiw4x
bpkg uninstall libiw4x
bpkg build --upgrade --recursive libiw4x
bpkg install libiw4x
```

#### Linux

```bash
# Create the build configuration:
bpkg create -d iw4x-mingw64 cc                 \
  config.cxx=x86_64-w64-mingw32-g++            \
  config.install.root=/path/to/iw4/game/folder
cd iw4x-mingw64

# To build:
bpkg build libiw4x@https://github.com/wroyca/iw4x-x64.git#main

# To test:
bpkg test libiw4x

# To install:
bpkg install libiw4x

# To uninstall:
bpkg uninstall libiw4x

# To upgrade:
bpkg fetch
bpkg status libiw4x
bpkg uninstall libiw4x
bpkg build --upgrade --recursive libiw4x
bpkg install libiw4x
```

Further information on these commands is available in the [build2 package consumption guide](https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml#guide-consume-pkg).

### Development

> [!NOTE]
> Development is primarily intended for developers who wish to work on the IW4x codebase. It is not recommended for general end-user installation or for those who only wish to test the current development version.

#### Linux

```bash
# Clone the repository:
git clone --recursive https://github.com/wroyca/iw4x-x64.git iw4x
cd iw4x

# Create the build configuration:
bdep init -C @mingw64 cc            \
  config.cxx=x86_64-w64-mingw32-g++ \
  config.cc.compiledb=./

# To build:
b

# To test:
b test
```

#### Windows

```powershell
# Clone the repository:
git clone --recursive https://github.com/wroyca/iw4x-x64.git iw4x
cd iw4x

# Create the build configuration:
bdep init -C -@msvc cc \
  config.cxx=cl

# To build:
b

# To test:
b test
```

Further information on bdep and build2 workflows is available in the [build2 build system manual](https://build2.org/build2/doc/build2-build-system-manual.xhtml).

## License

IW4x is distributed under the GPLv3. See [LICENSE.md](LICENSE.md) for details.
