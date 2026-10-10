import pytest
import pykraken as kn


@pytest.fixture
def world():
    # Physics does not require a window or SDL initialization.
    return kn.physics.World()


def pair(world, sensor=False, events=True):
    fixed = kn.physics.StaticBody(world)
    moving = kn.physics.RigidBody(world)
    fixed.add_collider(kn.Circle(kn.Vec2(0, 0), 1), enable_events=events, is_sensor=sensor)
    moving.add_collider(kn.Circle(kn.Vec2(0, 0), 0.5), enable_events=events)
    return fixed, moving


@pytest.mark.parametrize("sensor", [False, True])
def test_begin_end_and_snapshot_lifetime(world, sensor):
    fixed, moving = pair(world, sensor)
    read = world.get_sensor_events if sensor else world.get_contact_events
    assert read() == []
    world.step(1 / 60)
    events = read()
    assert len(events) == 1 and events[0].began
    event = events[0]
    a, b = (event.sensor, event.visitor) if sensor else (event.collider_a, event.collider_b)
    expected = {fixed.get_colliders()[0].id, moving.get_colliders()[0].id}
    assert {a.id, b.id} == expected
    assert a.is_valid and b.is_valid
    assert a.body == fixed or a.body == moving
    assert len(read()) == 1  # Reads never consume.
    world.step(1 / 60)
    assert read() == []  # Persistent contact is not another begin.
    assert event.began and {a.id, b.id} == expected
    moving.pos = kn.Vec2(10, 0)
    world.step(1 / 60)
    ended = read()
    assert len(ended) == 1 and not ended[0].began
    ea, eb = (ended[0].sensor, ended[0].visitor) if sensor else (ended[0].collider_a, ended[0].collider_b)
    assert {ea.id, eb.id} == expected
    assert event.began  # Earlier Python snapshot remains unchanged.
    world.step(1 / 60)
    assert read() == []


@pytest.mark.parametrize("sensor", [False, True])
def test_destroyed_collider_end_is_safe(world, sensor):
    fixed, moving = pair(world, sensor)
    read = world.get_sensor_events if sensor else world.get_contact_events
    world.step(1 / 60)
    saved = moving.get_colliders()[0]
    lookup = {saved: "player"}
    saved_id = saved.id
    moving.destroy()
    assert not saved.is_valid
    assert saved.id == saved_id
    with pytest.raises(RuntimeError):
        _ = saved.body
    world.step(1 / 60)
    ended = [e for e in read() if not e.began]
    assert len(ended) == 1
    a, b = (ended[0].sensor, ended[0].visitor) if sensor else (ended[0].collider_a, ended[0].collider_b)
    destroyed = a if a.id == saved_id else b
    assert destroyed == saved and lookup[destroyed] == "player"
    assert not destroyed.is_valid


@pytest.mark.parametrize("sensor", [False, True])
def test_event_opt_in_and_world_isolation(world, sensor):
    pair(world, sensor, events=False)
    other = kn.physics.World()
    pair(other, sensor, events=True)
    world.step(1 / 60)
    assert world.get_contact_events() == []
    assert world.get_sensor_events() == []
    assert other.get_contact_events() == []
    assert other.get_sensor_events() == []
    other.step(1 / 60)
    assert len(other.get_sensor_events() if sensor else other.get_contact_events()) == 1


def test_multiple_colliders_have_distinct_identity(world):
    body = kn.physics.RigidBody(world)
    body.add_collider(kn.Circle(kn.Vec2(-1, 0), 0.5))
    body.add_collider(kn.Circle(kn.Vec2(1, 0), 0.5))
    a, b = body.get_colliders()
    assert a.id != b.id
    assert a.body == b.body == body
    assert body.get_colliders() == [a, b]


def test_character_sensor_events_survive_capsule_replacement(world):
    zone = kn.physics.StaticBody(world)
    zone.add_collider(kn.Circle(kn.Vec2(0, 0), 2), is_sensor=True, enable_events=True)
    player = kn.physics.CharacterBody(world)
    original = player.get_colliders()[0]
    original.set_events_enabled(True)
    player.capsule_shape = player.capsule_shape
    assert not original.is_valid
    with pytest.raises(RuntimeError):
        original.set_events_enabled(False)
    world.step(1 / 60)
    events = world.get_sensor_events()
    assert len(events) == 1 and events[0].began
    assert events[0].visitor.body == player
    assert events[0].visitor == player.get_colliders()[0]
    player.pos = kn.Vec2(10, 0)
    world.step(1 / 60)
    assert len(world.get_sensor_events()) == 1
    assert not world.get_sensor_events()[0].began
