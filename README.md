# Simulation Database Abstraction Layer (SIMDAL)

The *Simulation Database Abstraction Layer* can be used to abstract database accesses to store tracing data during simulation.
It provides a thread-safe interface and backend implementations for SQLite and a CSV-based database.
The project contains predefined entry types which can be stored in the database.
Documentation of the resulting database tables can be found [here](./doc/table-description.md).

[![Build Status](https://github.com/nbosb/simdal/workflows/cmake/badge.svg?event=push)](https://github.com/nbosb/simdal/actions/workflows/cmake.yml)

---

## BUILD

1. Clone the git repository

    ```bash
    git clone https://github.com/nbosb/simdal.git
    ```

2. Choose directories for building and deployment

    ```text
    <source-dir>  location of your repo copy,     e.g. /home/nils/simdal
    <build-dir>   location to store object files, e.g. /home/nils/simdal/BUILD
    <install-dir> output directory for binaries,  e.g. /opt/simdal
    ```

3. Configure and build the SIMDAL using `cmake`.
    During configuration, you can specify the following options:

    * `-DSIMDAL_BUILD_TESTS=[ON|OFF]`: Build unit tests (default: `OFF`)

    Release and debug build configurations are controlled via the regular parameters (`-DCMAKE_BUILD_TYPE`):

    ```bash
    mkdir -p <build-dir>
    cd <build-dir>
    cmake -DCMAKE_INSTALL_PREFIX=<install-dir> -DCMAKE_BUILD_TYPE=Release <source-dir>
    make -j`nproc`
    make install
    ```

    If building with `-DSIMDAL_BUILD_TESTS=ON`, you can run all unit tests using `make test` within the `<build-dir>`

4. After installation, the following files and directories should be present:

    ```bash
    <install-dir>/include            # header files
    <install-dir>/lib64/libsimdal.a  # simdal library
    <install-dir>/tests              # unit tests (if enabled)
    ```

## Documentation

An overview of the created tables of the database can be found [here](./doc/table-description.md).

## License

This project is licensed under the Apache-2.0 License.
See the [LICENSE](./LICENSE) file for details.
