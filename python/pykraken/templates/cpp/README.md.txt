# {{PROJECT_NAME}}

A C++20 game built with Kraken Engine {{KRAKEN_VERSION}}.

## Build and run

```bash
cmake --preset dev
cmake --build --preset dev
```

The default project builds Kraken and its dependencies from source. Projects created with
`kraken init --cpp --sdk` use a prebuilt SDK stored under `.kraken/sdk` instead.
