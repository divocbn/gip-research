# gip_gfx

_gip_gfx_ is a thin wrapper around [_SDL2_](https://libsdl.org) hopefully providing a gentle introduction to basic graphics programming.

It is currently still a work in progress.
However, it is being actively used in an undergraduate computer science course at [Westphalian University of Applied Sciences](https://w-hs.de)..


## Prerequisites

- [_SDL2_](https://libsdl.org)
- [_CMake_](https://cmake.org/) >= 3.23

_gip_gfx_ requires [_SDL2_](https://libsdl.org) being installed, as it is wrapping their functionality, providing a simpler interface.
In order to compile, _gip_gfx_ the [_SDL2_](https://libsdl.org) development headers need to be installed alongside the library itself.
Ideally, use your operating system's package manager to install _SLD2_.

[_CMake_](https://cmake.org/) is being used as a build system.


## Installation

- Download or clone _gip_gfx_' source code into a directory of your choice -- later called ``SOURCE_DIR``.
- Decide upon and create an installation directory -- later called ``INSTALL_DIR`` --, e.g., ``~/opt/gip_gfx/`` for your local user account or ``/opt/gip_gfx`` for all users.
- Open a terminal in ``SOURCE_DIR`` and issue the following commands to first build and then install _gip_gfx_.

``` bash
$ mkdir build 
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=INSTALL_DIR ..
$ cmake --build .
$ cmake --install .
```

- Add ``INSTALL_DIR`` to your ``PATH`` environment variable.

If you know what this is about -- or if you are curious to learn --, you might want to use [_GNU stow_](https://www.gnu.org/software/stow/) to link _gip_gfx_' install tree into, e.g., ``/usr/local``.


## Examples

_gip_gfx_ provides source code examples to get you started:
just copy them into a new directory and hack away.

The examples are located in the ``examples`` subdirectory of _gip_gfx_' source directory ``SOURCE_DIR``.
If installed properly, you'll also find the examples' source code in ``INSTALL_DIR/share/gip_gfx/examples``.
Please note that the latter contains ready-to-use examples while the former require some adjustments in the respective ``CMakeLists.txt`` file.
The required adjustments are denoted by comments in the respective ``CMakeLists.txt`` file.
If you do not want to bother, better copy the examples from the location inside ``INSTALL_DIR``.

When building and installing _gip_gfx_ from source, the examples are compiled and installed as well.
This serves as a test for _gip_gfx_.
If everything went well, you'll find the examples ready to run in ``SOURCE_DIR/build/examples`` and ``INSTALL_DIR/bin/gip_gfx/examples``.
