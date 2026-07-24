# Releasing Kraken

Kraken uses a draft-first release process. Release binaries are built and tested before the
GitHub release is published or anything is uploaded to PyPI.

## Development builds

`build.yml` runs on pushes and pull requests. It builds the Python wheels, runs the Python test
matrix, and checks the native C++ build. Its artifacts are for development validation and are not
promoted to a release.

## Build a release candidate

1. Update the version in `pyproject.toml`, `vcpkg.json`, `CMakeLists.txt`, and
   `include/kraken/Runtime.hpp`.
2. Update `CHANGELOG.md` and commit the release candidate.
3. Run the **Build Draft Release** workflow from GitHub Actions on that commit.

The workflow:

- Builds the Windows x64, macOS ARM64, and Linux x64 Python wheels.
- Tests every wheel on Python 3.12, 3.13, and 3.14.
- Builds the macOS ARM64 and Windows x64 SDKs.
- Attempts the experimental Windows ARM64 SDK.
- Builds the complete bundled-source C++ route without vcpkg.
- Extracts each SDK archive and builds a standalone C++ consumer against it.
- Creates or updates a draft GitHub release and attaches the exact tested artifacts.

If a required build or test fails, no draft release is created. Fix the problem, commit it, and
run the workflow again. Existing draft assets with the same names are replaced.

## Review the draft

Before publishing:

1. Download and inspect the draft assets.
2. Optionally install a wheel in a clean virtual environment and run `kraken init`.
3. Optionally download an SDK and build a generated C++ project against it.
4. Review the generated release notes and changelog.
5. Confirm the draft targets the intended commit and tag.

The required release assets are three Python wheels plus the macOS ARM64 and Windows x64 SDKs.
Windows ARM64 is experimental and does not prevent creating the draft if it fails.

## Publish

Publish the draft from GitHub's release page. This triggers `release.yml`, which verifies the
required assets are attached and publishes those existing wheels to PyPI. It does not rebuild or
replace the release binaries.

PyPI Trusted Publishing must authorize `.github/workflows/release.yml` with the `release`
environment. If the repository is renamed, update the trusted publisher's repository name too.
