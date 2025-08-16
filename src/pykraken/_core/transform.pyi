"""
Functions for transforming surfaces
"""
from __future__ import annotations
import pykraken._core
__all__ = ['box_blur', 'flip', 'gaussian_blur', 'grayscale', 'invert', 'rotate', 'scale_by', 'scale_to']
def box_blur(surface: pykraken._core.Surface, radius: int, repeat_edge_pixels: bool = True) -> pykraken._core.Surface:
    """
    Apply a box blur effect to a surface.
    
    Box blur creates a uniform blur effect by averaging pixels within a square kernel.
    It's faster than Gaussian blur but produces a more uniform, less natural look.
    
    Args:
        surface (Surface): The surface to blur.
        radius (int): The blur radius in pixels. Larger values create stronger blur.
        repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                            outside the surface bounds. Defaults to True.
    
    Returns:
        Surface: A new surface with the box blur effect applied.
    
    Raises:
        RuntimeError: If surface creation fails during the blur process.
    """
def flip(surface: pykraken._core.Surface, flip_x: bool, flip_y: bool) -> pykraken._core.Surface:
    """
    Flip a surface horizontally, vertically, or both.
    
    Args:
        surface (Surface): The surface to flip.
        flip_x (bool): Whether to flip horizontally (mirror left-right).
        flip_y (bool): Whether to flip vertically (mirror top-bottom).
    
    Returns:
        Surface: A new surface with the flipped image.
    
    Raises:
        RuntimeError: If surface creation fails.
    """
def gaussian_blur(surface: pykraken._core.Surface, radius: int, repeat_edge_pixels: bool = True) -> pykraken._core.Surface:
    """
    Apply a Gaussian blur effect to a surface.
    
    Gaussian blur creates a natural, smooth blur effect using a Gaussian distribution
    for pixel weighting. It produces higher quality results than box blur but is
    computationally more expensive.
    
    Args:
        surface (Surface): The surface to blur.
        radius (int): The blur radius in pixels. Larger values create stronger blur.
        repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                            outside the surface bounds. Defaults to True.
    
    Returns:
        Surface: A new surface with the Gaussian blur effect applied.
    
    Raises:
        RuntimeError: If surface creation fails during the blur process.
    """
def grayscale(surface: pykraken._core.Surface) -> pykraken._core.Surface:
    """
    Convert a surface to grayscale.
    
    Converts the surface to grayscale using the standard luminance formula:
    gray = 0.299 * red + 0.587 * green + 0.114 * blue
    
    This formula accounts for human perception of brightness across different colors.
    The alpha channel is preserved unchanged.
    
    Args:
        surface (Surface): The surface to convert to grayscale.
    
    Returns:
        Surface: A new surface converted to grayscale.
    
    Raises:
        RuntimeError: If surface creation fails.
    """
def invert(surface: pykraken._core.Surface) -> pykraken._core.Surface:
    """
    Invert the colors of a surface.
    
    Creates a negative image effect by inverting each color channel (RGB).
    The alpha channel is preserved unchanged.
    
    Args:
        surface (Surface): The surface to invert.
    
    Returns:
        Surface: A new surface with inverted colors.
    
    Raises:
        RuntimeError: If surface creation fails.
    """
def rotate(surface: pykraken._core.Surface, angle: float) -> pykraken._core.Surface:
    """
    Rotate a surface by a given angle.
    
    Args:
        surface (Surface): The surface to rotate.
        angle (float): The rotation angle in degrees. Positive values rotate clockwise.
    
    Returns:
        Surface: A new surface containing the rotated image. The output surface may be
                larger than the input to accommodate the rotated image.
    
    Raises:
        RuntimeError: If surface rotation fails.
    """
def scale_by(surface: pykraken._core.Surface, factor: float) -> pykraken._core.Surface:
    """
    Scale a surface by a given factor.
    
    Args:
        surface (Surface): The surface to scale.
        factor (float): The scaling factor (must be > 0). Values > 1.0 enlarge,
                       values < 1.0 shrink the surface.
    
    Returns:
        Surface: A new surface scaled by the specified factor.
    
    Raises:
        ValueError: If factor is <= 0.
        RuntimeError: If surface creation or scaling fails.
    """
def scale_to(surface: pykraken._core.Surface, size: pykraken._core.Vec2) -> pykraken._core.Surface:
    """
    Scale a surface to a new exact size.
    
    Args:
        surface (Surface): The surface to scale.
        size (Vec2): The target size as (width, height).
    
    Returns:
        Surface: A new surface scaled to the specified size.
    
    Raises:
        RuntimeError: If surface creation or scaling fails.
    """
