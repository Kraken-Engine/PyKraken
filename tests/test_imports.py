def test_import_shader_uniform_module():
    # Importing the module should work (conftest.py adds src/ to sys.path)
    from pykraken import shader_uniform
    assert hasattr(shader_uniform, "ShaderUniform")
