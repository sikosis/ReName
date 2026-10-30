# ReName

ReName v0.14 is a Haiku Tracker add-on and standalone application for safely
renaming several files at once.

It supports:

- replacing a basename, with automatic numbering or `*` as the original-name
  placeholder;
- adding, replacing, or removing extensions;
- upper case, lower case, and capitalised-word conversion;
- case-sensitive search and replace; and
- a complete preview with conflict checking before files are changed.

## Build

On Haiku with the development tools installed:

```sh
make
make test
```

The executable is written to this directory as `ReName`.

## Install as a Tracker add-on

```sh
make install
```

Select files in Tracker, then choose **ReName** from the **Add-ons** menu. The
application can also be launched directly and files dropped into its window.

## Package

Build a native Haiku package:

```sh
make package
```

This runs the tests first. The package is written to
`build/rename-0.14-1-<architecture>.hpkg`. It installs the standalone application
and exposes the same executable as a Tracker add-on. Open the HPKG in HaikuDepot
to install it.

## Clean

```sh
make clean
```
