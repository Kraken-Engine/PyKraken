from __future__ import annotations
import inspect
import pydantic._internal._decorators
import pydantic.main
from pydantic.main import BaseModel
import pydantic_core._pydantic_core
import struct as struct
import typing
__all__: list[str] = ['BaseModel', 'ShaderUniform', 'struct']
class ShaderUniform(pydantic.main.BaseModel):
    """
    
    Base class for shader uniform data structures.
    
    Inherits from Pydantic's BaseModel to provide type validation and serialization
    for shader uniform buffers. Subclass this to define your shader's uniform layout,
    and use `to_bytes()` to convert the data to a binary format suitable for upload
    to the GPU via `ShaderState.set_uniform()`.
    
    Supported field types:
        - float: Packed as 'f' (4 bytes)
        - int: Packed as 'i' (4 bytes, signed)
        - bool: Packed as '?' (1 byte)
        - tuple/list of 2-4 floats: Packed as vectors (vec2, vec3, vec4)
    """
    __abstractmethods__: typing.ClassVar[frozenset]  # value = frozenset()
    __class_vars__: typing.ClassVar[set] = set()
    __private_attributes__: typing.ClassVar[dict] = {}
    __pydantic_complete__: typing.ClassVar[bool] = True
    __pydantic_computed_fields__: typing.ClassVar[dict] = {}
    __pydantic_core_schema__: typing.ClassVar[dict] = {'type': 'model', 'cls': ShaderUniform, 'schema': {'type': 'model-fields', 'fields': {}, 'model_name': 'ShaderUniform', 'computed_fields': list()}, 'custom_init': False, 'root_model': False, 'config': {'title': 'ShaderUniform'}, 'ref': 'pykraken.shader_uniform.ShaderUniform:2326036864224', 'metadata': {'pydantic_js_functions': [pydantic.main.BaseModel.__get_pydantic_json_schema__]}}
    __pydantic_custom_init__: typing.ClassVar[bool] = False
    __pydantic_decorators__: typing.ClassVar[pydantic._internal._decorators.DecoratorInfos]  # value = DecoratorInfos(validators={}, field_validators={}, root_validators={}, field_serializers={}, model_serializers={}, model_validators={}, computed_fields={})
    __pydantic_fields__: typing.ClassVar[dict] = {}
    __pydantic_generic_metadata__: typing.ClassVar[dict] = {'origin': None, 'args': tuple(), 'parameters': tuple()}
    __pydantic_parent_namespace__ = None
    __pydantic_post_init__ = None
    __pydantic_serializer__: typing.ClassVar[pydantic_core._pydantic_core.SchemaSerializer]  # value = SchemaSerializer(serializer=Model(...
    __pydantic_setattr_handlers__: typing.ClassVar[dict] = {}
    __pydantic_validator__: typing.ClassVar[pydantic_core._pydantic_core.SchemaValidator]  # value = SchemaValidator(title="ShaderUniform", validator=Model(...
    __signature__: typing.ClassVar[inspect.Signature]  # value = <Signature () -> None>
    _abc_impl: typing.ClassVar[_abc._abc_data]  # value = <_abc._abc_data object>
    model_config: typing.ClassVar[dict] = {}
    def to_bytes(self) -> bytes:
        """
        
        Converts the uniform data to a packed binary format.
        
        Serializes all fields in the model to a bytes object using Python's struct
        module, suitable for uploading to GPU uniform buffers. The packing format
        is automatically determined based on field types.
        
        Returns:
            bytes: The packed binary representation of the uniform data.
        
        Raises:
            ValueError: If a tuple/list field has an invalid length (not 2, 3, or 4).
            TypeError: If a field has an unsupported type.
        """
