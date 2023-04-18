#!/usr/bin/env python3
"""
Dynamic analyser of a cart controller.
Project 2 (ATA)
Author: Michal Šedý <xsedym02@vutbr.cz>
"""


class Monitor:
    """Dynamic analyser

    Attributes:
        CART_STATIONS (set): A set of station names.
        CART_MAX_SLOTS (int): A number of cart slots.
        CART_MAX_WEIGHT (int): A maximum cart weight (load).
    """
    CART_STATIONS = {"A", "B", "C", "D"}
    CART_MAX_SLOTS = 4
    CART_MAX_WEIGHT = 150

    class No2LoadsOrUnloadsAtm:
        """Class of the automaton for the monitoring of properties 1 and 2.
        Property 1: The loading must not be applied on an occupied slot.
        Property 2: The unloading must not be applied on an empty slot.
        """
        def __init__(self, slot_id: int) -> None:
            self.slot_id = slot_id
            self.state = "empty"
            self.trans = {"empty": {"loading": "full",
                                    "unloading": "underflow"},
                          "full": {"unloading": "empty",
                                   "loading": "overflow"}}

        def step(self, event_time: str, event_type: str) -> None:
            """Performs automaton step."""
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                # No suitable transition.
                print(f"{event_time}:error: other; {event_type} on slot #{self.slot_id} in the state {self.state} is not possible")
                return

            if self.state == "underflow":
                # Property 2 violation.
                print(f"{event_time}:error: property 2 violation; unloading from an empty slot #{self.slot_id}")
                self.state = "empty"
            elif self.state == "overflow":
                # Property 1 violation.
                print(f"{event_time}:error: property 1 violation; loading into an occupied slot #{self.slot_id}")
                self.state = "full"

    class MustLoadAndUnloadAtm:
        """Class of the automaton for the monitoring of properties 4 and 8.
        Property 4: Every request must cause loading at some time.
        Property 8: Every loading must cause unloading at some time.
        """
        def __init__(self, item_name: str) -> None:
            self.cargo_name = item_name
            self.state = "created"
            self.trans = {"created": {"loading": "loaded",
                                      "stop": "not_loaded"},
                          "loaded": {"unloading": "unloaded",
                                     "stop": "not_unloaded"}}

        def step(self, event_time: str, event_type: str) -> None:
            """Performs automaton step."""
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                # No suitable transition.
                print(f"{event_time}:error: other; {event_type} a cargo {self.cargo_name} in the state {self.state} is not possible")
                return

            if self.state == "not_unloaded":
                # Property 8 violation.
                print(f"{event_time}:error: property 8 violation; a cargo {self.cargo_name} was not unloaded before the cart finished")
            elif self.state == "not_loaded":
                # Property 4 violation.
                print(f"{event_time}:error: property 4 violation; a cargo {self.cargo_name} was not loaded before the cart finished")

    class CartLimitAtm:
        """Class of the automaton for the monitoring of properties 6 and 7.
        Property 6: A cart must not contain more than 4 cargo.
        Property 7: A cart must not be overloaded.
        """
        def __init__(self) -> None:
            self.cargo_cnt = 0
            self.cargo_weight = 0

        def step(self, event_time: str, event_type: str, weight: str) -> None:
            """Performs automaton step."""
            if event_type == "loading":
                self.cargo_weight += int(weight)
                self.cargo_cnt += 1

                if self.cargo_cnt > Monitor.CART_MAX_SLOTS:
                    # Property 6 violation.
                    print(f"{event_time}:error: property 6 violation; loaded more than 4 cargo on the cart; contains {self.cargo_cnt} cargo")
                if self.cargo_weight > Monitor.CART_MAX_WEIGHT:
                    # Property 7 violation.
                    print(f"{event_time}:error: property 7 violation; the cart is overloaded; current weight is {self.cargo_weight}")

            elif event_type == "unloading":
                if self.cargo_cnt != 0:
                    self.cargo_weight -= int(weight)
                    self.cargo_cnt -= 1

    class DirectUnloadAtm:
        """Class of the automaton for the monitoring of the property 3.
        Property 3: A cargo must be unloaded, when the cart is at the destination.
        """
        def __init__(self, cargo_name: str, dst: str) -> None:
            self.cargo_dst = dst
            self.cargo_name = cargo_name
            self.state = "created"
            self.trans = {"created": {("moving", pos): "created" for pos in Monitor.CART_STATIONS}}
            self.trans["created"]["loading"] = "loaded"
            self.trans["loaded"] = dict()
            self.trans["loaded"][("moving", dst)] = "late"
            self.trans["loaded"][("unloading", dst)] = "unloaded"
            for pos in Monitor.CART_STATIONS.difference(dst):
                self.trans["loaded"][("unloading", pos)] = "bad_unloading"
                self.trans["loaded"][("moving", pos)] = "loaded"

        def step(self, event_time: str, event_type: str) -> None:
            """Performs automaton step."""
            try:
                self.state = self.trans[self.state][event_type]
            except KeyError:
                # No suitable transition.
                print(f"{event_time}:error: other; {event_type} a cargo {self.cargo_name} in the state {self.state} is not possible")
                return

            if self.state == "late":
                # Property 3 violation - not unloaded at a destination.
                print(f"{event_time}:error: property 3 violation; {self.cargo_name} was not unloaded at the destination station")
            if self.state == "bad_unloading":
                # Property 3 violation - unloaded out of a destination.
                print(f"{event_time}:error: property 3 violation; {self.cargo_name} was not supposed to be unloaded on {event_type[1]}")

    class RequestBeforeLoadAtm:
        """Class of the automaton for the monitoring of the property 5.
        Property 5: A cart must not load in the station if there is no request.
        """
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
                # No suitable transition.
                print(f"{event_time}:error: other; {event_type} a cargo {self.cargo_name} in the state {self.state} is not possible")
                return

            if self.state == "bad_loading":
                # Property 5 violation.
                print(f"{event_time}:error: property 5 violation; no prior request for the loading of {self.cargo_name} at {event_type[1]}")

    class NoIdleWhenRequestAtm:
        """Class of the automaton for the monitoring of the property 9.
        Property 9: The cart mast not be in the mode idle when some request exists.
        """
        def __init__(self) -> None:
            self.request_cnt = 0
            self.idle_time = None

        def step(self, event_time: str, event_type: str) -> int:
            """Performs an automaton step."""
            if self.idle_time is not None:
                if self.idle_time == int(event_time) and event_type != "idle":
                    self.idle_time = None
                else:
                    # Property 9 violation.
                    print(f"{event_time}:error: property 9 violation; the cart is in the idle mode with pending transportation request(s)")
                    self.idle_time = None
            if event_type == "requesting":
                self.request_cnt += 1
            elif event_type == "unloading":
                self.request_cnt -= 1
            elif event_type == "idle":
                self.idle_time = int(event_time)


    def __init__(self) -> None:
        self.last_event_time = 0

        # The set containing unique pairs of loading source station and slot of a cart.
        self.uniq_loads = set()

        # List of slots automata controlling loading and unloading.
        # Loading must not be applied on a full slot. (property 1)
        # Unloading must not be applied on an empty slot. (property 2)
        self.no_2_loads_or_unloads_list = [Monitor.No2LoadsOrUnloadsAtm(i) for i in range(4)]

        # A dictionary of cargo automata controlling unloading in the destination.
        # Indexed by cargo name.
        # A cargo must be unloaded, when the cart is in the destination. (property 3)
        self.direct_unload_dict: dict[str, Monitor.DirectUnloadAtm] = dict()

        # A dictionary of cargo automata controlling loading in the source station.
        # Indexed by cargo name.
        # A cart must not load in the station if there is no request. (property 5)
        self.request_before_load_dict: dict[str, Monitor.RequestBeforeLoadAtm] = dict()

        # Automaton controlling maximum cart load and weight.
        # A cargo can contain maximum of 4 cargos. (property 6)
        # The sum of weights of loaded cargo must be less or equal to 150kg. (property 7)
        self.cart_limit_atm = Monitor.CartLimitAtm()

        # A dictionary of automata for each cargo, controlling cargo loading and unloading.
        # Indexed by cargo name.
        # Every request must cause loading at some time. (property 4)
        # Every loading must cause unloading at some time. (property 8)
        self.must_load_and_unload_dict: dict[str, Monitor.MustLoadAndUnloadAtm] = dict()

        # The cart mast not be in the mode idle when some request exists. (property 9)
        self.no_idle_when_request_atm = Monitor.NoIdleWhenRequestAtm()

    def onevent(self, event) -> None:
        """Event handler."""
        event_time = event[0]
        self.last_event_time = event_time
        event_id = event[1]
        del event[1]

        # Testing property 9.
        self.no_idle_when_request_atm.step(event_time, event_id)

        # Handling event.
        if event_id == "moving":
            self.onmoving(*event)
        elif event_id == "requesting":
            self.onrequesting(*event)
        elif event_id == "loading":
            self.onloading(*event)
        elif event_id == "unloading":
            self.onunloading(*event)

    def onmoving(self, time: str, src: str, dst: str) -> None:
        """moving handler"""
        for atm in self.direct_unload_dict.values():
            atm.step(time, ("moving", src))

    def onrequesting(self, time: str, src: str, dst: str, cargo: str, weight: str) -> None:
        """requesting handler
        New automata ned to be instantiated.
        """
        if cargo in self.request_before_load_dict or cargo in self.must_load_and_unload_dict or cargo in self.direct_unload_dict:
            # This cargo already exists.
            print(f"{time}:error: other; the request for {cargo} has been already made")

        self.request_before_load_dict[cargo] = Monitor.RequestBeforeLoadAtm(cargo, src)
        self.must_load_and_unload_dict[cargo] = Monitor.MustLoadAndUnloadAtm(cargo)
        self.direct_unload_dict[cargo] = Monitor.DirectUnloadAtm(cargo, dst)


    def onloading(self, time: str, pos: str, cargo: str, weight: str, slot: str) -> None:
        """loading handler"""

        # Remembering the load for the coverage computation.
        self.uniq_loads.add((pos, slot))

        # Testing properties 1 and 2.
        self.no_2_loads_or_unloads_list[int(slot)].step(time, "loading")

        # Testing properties 6 and 7.
        self.cart_limit_atm.step(time, "loading", weight)

        if cargo not in self.request_before_load_dict or cargo not in self.must_load_and_unload_dict or cargo not in self.direct_unload_dict:
            # The loaded cargo does not exist.
            print(f"{time}:error: other; A cargo {cargo} can not be loaded. It does not exist.")

        # Testing property 3.
        if cargo in self.direct_unload_dict:
            self.direct_unload_dict[cargo].step(time, "loading")

        # Testing property 5.
        if cargo in self.request_before_load_dict:
            self.request_before_load_dict[cargo].step(time, ("loading", pos))
            # Removing automaton.
            del self.request_before_load_dict[cargo]

        # Testing properties 4 and 8.
        if cargo in self.must_load_and_unload_dict:
            self.must_load_and_unload_dict[cargo].step(time, "loading")

    def onunloading(self, time: str, pos: str, cargo: str, weight: str, slot: str) -> None:
        """unloading handler"""

        # Testing properties 1 and 2.
        self.no_2_loads_or_unloads_list[int(slot)].step(time, "unloading")
        self.cart_limit_atm.step(time, "unloading", weight)

        if cargo not in self.must_load_and_unload_dict or cargo not in self.direct_unload_dict:
            # The unloaded cargo des not exist.
            print(f"{time}:error: other; A cargo {cargo} can not be unloaded. It does not exist.")

        # Testing property 3.
        if cargo in self.direct_unload_dict:
            self.direct_unload_dict[cargo].step(time, ("unloading", pos))
            # Removing automaton.
            del self.direct_unload_dict[cargo]

        # Testing properties 4 and 8.
        if cargo in self.must_load_and_unload_dict:
            self.must_load_and_unload_dict[cargo].step(time, "unloading")
            # Removing automaton.
            del self.must_load_and_unload_dict[cargo]

    def finish(self) -> None:
        """finish handler"""

        # Testing properties 4 and 8.
        for atm in self.must_load_and_unload_dict.values():
            atm.step(self.last_event_time, "stop")

    def get_coverage(self) -> float:
        """Calculates the cart coverage."""
        return len(self.uniq_loads)/(len(Monitor.CART_STATIONS)*Monitor.CART_MAX_SLOTS)


    def report_coverage(self):
        """Coverage reporter"""
        print("CartCoverage %d%%" % ((self.get_coverage())*100))


def monitor(reader):
    """Main function"""
    mtr = Monitor()
    for line in reader:
        line = line.strip()
        mtr.onevent(line.split())
    mtr.finish()
    mtr.report_coverage()

if __name__ == "__main__":
    import sys
    monitor(sys.stdin)
