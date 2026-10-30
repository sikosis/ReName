<h1 align="center">ReName</h1>

<p align="center">
  <img src="https://img.shields.io/badge/version-v0.14-f5a623" alt="Version v0.14">
  <img src="https://img.shields.io/badge/platform-Haiku-ffcc00" alt="Platform: Haiku">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599c" alt="C++17">
</p>

ReName is a Haiku Tracker add-on for previewing and safely applying batch file
renames. The current implementation is in [`ReName/`](ReName/); the historical
source is retained unchanged in [`ReName-Old/`](ReName-Old/).

Build and test on Haiku:

```sh
cd ReName
make
make test
make package
```

The HPKG is created under `ReName/build/`.
