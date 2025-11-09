from pydantic import BaseModel
import struct


class ShaderUniform(BaseModel):
    """Base model for shader uniform data structures.

    Subclass this to describe your shader's uniform layout. Instances can be
    converted to bytes for uploads through ``ShaderState.set_uniform()``. Field
    values are packed according to their types: ``float`` values use the ``f``
    format, ``int`` values use ``i``, ``bool`` values use ``?``, and tuples or
    lists of two to four floats are packed as vector components.
    """
    
    def to_bytes(self) -> bytes:
        """Serialize the uniform data into a packed binary format.

        The packing order follows the model's field order, and Python's ``struct``
        module determines the byte layout based on field types.

        Returns:
            bytes: Packed binary representation of the uniform data.

        Raises:
            ValueError: If a tuple or list field does not contain 2, 3, or 4 values.
            TypeError: If a field uses an unsupported type.
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
