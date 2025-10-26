from pydantic import BaseModel
import struct


class ShaderUniform(BaseModel):
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
        fmt = ""
        values = []
        
        for name, value in self.model_dump().items():
            if isinstance(value, float):
                fmt += "f"; values.append(value)
            elif isinstance(value, int):
                fmt += "i"; values.append(value)
            elif isinstance(value, bool):
                fmt += "?"; values.append(value)
            elif isinstance(value, (tuple, list)):
                n = len(value)
                if n not in (2, 3, 4):
                    raise ValueError(f"Field '{name}' length {n} invalid, must be 2, 3, or 4.")
                fmt += f"{n}f"
                values.extend(map(float, value))
            else:
                raise TypeError(f"Unsupported uniform field '{name}' of type '{type(value)}'")

        return struct.pack(fmt, *values)
