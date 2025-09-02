"""
Easing functions and animation utilities
"""
from __future__ import annotations
import typing
__all__: list[str] = ['in_back', 'in_bounce', 'in_circ', 'in_cubic', 'in_elastic', 'in_expo', 'in_out_back', 'in_out_bounce', 'in_out_circ', 'in_out_cubic', 'in_out_elastic', 'in_out_expo', 'in_out_quad', 'in_out_quart', 'in_out_quint', 'in_out_sin', 'in_quad', 'in_quart', 'in_quint', 'in_sin', 'linear', 'out_back', 'out_bounce', 'out_circ', 'out_cubic', 'out_elastic', 'out_expo', 'out_quad', 'out_quart', 'out_quint', 'out_sin']
def in_back(t: typing.SupportsFloat) -> float:
    """
    Back easing in (overshoot at start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_bounce(t: typing.SupportsFloat) -> float:
    """
    Bounce easing in (bounces toward target).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_circ(t: typing.SupportsFloat) -> float:
    """
    Circular easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_cubic(t: typing.SupportsFloat) -> float:
    """
    Cubic easing in (very slow start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_elastic(t: typing.SupportsFloat) -> float:
    """
    Elastic easing in (springy start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_expo(t: typing.SupportsFloat) -> float:
    """
    Exponential easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_back(t: typing.SupportsFloat) -> float:
    """
    Back easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_bounce(t: typing.SupportsFloat) -> float:
    """
    Bounce easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_circ(t: typing.SupportsFloat) -> float:
    """
    Circular easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_cubic(t: typing.SupportsFloat) -> float:
    """
    Cubic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_elastic(t: typing.SupportsFloat) -> float:
    """
    Elastic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_expo(t: typing.SupportsFloat) -> float:
    """
    Exponential easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_quad(t: typing.SupportsFloat) -> float:
    """
    Quadratic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_quart(t: typing.SupportsFloat) -> float:
    """
    Quartic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_quint(t: typing.SupportsFloat) -> float:
    """
    Quintic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_sin(t: typing.SupportsFloat) -> float:
    """
    Sinusoidal easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_quad(t: typing.SupportsFloat) -> float:
    """
    Quadratic easing in (slow start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_quart(t: typing.SupportsFloat) -> float:
    """
    Quartic easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_quint(t: typing.SupportsFloat) -> float:
    """
    Quintic easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_sin(t: typing.SupportsFloat) -> float:
    """
    Sinusoidal easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def linear(t: typing.SupportsFloat) -> float:
    """
    Linear easing.
    
    Args:
        t (float): Normalized time (0.0 to 1.0).
    Returns:
        float: Eased result.
    """
def out_back(t: typing.SupportsFloat) -> float:
    """
    Back easing out (overshoot at end).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_bounce(t: typing.SupportsFloat) -> float:
    """
    Bounce easing out (bounces after start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_circ(t: typing.SupportsFloat) -> float:
    """
    Circular easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_cubic(t: typing.SupportsFloat) -> float:
    """
    Cubic easing out (fast then smooth).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_elastic(t: typing.SupportsFloat) -> float:
    """
    Elastic easing out (springy end).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_expo(t: typing.SupportsFloat) -> float:
    """
    Exponential easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_quad(t: typing.SupportsFloat) -> float:
    """
    Quadratic easing out (fast start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_quart(t: typing.SupportsFloat) -> float:
    """
    Quartic easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_quint(t: typing.SupportsFloat) -> float:
    """
    Quintic easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_sin(t: typing.SupportsFloat) -> float:
    """
    Sinusoidal easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
