"""
Functions for transforming pixel arrays
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['box_blur', 'flip', 'gaussian_blur', 'grayscale', 'invert', 'rotate', 'scale_by', 'scale_to']
def box_blur(pixel_array: pykraken._core.PixelArray, radius: typing.SupportsInt, repeat_edge_pixels: bool = True) -> pykraken._core.PixelArray:
    """
    Apply a box blur effect to a pixel array.
    
    Box blur creates a uniform blur effect by averaging pixels within a square kernel.
    It's faster than Gaussian blur but produces a more uniform, less natural look.
    
    Args:
        pixel_array (PixelArray): The pixel array to blur.
        radius (int): The blur radius in pixels. Larger values create stronger blur.
        repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                            outside the pixel array bounds. Defaults to True.
    
    Returns:
        PixelArray: A new pixel array with the box blur effect applied.
    
    Raises:
        RuntimeError: If pixel array creation fails during the blur process.
    """
def flip(pixel_array: pykraken._core.PixelArray, flip_x: bool, flip_y: bool) -> pykraken._core.PixelArray:
    """
    Flip a pixel array horizontally, vertically, or both.
    
    Args:
        pixel_array (PixelArray): The pixel array to flip.
        flip_x (bool): Whether to flip horizontally (mirror left-right).
        flip_y (bool): Whether to flip vertically (mirror top-bottom).
    
    Returns:
        PixelArray: A new pixel array with the flipped image.
    
    Raises:
        RuntimeError: If pixel array creation fails.
    """
def gaussian_blur(pixel_array: pykraken._core.PixelArray, radius: typing.SupportsInt, repeat_edge_pixels: bool = True) -> pykraken._core.PixelArray:
    """
    Apply a Gaussian blur effect to a pixel array.
    
    Gaussian blur creates a natural, smooth blur effect using a Gaussian distribution
    for pixel weighting. It produces higher quality results than box blur but is
    computationally more expensive.
    
    Args:
        pixel_array (PixelArray): The pixel array to blur.
        radius (int): The blur radius in pixels. Larger values create stronger blur.
        repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                            outside the pixel array bounds. Defaults to True.
    
    Returns:
        PixelArray: A new pixel array with the Gaussian blur effect applied.
    
    Raises:
        RuntimeError: If pixel array creation fails during the blur process.
    """
def grayscale(pixel_array: pykraken._core.PixelArray) -> pykraken._core.PixelArray:
    """
    Convert a pixel array to grayscale.
    
    Converts the pixel array to grayscale using the standard luminance formula:
    gray = 0.299 * red + 0.587 * green + 0.114 * blue
    
    This formula accounts for human perception of brightness across different colors.
    The alpha channel is preserved unchanged.
    
    Args:
        pixel_array (PixelArray): The pixel array to convert to grayscale.
    
    Returns:
        PixelArray: A new pixel array converted to grayscale.
    
    Raises:
        RuntimeError: If pixel array creation fails.
    """
def invert(pixel_array: pykraken._core.PixelArray) -> pykraken._core.PixelArray:
    """
    Invert the colors of a pixel array.
    
    Creates a negative image effect by inverting each color channel (RGB).
    The alpha channel is preserved unchanged.
    
    Args:
        pixel_array (PixelArray): The pixel array to invert.
    
    Returns:
        PixelArray: A new pixel array with inverted colors.
    
    Raises:
        RuntimeError: If pixel array creation fails.
    """
def rotate(pixel_array: pykraken._core.PixelArray, angle: typing.SupportsFloat) -> pykraken._core.PixelArray:
    """
    Rotate a pixel array by a given angle.
    
    Args:
        pixel_array (PixelArray): The pixel array to rotate.
        angle (float): The rotation angle in degrees. Positive values rotate clockwise.
    
    Returns:
        PixelArray: A new pixel array containing the rotated image. The output pixel array may be
                larger than the input to accommodate the rotated image.
    
    Raises:
        RuntimeError: If pixel array rotation fails.
    """
def scale_by(pixel_array: pykraken._core.PixelArray, factor: typing.SupportsFloat) -> pykraken._core.PixelArray:
    """
    Scale a pixel array by a given factor.
    
    Args:
        pixel_array (PixelArray): The pixel array to scale.
        factor (float): The scaling factor (must be > 0). Values > 1.0 enlarge,
                       values < 1.0 shrink the pixel array.
    
    Returns:
        PixelArray: A new pixel array scaled by the specified factor.
    
    Raises:
        ValueError: If factor is <= 0.
        RuntimeError: If pixel array creation or scaling fails.
    """
def scale_to(pixel_array: pykraken._core.PixelArray, size: pykraken._core.Vec2) -> pykraken._core.PixelArray:
    """
    Scale a pixel array to a new exact size.
    
    Args:
        pixel_array (PixelArray): The pixel array to scale.
        size (Vec2): The target size as (width, height).
    
    Returns:
        PixelArray: A new pixel array scaled to the specified size.
    
    Raises:
        RuntimeError: If pixel array creation or scaling fails.
    """
