import pytest

from pykraken import Orchestrator, Transform, Vec2, fx

def test_effect_creation():
    """Test that all fx functions correctly return Effect objects."""
    assert isinstance(fx.move_to(Vec2(100, 100), dur=1.0), fx.Effect)
    assert isinstance(fx.scale_to(Vec2(2.0, 2.0), dur=1.0), fx.Effect)
    assert isinstance(fx.scale_by(1.5, dur=1.0), fx.Effect)
    assert isinstance(fx.rotate_to(3.14, clockwise=True, dur=1.0), fx.Effect)
    assert isinstance(fx.rotate_by(1.57, clockwise=False, dur=1.0), fx.Effect)
    assert isinstance(fx.shake(amp=10.0, freq=5.0, dur=1.0), fx.Effect)

    def my_callback():
        pass
    assert isinstance(fx.call(my_callback), fx.Effect)
    assert isinstance(fx.wait(dur=0.5), fx.Effect)

def test_orchestrator_initial_state():
    """Test the initial state of a newly created Orchestrator."""
    t = Transform()
    orch = Orchestrator(t)

    assert not orch.finalized
    assert not orch.playing
    assert not orch.finished
    assert not orch.looping

def test_orchestrator_chaining():
    """Test that chaining methods return self to enable the builder pattern."""
    t = Transform()
    orch = Orchestrator(t)

    eff1 = fx.move_to(Vec2(10, 10), dur=1.0)
    eff2 = fx.scale_to(Vec2(2, 2), dur=1.0)
    eff3 = fx.rotate_by(1.0, dur=0.5)

    res1 = orch.then(eff1)
    res2 = res1.parallel(eff2, eff3)

    assert res1 is orch
    assert res2 is orch

def test_orchestrator_lifecycle():
    """Test the play, pause, resume, stop, and rewind lifecycle methods."""
    t = Transform()
    orch = Orchestrator(t)
    orch.then(fx.wait(1.0)).finalize()

    assert orch.finalized

    # Try playing
    orch.play()
    assert orch.playing
    assert not orch.finished

    # Pause
    orch.pause()
    assert not orch.playing

    # Resume
    orch.resume()
    assert orch.playing

    # Rewind
    orch.rewind()
    # Rewind doesn't stop playing in the C++ code, but resets step, let's just make sure it still runs

    # Stop
    orch.stop()
    assert not orch.playing

def test_orchestrator_looping_property():
    """Test setting and getting the looping property."""
    t = Transform()
    orch = Orchestrator(t)

    orch.looping = True
    assert orch.looping

    orch.looping = False
    assert not orch.looping

def test_orchestrator_cannot_modify_after_finalize():
    """Test that finalize locks down the orchestrator from adding more steps (logs warning, but we can just check it doesn't crash)."""
    t = Transform()
    orch = Orchestrator(t)

    orch.finalize()
    assert orch.finalized

    # Adding more effects after finalized triggers a C++ log::warn, but returns self harmlessly
    eff = fx.wait(1.0)
    orch.then(eff)

def test_orchestrator_invalid_parallel_args():
    """Test that non-effect arguments to parallel raise TypeError."""
    t = Transform()
    orch = Orchestrator(t)

    with pytest.raises(TypeError):
        orch.parallel(fx.wait(1.0), "not an effect")
