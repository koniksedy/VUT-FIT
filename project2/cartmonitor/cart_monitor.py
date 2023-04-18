#!/usr/bin/env python3
"""
Dynamic analyser of a cart controller.
"""

class Singleton(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]

class Monitor(metaclass=Singleton):
    __metaclass__ = Singleton

    CART_STATIONS = {"A", "B", "C", "D"}
    CART_MAX_SLOTS = 4
    CART_MAX_WEIGHT = 150

    class No2LoadsOrUnloadsAtm:
        def __init__(self, slot_id: int) -> None:
            self.slot_id = slot_id
            self.state = "empty"
            self.trans = {"empty": {"loading": "full",
                                    "unloading": "underflow"},
                          "full": {"unloading": "empty",
                                   "loading": "overflow"}}

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:error: {event_type} on slot #{self.slot_id} in the state {self.state} is not possible")
                return

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
            self.trans = {"created": {"loading": "loaded",
                                      "stop": "not_loaded"},
                          "loaded": {"unloading": "unloaded",
                                     "stop": "not_unloaded"}}

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:error: {event_type} on cargo {self.cargo_name} in the state {self.state} is not possible")
                return

            if self.state == "not_unloaded":
                print(f"{event_time}:error: a cargo {self.cargo_name} was not unloaded before the cart stop")
            elif self.state == "not_loaded":
                print(f"{event_time}:error: a cargo {self.cargo_name} was not loaded before the cart stop")

    class CartLimitAtm:
        def __init__(self) -> None:
            self.cargo_cnt = 0
            self.cargo_weight = 0

        def step(self, event_time: str, event_type: str, weight: str) -> None:
            if event_type == "loading":
                self.cargo_weight += int(weight)
                self.cargo_cnt += 1

                if self.cargo_cnt > Monitor.CART_MAX_SLOTS:
                    print(f"{event_time}:error: loaded more than 4 cargo on the cart")
                if self.cargo_weight > Monitor.CART_MAX_WEIGHT:
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
            self.trans = {"created": {("moving", pos): "created" for pos in Monitor.CART_STATIONS}}
            self.trans["created"]["loading"] = "loaded"
            self.trans["loaded"][("moving", dst)] = "late"
            self.trans["loaded"][("unloading", dst)] = "unloaded"
            for pos in Monitor.CART_STATIONS.difference(dst):
                self.trans["loaded"][("unloading", pos)] = "bad_unloading"
                self.trans["loaded"][("moving", pos)] = "loaded"

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:DUerror: {event_type} on cargo {self.cargo_name} in the state {self.state} is not possible")
                return

            if self.state == "late":
                print(f"{event_time}:error: {self.cargo_name} was not unloaded on the destination station")
            if self.state == "bad_unloading":
                print(f"{event_time}:error: {self.cargo_name} was not supposed to be unloade on {event_type[1]}")

    class RequestBeforeLoadAtm:
        def __init__(self, cargo_name: str, src: str) -> None:
            self.cargo_src = src
            self.cargo_name = cargo_name
            self.state = "created"
            self.trans = dict()
            self.trans["created"] = {("loading", pos): "bad_loading" for pos in Monitor.CART_STATIONS}
            self.trans["created"][("loading", src)] = "loaded"

        def step(self, event_time: str, event_type: str) -> None:
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                print(f"{event_time}:RBerror: {event_type} on cargo {self.cargo_name} in the state {self.state} is not possible")
                return

            if self.state == "bad_loading":
                print(f"{event_time}:error: no priorir request for the loading of {self.cargo_name} at {self.cargo_src}")

    class NoIdleWhenRequestAtm:
        def __init__(self) -> None:
            self.request_cnt = 0
            self.idle_time = None

        def step(self, event_time: str, event_type: str) -> int:
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
        # The set containing unique pairs of loading source station and slot of a cart.
        self.uniq_loads = set()

        # List of slots automata controling loading and unloading.
        # Loading must not be appliad on a full slot. (property 1)
        # Unloading must not be applied on an empty slot. (property 2)
        self.no_2_loads_or_unloads_list = [Monitor.No2LoadsOrUnloadsAtm(i) for i in range(4)]

        # A dictionary of cargo automata controling unloading in the destination.
        # Indexed by cargo name.
        # A cargo must be unloaded, when the cart is in the destination. (property 3)
        self.direct_unload_dict: dict[str, Monitor.DirectUnloadAtm] = dict()

        # A dictionary of cargo automata controling loading in the source station.
        # Indexed by cargo name.
        # A cart must not load in the station if there is no request. (property 5)
        self.request_before_load_dict: dict[str, Monitor.RequestBeforeLoadAtm] = dict()

        # Automaton controling maximum cart load and weight.
        # A cargo can contain maximum of 4 cargos. (property 6)
        # The sum of weights of loaded cargo must be less or equat 150kg. (property 7)
        self.cart_limit_atm = Monitor.CartLimitAtm()

        # A dictionary of automata for each cargo, controlling cargo loading and unloading.
        # Indexed by cargo name.
        # The cargo must be loaded in some time if the transport reques has been made. (property 4)
        # The loaded cargu must be unloaded in some time. (property 8)
        self.must_load_and_unload_dict: dict[str, Monitor.MustLoadAndUnloadAtm] = dict()

        # The cart mast not be in the mode idle when some reqest exists. (property 9)
        self.no_idle_when_request_atm = Monitor.NoIdleWhenRequestAtm()

    def onevent(self, event) -> None:
        event_time = event[0]
        event_id = event[1]
        del event[1]

        self.no_idle_when_request_atm.step(event_time, event_id)
        if event_id == "moving":
            self.onmoving(*event)
        elif event_id == "requesting":
            self.onrequesting(*event)
        elif event_id == "loading":
            self.onloading(*event)
        elif event_id == "unloading":
            self.onunloading(*event)
        elif event_id == "stop":
            self.onstop(*event)

    def onmoving(self, time: str, src: str, dst: str) -> None:
        for atm in self.direct_unload_dict.values():
            try:
                atm.step(time, ("moving", src))
            except KeyError:
                print(f"{time}:error: A cargo {atm.cargo_name} can not be moved. It is in the state {atm.state}.")

    def onrequesting(self, time: str, src: str, dst: str, cargo: str, weight: str) -> None:
        if cargo in self.request_before_load_dict or cargo in self.must_load_and_unload_dict or cargo in self.direct_unload_dict:
            print(f"{time}:error: the request for {cargo} has been allready made")
        self.request_before_load_dict[cargo] = Monitor.RequestBeforeLoadAtm(cargo, src)
        self.must_load_and_unload_dict[cargo] = Monitor.MustLoadAndUnloadAtm(cargo)
        self.direct_unload_dict[cargo] = Monitor.DirectUnloadAtm(cargo, dst)


    def onloading(self, time: str, pos: str, cargo: str, weight: str, slot: str) -> None:
        self.uniq_loads.add((pos, slot))

        self.no_2_loads_or_unloads_list[int(slot)].step(time, "loading")
        self.cart_limit_atm.step(time, "loading", weight)

        if cargo not in self.request_before_load_dict or cargo not in self.must_load_and_unload_dict or cargo not in self.direct_unload_dict:
            print(f"{time}:error: The cargo {cargo} can not be loaded. It does not exist.")

        if cargo in self.direct_unload_dict:
            self.direct_unload_dict[cargo].step(time, "loading")
        if cargo in self.request_before_load_dict:
            self.request_before_load_dict[cargo].step(time, ("loading", pos))
            del self.request_before_load_dict[cargo]
        if cargo in self.must_load_and_unload_dict:
            self.must_load_and_unload_dict[cargo].step(time, "loading")

    def onunloading(self, time: str, pos: str, cargo: str, weight: str, slot: str) -> None:
        self.no_2_loads_or_unloads_list[int(slot)].step(time, "unloading")
        self.cart_limit_atm.step(time, "unloading", weight)

        if cargo not in self.must_load_and_unload_dict or cargo not in self.direct_unload_dict:
            print(f"{time}:error: The cargo {cargo} can not be unloaded. It does not exist.")

        if cargo in self.direct_unload_dict:
            self.direct_unload_dict[cargo].step(time, "unloading")
            del self.direct_unload_dict[cargo]
        if cargo in self.must_load_and_unload_dict:
            self.must_load_and_unload_dict[cargo].step(time, "unloading")
            del self.must_load_and_unload_dict[cargo]

    def onstop(self, time: str) -> None:
        for atm in self.must_load_and_unload_dict.values():
            atm.step(time, "stop")

    def get_coverage(self) -> float:
        return len(self.uniq_loads)/(len(Monitor.CART_STATIONS)*Monitor.CART_MAX_SLOTS)


def report_coverage():
    """Coverage reporter"""
    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    # Zde nahradte vypocet/vypis aktualne dosazeneho pokryti
    print('CartCoverage %d%%' % ((Monitor().get_coverage())*100))

def onmoving(time, pos1, pos2):
    """priklad event-handleru pro udalost moving"""
    # Podobnou funkci muzete i nemusite vyuzit, viz onevent().
    # Vsechny parametry jsou typu str; nektere muze byt nutne pretypovat.
    time = int(time)
    print('%d:debug: got moving from %s to %s' % (time, pos1, pos2))

def onevent(event):
    """Event handler. event = [TIME, EVENT_ID, ...]"""
    print(event)
    Monitor().onevent(event)

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
    # with open("bad_log.txt", "r") as fd:
    #     monitor(fd)
