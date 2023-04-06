#!/usr/bin/env python3
"""
Example of usage/test of Cart controller implementation.
"""

import sys
from cartctl import CartCtl
from cart import Cart, CargoReq
from jarvisenv import Jarvis
import unittest

def log(msg):
    "simple logging"
    print('  %s' % msg)

class TestCartRequests(unittest.TestCase):

    def test_happy(self):
        "Happy-path test"

        def add_load(c: CartCtl, cargo_req: CargoReq):
            "callback for schedulled load"
            log('%d: Requesting %s at %s' % \
                (Jarvis.time(), cargo_req, cargo_req.src))
            c.request(cargo_req)

        def on_move(c: Cart):
            "example callback (for assert)"
            # put some asserts here
            log('%d: Cart is moving %s->%s' % (Jarvis.time(), c.pos, c.data))

        def on_load(c: Cart, cargo_req: CargoReq):
            "example callback for logging"
            log('%d: Cart at %s: loading: %s' % (Jarvis.time(), c.pos, cargo_req))
            log(c)
            cargo_req.context = "loaded"

        def on_unload(c: Cart, cargo_req: CargoReq):
            "example callback (for assert)"
            # put some asserts here
            log('%d: Cart at %s: unloading: %s' % (Jarvis.time(), c.pos, cargo_req))
            log(c)
            self.assertEqual('loaded', cargo_req.context)
            cargo_req.context = 'unloaded'
            if cargo_req.content == 'helmet':
                self.assertEqual('B', c.pos)
            if cargo_req.content == 'heart':
                self.assertEqual('A', c.pos)
            #if cargo_req.content.startswith('bracelet'):
            #    self.assertEqual('C', c.pos)
            if cargo_req.content == 'braceletR':
                self.assertEqual('A', c.pos)
            if cargo_req.content == 'braceletL':
                self.assertEqual('C', c.pos)

        # Setup Cart
        # 4 slots, 150 kg max payload capacity, 2=max debug
        cart_dev = Cart(4, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        c = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet = CargoReq('A', 'B', 20, 'helmet')
        helmet.onload = on_load
        helmet.onunload = on_unload

        heart = CargoReq('C', 'A', 40, 'heart')
        heart.onload = on_load
        heart.onunload = on_unload

        braceletR = CargoReq('D', 'A', 40, 'braceletR')
        braceletR.onload = on_load
        braceletR.onunload = on_unload

        braceletL = CargoReq('D', 'C', 40, 'braceletL')
        braceletL.onload = on_load
        braceletL.onunload = on_unload

        # Setup Plan
        Jarvis.reset_scheduler()
        #         when  event     called_with_params
        Jarvis.plan(10, add_load, (c,helmet))
        Jarvis.plan(45, add_load, (c,heart))
        Jarvis.plan(40, add_load, (c,braceletR))
        Jarvis.plan(25, add_load, (c,braceletL))
        
        # Exercise + Verify indirect output
        #   SUT is the Cart.
        #   Exercise means calling Cart.request in different time periods.
        #   Requests are called by add_load (via plan and its scheduler).
        #   Here, we run the plan.
        Jarvis.run()

        # Verify direct output
        log(cart_dev)
        self.assertTrue(cart_dev.empty())
        self.assertEqual('unloaded', helmet.context)
        self.assertEqual('unloaded', heart.context)
        self.assertEqual('unloaded', braceletR.context)
        self.assertEqual('unloaded', braceletL.context)
        #self.assertEqual(cart_dev.pos, 'C')

if __name__ == "__main__":
    unittest.main()
