IW4x (Microsoft Store x64 Research)

![Alt text](.github/screenshot.png?raw=true)

This file contains setup instructions and other details that are more
appropriate for development rather than consumption.

The development setup for `iw4x` uses the standard `bdep`-based workflow.
For example:

```
git clone .../iw4x.git
cd iw4x

bdep init -C @mingw64 config.cxx=x86_64-w64-mingw32-g++ cc config.cc.compiledb=./
bdep update
bdep test
```
