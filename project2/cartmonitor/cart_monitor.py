#!/usr/bin/env python3
"""
Dynamic analyser of a cart controller.
"""


class Monitor:
    class No2LoadsOrUnloadsAtm:
        def __init__(self, slot_id: int) -> None:
            self.slot_id = slot_id
            self.state = "empty"
            self.trans = {"empty": {"loading": "full"},
                          "empty": {"unloading": "underflow"},
                          "full": {"unload": "empty",},
                          "full": {"loading": "overflow"}}

        def step(self, event_time: str, event_type: str) -> None:
            self.state = self.trans[self.state][event_type]

            if self.state == "underflow":
                print(f"{event_time}:error: unloading from an empty slot #{self.slot_id}")
                self.state = "empty"
            elif self.state == "overflow":
                print(f"{event_time}:error: loading into an occupied slot #{self.slot_id}")
                self.state = "full"

    class MustLoadAndUnloadAtm:
        def __init__(self, item_name: str) -> None:
            self.cargo_name = item_name
            self.state = "created"
            self.trans = {"created": {"loading": "loaded"},
                          "loaded": {"unloading": "unloaded"},
                          "loaded": {"stop": "not_unloaded"},
                          "created": {"stop": "not_loaded"}}

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:mester_error: an operation {event_type} on a state {self.state} is not supported #{self.cargo_name}")

            if self.state == "not_unloaded":
                print(f"{event_time}:error: a cargo {self.cargo_name} was not unloaded before the cart stop")
            elif self.state == "not_loaded":
                print(f"{event_time}:error: a cargo {self.cargo_name} was not loaded before the cart stop")

        def is_true(self) -> bool:
            return self.state == "unloaded"

    class CargoLimitAtm:
        def __init__(self) -> None:
            self._MAX_CARGO = 4
            self._MAX_WEIGHT = 150
            self.cargo_cnt = 0
            self.cargo_weight = 0

        def step(self, event_time: str, event_type: str, weight: str) -> None:
            if event_type == "loading":
                self.cargo_weight += int(weight)
                self.cargo_cnt += 1

                if self.cargo_cnt > self._MAX_CARGO:
                    print(f"{event_time}:error: loaded more than 4 cargo on the cart")
                if self.cargo_weight > self._MAX_WEIGHT:
                    print(f"{event_time}:error: the cart is overloaded; current weight is {self.cargo_weight}")

            elif event_type == "unloading":
                if self.cargo_cnt != 0:
                    self.cargo_weight -= int(weight)
                    self.cargo_cnt -= 1

    class DirectUnloadAtm:
        def __init__(self, cargo_name: str, dst: str) -> None:
            self.cargo_dst = dst
            self.cargo_name = cargo_name
            self.state = "created"
            self.trans = {"created": {"loading": "loaded"}}
            self.trans["loaded"] = {("moving", pos): "loaded" for pos in {"A", "B", "C", "D"}.difference(dst)}
            self.trans["loaded"][("moving", dst)] = "late"
            self.trans["loaded"]["unloading"] ="unloaded"

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:mester_error: an operation {event_type} on a state {self.state} is not supported #{self.cargo_name}")

            if self.state == "late":
                print(f"{event_time}:error: {self.cargo_name} was not unloaded on the destination station")

        def is_true(self) -> bool:
            return self.state == "unloaded"

    class RequestBeforeLoadAtm:
        def __init__(self, cargo_name: str, src: str) -> None:
            self.cargo_src = src
            self.cargo_name = cargo_name
            self.state = "created"
            self.trans = {("loading", pos): "bad_loading" for pos in {"A", "B", "C", "D"}}
            self.trans[("loading", src)] = "loaded"

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:mester_error: an operation {event_type} on a state {self.state} is not supported #{self.cargo_name}")

            if self.state == "bad_loading":
                print(f"{event_time}:error: no priorir request for the loading of {self.cargo_name} at {self.cargo_src}")

        def is_true(self) -> bool:
            return self.state == "loaded"

    class NoIdleWhenRequestCtl:
        def __init__(self) -> None:
            self.request_cnt = 0
            self.idle_time = None

        def step(self, event_time: str, event_type: str) -> None:
            if self.idle_time is not None:
                if self.idle_time == int(event_time) and event_type != "idle":
                    self.idle_time = None
                else:
                    print(f"{event_time}:error: the cart is in the idle mode even with pending transportation request(s)")

            if event_type == "requesting":
                self.request_cnt += 1
            elif event_type == "unloading":
                self.request_cnt -= 1
            elif event_type == "idle":
                self.idle_time = int(event_time)


    def __init__(self) -> None:
        self._no2loads


def report_coverage():
    """Coverage reporter"""
    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    # Zde nahradte vypocet/vypis aktualne dosazeneho pokryti
    print('CartCoverage %d%%' % ((21/50)*100))

def onmoving(time, pos1, pos2):
    """priklad event-handleru pro udalost moving"""
    # Podobnou funkci muzete i nemusite vyuzit, viz onevent().
    # Vsechny parametry jsou typu str; nektere muze byt nutne pretypovat.
    time = int(time)
    print('%d:debug: got moving from %s to %s' % (time, pos1, pos2))

def onevent(event):
    """Event handler. event = [TIME, EVENT_ID, ...]"""
    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    # ZDE IMPLEMENTUJTE MONITORY
    print(event)

    # vyjmeme identifikaci udalosti z dane n-tice
    event_id = event[1]
    del(event[1])
    # priklad predani ke zpracovani udalosti moving
    if event_id == 'moving':
        # predame n-tici jako jednotlive parametry pri zachovani poradi
        onmoving(*event)
    #elif event_id == '....':
    #    ...

###########################################################
# Nize netreba menit.

def monitor(reader):
    """Main function"""
    for line in reader:
        line = line.strip()
        onevent(line.split())
    report_coverage()

if __name__ == "__main__":
    import sys
    monitor(sys.stdin)
