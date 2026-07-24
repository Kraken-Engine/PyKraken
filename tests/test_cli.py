import pytest

from pykraken import cli


def test_python_project_template(tmp_path):
    project = tmp_path / "python-game"

    cli._write_project(project, "python")

    assert (project / "main.py").is_file()
    assert "import pykraken as kn" in (project / "main.py").read_text()
    assert (project / "README.md").is_file()
    assert (project / ".gitignore").is_file()


def test_cpp_project_template_uses_bundled_dependencies(tmp_path):
    project = tmp_path / "cpp-game"

    cli._write_project(project, "cpp")

    cmake = (project / "CMakeLists.txt").read_text()
    assert "KRAKEN_DEPENDENCIES BUNDLED" in cmake
    assert "Kraken::Kraken" in cmake
    assert f"GIT_TAG v{cli.VERSION}" in cmake
    assert (project / "src" / "main.cpp").is_file()


def test_cpp_sdk_template_uses_find_package(tmp_path):
    project = tmp_path / "sdk-game"

    cli._write_project(project, "cpp", sdk=True)

    cmake = (project / "CMakeLists.txt").read_text()
    preset = (project / "CMakePresets.json").read_text()
    assert "find_package(KrakenEngine" in cmake
    assert "FetchContent" not in cmake
    assert '"CMAKE_BUILD_TYPE": "Release"' in preset
    assert "${sourceDir}/.kraken/sdk" in preset


def test_project_template_refuses_to_overwrite(tmp_path):
    project = tmp_path / "existing"
    project.mkdir()
    (project / "main.py").write_text("keep me")

    with pytest.raises(FileExistsError):
        cli._write_project(project, "python")

    assert (project / "main.py").read_text() == "keep me"


@pytest.mark.parametrize(
    ("system", "machine", "suffix"),
    [
        ("Darwin", "arm64", "macos-arm64.tar.gz"),
        ("Windows", "AMD64", "windows-x64.zip"),
        ("Windows", "ARM64", "windows-arm64.zip"),
    ],
)
def test_sdk_asset_for_supported_platform(monkeypatch, system, machine, suffix):
    monkeypatch.setattr(cli.platform, "system", lambda: system)
    monkeypatch.setattr(cli.platform, "machine", lambda: machine)

    assert cli._sdk_asset().endswith(suffix)


def test_sdk_asset_rejects_unsupported_platform(monkeypatch):
    monkeypatch.setattr(cli.platform, "system", lambda: "Linux")
    monkeypatch.setattr(cli.platform, "machine", lambda: "x86_64")

    with pytest.raises(RuntimeError, match="bundled source build"):
        cli._sdk_asset()
