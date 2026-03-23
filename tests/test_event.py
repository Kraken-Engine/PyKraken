import time
import pykraken
from pykraken import event


def test_push_and_poll_custom_event():
    pykraken.init(debug=False)
    try:
        evt = event.new_custom()
        event.push(evt)
        evts = event.poll()
        assert any((isinstance(e.type, int) and e.type == evt.type) for e in evts)
    finally:
        pykraken.quit()

def test_schedule_custom_event_once():
    pykraken.init()
    try:
        evt = event.new_custom()
        event.schedule(evt, 50, repeat=False)
        found = False
        deadline = time.time() + 1.0
        while time.time() < deadline:
            evts = event.poll()
            if any((isinstance(e.type, int) and e.type == evt.type) for e in evts):
                found = True
                break
            time.sleep(0.02)
        assert found
    finally:
        try:
            event.unschedule(evt)
        except Exception:
            pass
        pykraken.quit()