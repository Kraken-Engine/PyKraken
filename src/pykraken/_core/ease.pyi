"""
Easing functions and animation utilities
"""
from __future__ import annotations
__all__ = ['in_back', 'in_bounce', 'in_circ', 'in_cubic', 'in_elastic', 'in_expo', 'in_out_back', 'in_out_bounce', 'in_out_circ', 'in_out_cubic', 'in_out_elastic', 'in_out_expo', 'in_out_quad', 'in_out_quart', 'in_out_quint', 'in_out_sine', 'in_quad', 'in_quart', 'in_quint', 'in_sine', 'linear', 'out_back', 'out_bounce', 'out_circ', 'out_cubic', 'out_elastic', 'out_expo', 'out_quad', 'out_quart', 'out_quint', 'out_sine']
def in_back(t: float) -> float:
    """
    Back easing in (overshoot at start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_bounce(t: float) -> float:
    """
    Bounce easing in (bounces toward target).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_circ(t: float) -> float:
    """
    Circular easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_cubic(t: float) -> float:
    """
    Cubic easing in (very slow start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_elastic(t: float) -> float:
    """
    Elastic easing in (springy start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_expo(t: float) -> float:
    """
    Exponential easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_back(t: float) -> float:
    """
    Back easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_bounce(t: float) -> float:
    """
    Bounce easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_circ(t: float) -> float:
    """
    Circular easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_cubic(t: float) -> float:
    """
    Cubic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_elastic(t: float) -> float:
    """
    Elastic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_expo(t: float) -> float:
    """
    Exponential easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_quad(t: float) -> float:
    """
    Quadratic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_quart(t: float) -> float:
    """
    Quartic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_quint(t: float) -> float:
    """
    Quintic easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_out_sine(t: float) -> float:
    """
    Sinusoidal easing in and out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_quad(t: float) -> float:
    """
    Quadratic easing in (slow start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_quart(t: float) -> float:
    """
    Quartic easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_quint(t: float) -> float:
    """
    Quintic easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def in_sine(t: float) -> float:
    """
    Sinusoidal easing in.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def linear(t: float) -> float:
    """
    Linear easing.
    
    Args:
        t (float): Normalized time (0.0 to 1.0).
    Returns:
        float: Eased result.
    """
def out_back(t: float) -> float:
    """
    Back easing out (overshoot at end).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_bounce(t: float) -> float:
    """
    Bounce easing out (bounces after start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_circ(t: float) -> float:
    """
    Circular easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_cubic(t: float) -> float:
    """
    Cubic easing out (fast then smooth).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_elastic(t: float) -> float:
    """
    Elastic easing out (springy end).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_expo(t: float) -> float:
    """
    Exponential easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_quad(t: float) -> float:
    """
    Quadratic easing out (fast start).
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_quart(t: float) -> float:
    """
    Quartic easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_quint(t: float) -> float:
    """
    Quintic easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
def out_sine(t: float) -> float:
    """
    Sinusoidal easing out.
    
    Args:
        t (float): Normalized time.
    Returns:
        float: Eased result.
    """
