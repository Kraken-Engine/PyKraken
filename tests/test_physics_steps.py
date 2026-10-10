import math

import pytest
import pykraken as kn


FIXED = 1 / 32


@pytest.fixture
def world():
    kn.init()
    kn.time.set_target(0)
    kn.time.set_scale(1)
    kn.window.is_open()  # Prime the engine clock.
    kn.physics.set_fixed_delta(FIXED)
    kn.physics.set_max_substeps(4)
    w = kn.physics.World()
    yield w
    del w
    kn.physics.set_fixed_delta(1 / 60)
    kn.time.set_scale(1)
    kn.time.set_max_delta(float("inf"))
    kn.quit()


def frame(delta):
    # Clamp real elapsed time, then scale it to a deterministic frame delta.
    kn.time.set_max_delta(0.001)
    kn.time.set_scale(delta / 0.001)
    kn.time.delay(2)
    kn.window.is_open()


def moving_body(world):
    body = kn.physics.RigidBody(world)
    body.add_collider(kn.Circle(kn.Vec2(0, 0), 0.5))
    body.linear_velocity = kn.Vec2(2, 0)
    return body


def test_accumulates_partial_frames_and_consumes_once(world):
    for _ in range(3):
        frame(FIXED / 4)
        assert kn.physics.fixed_steps() == []
    frame(FIXED / 4)
    assert kn.physics.fixed_steps() == [FIXED]
    assert kn.physics.fixed_steps() == []
    frame(FIXED * 2)
    assert kn.physics.fixed_steps() == [FIXED, FIXED]


def test_catch_up_limit_discards_excess(world):
    frame(FIXED * 100)
    assert kn.physics.fixed_steps() == [FIXED] * 8
    assert kn.physics.fixed_steps() == []
    frame(0)
    assert kn.physics.fixed_steps() == []


def test_paused_frames_and_timestep_changes(world):
    frame(FIXED / 2)
    assert kn.physics.fixed_steps() == []
    frame(0)
    assert kn.physics.fixed_steps() == []
    kn.physics.set_fixed_delta(FIXED * 2)
    frame(FIXED)
    assert kn.physics.fixed_steps() == []
    frame(FIXED)
    assert kn.physics.fixed_steps() == [FIXED * 2]


def test_worlds_only_advance_when_explicitly_stepped(world):
    other = kn.physics.World()
    body = moving_body(world)
    other_body = moving_body(other)
    frame(FIXED * 2)
    steps = kn.physics.fixed_steps()
    assert body.pos.x == 0
    assert other_body.pos.x == 0
    for dt in steps:
        world.step(dt)
    assert body.pos.x == pytest.approx(2 * FIXED * 2)
    assert other_body.pos.x == 0
    for dt in steps:
        other.step(dt)
    assert other_body.pos.x == pytest.approx(body.pos.x)


@pytest.mark.parametrize("delta", [0, -1, math.nan, math.inf, -math.inf])
def test_rejects_invalid_deltas(world, delta):
    with pytest.raises(ValueError):
        world.step(delta)
    with pytest.raises(ValueError):
        kn.physics.set_fixed_delta(delta)
    assert kn.physics.get_fixed_delta() == FIXED


def test_solver_substeps_do_not_change_catch_up_count(world):
    kn.physics.set_max_substeps(1)
    frame(FIXED * 12)
    assert len(kn.physics.fixed_steps()) == 8
    kn.physics.set_max_substeps(8)
    frame(FIXED * 12)
    assert len(kn.physics.fixed_steps()) == 8


def test_removed_callback_api(world):
    for name in ("fixed_callback", "add_fixed_update", "remove_fixed_update", "clear_fixed_updates"):
        assert not hasattr(world, name)


def test_restart_clears_pending_steps(world):
    frame(FIXED * 2)
    kn.quit()
    kn.init()
    assert kn.physics.fixed_steps() == []
