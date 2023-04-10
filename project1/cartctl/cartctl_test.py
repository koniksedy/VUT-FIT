#!/usr/bin/env python3
"""
Implementation of automatic tests of the self-driving cart in the factory.
Project: ATA - 1
Author: Michal Šedý <xsedym02@vutbr.cz>
"""


import sys
import cartctl
import cart
from cartctl import CartCtl
from cart import Cart, CargoReq
from jarvisenv import Jarvis
import unittest


def log(msg):
    """simple logging"""
    print('  %s' % msg)

class TestCartRequests(unittest.TestCase):

    def _add_load(self, c: CartCtl, cargo_req: CargoReq):
            """Scheduled load callback"""
            log("%d: Requesting %s at %s" % (Jarvis.time(), cargo_req, cargo_req.src))
            c.request(cargo_req)

    def _on_move_ceg(self, cart_ctl: CartCtl, cart_dev: Cart, prio: bool) -> None:
        """Cargo move callback"""
        log("%d: Cart is moving %s->%s" % (Jarvis.time(), cart_dev.pos, cart_dev.data))
        self.assertEqual(cart.Status.Moving, cart_dev.status)
        if prio:
            self.assertEqual(cartctl.Status.UnloadOnly, cart_ctl.status)
        else:
            self.assertEqual(cartctl.Status.Normal, cart_ctl.status)

    def _on_load_ceg(self, c: Cart, cargo_req: CargoReq, content: str, pos: str, prio: bool) -> None:
        """Cargo load callback"""
        log("%d: Cart at %s: loading: %s" % (Jarvis.time(), c.pos, cargo_req))
        log(c)
        if prio:
            self.assertGreaterEqual(Jarvis.time() - cargo_req.born, 60)
            self.assertLess(Jarvis.time() - cargo_req.born, 60 * 2)
        else:
            self.assertLess(Jarvis.time() - cargo_req.born, 60)
        self.assertEqual(cargo_req.prio, prio)
        self.assertEqual(cargo_req.content, content)
        self.assertIn(cargo_req, c.slots)
        self.assertEqual(c.pos, pos)
        cargo_req.context = "loaded"

    def _on_unload_ceg(self, c: Cart, cargo_req: CargoReq, content: str, pos: str, prio: bool) -> None:
        """Cargo unload callback"""
        log("%d: Cart at %s: unloading: %s" % (Jarvis.time(), c.pos, cargo_req))
        log(c)
        self.assertEqual(cargo_req.prio, prio)
        self.assertEqual(cargo_req.context, "loaded")
        self.assertEqual(cargo_req.content, content)
        self.assertEqual(c.pos, pos)
        cargo_req.context = "unloaded"
        self.assertNotIn(cargo_req, c.slots)

    def _assert_final_cart_ceg(self, cart_ctl: CartCtl , cart_dev: Cart, content_list: list, pos=None) -> None:
        """Verify direct output"""
        log(cart_dev)
        for cargo in content_list:
            self.assertEqual(cargo.context, "unloaded")
        self.assertTrue(cart_dev.empty())
        self.assertEqual(cart.Status.Idle, cart_dev.status)
        self.assertEqual(cartctl.Status.Idle, cart_ctl.status)
        if pos is not None:
            self.assertEqual(cart_dev.pos, pos)

    def test_happy(self):
        """Happy-path test"""

        def add_load(c: CartCtl, cargo_req: CargoReq):
            """callback for scheduled load"""
            log("%d: Requesting %s at %s" % (Jarvis.time(), cargo_req, cargo_req.src))
            c.request(cargo_req)

        def on_move(c: Cart):
            """example callback (for assert)"""
            # put some asserts here
            log("%d: Cart is moving %s->%s" % (Jarvis.time(), c.pos, c.data))

        def on_load(c: Cart, cargo_req: CargoReq):
            """example callback for logging"""
            log("%d: Cart at %s: loading: %s" % (Jarvis.time(), c.pos, cargo_req))
            log(c)
            cargo_req.context = "loaded"

        def on_unload(c: Cart, cargo_req: CargoReq):
            """example callback (for assert)"""
            # put some asserts here
            log("%d: Cart at %s: unloading: %s" % (Jarvis.time(), c.pos, cargo_req))
            log(c)
            self.assertEqual("loaded", cargo_req.context)
            cargo_req.context = "unloaded"
            if cargo_req.content == "helmet":
                self.assertEqual("B", c.pos)
            if cargo_req.content == "heart":
                self.assertEqual("A", c.pos)
            #if cargo_req.content.startswith('bracelet'):
            #    self.assertEqual('C', c.pos)
            if cargo_req.content == "braceletR":
                self.assertEqual("A", c.pos)
            if cargo_req.content == "braceletL":
                self.assertEqual("C", c.pos)

        # Setup Cart
        # 4 slots, 150 kg max payload capacity, 2=max debug
        cart_dev = Cart(4, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        c = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet = CargoReq("A", "B", 20, "helmet")
        helmet.onload = on_load
        helmet.onunload = on_unload

        heart = CargoReq("C", "A", 40, "heart")
        heart.onload = on_load
        heart.onunload = on_unload

        braceletR = CargoReq("D", "A", 40, "braceletR")
        braceletR.onload = on_load
        braceletR.onunload = on_unload

        braceletL = CargoReq("D", "C", 40, "braceletL")
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
        self.assertEqual("unloaded", helmet.context)
        self.assertEqual("unloaded", heart.context)
        self.assertEqual("unloaded", braceletR.context)
        self.assertEqual("unloaded", braceletL.context)
        #self.assertEqual(cart_dev.pos, 'C')

    def test_ceg_no_task(self) -> None:
        """CEG: 1
        No task is created for the cart. The cart should be still.
        """
        def on_move(c: Cart) -> None:
            """Cart move callback"""
            log("%d: Cart is moving %s->%s" % (Jarvis.time(), c.pos, c.data))
            self.fail("Cart should not be moving.")

        # Setup Cart
        cart_dev = Cart(4, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        cart_ctl = CartCtl(cart_dev, Jarvis)

        # Setup Plan
        Jarvis.reset_scheduler()

        # Exercise + Verify indirect output
        Jarvis.run()

        # Verify direct output
        self._assert_final_cart_ceg(cart_ctl, cart_dev, [])

    def test_ceg_pick_up_normal(self) -> None:
        """CEG: 2
        Normal task. Successful delivery.
        """
        def on_move(c: Cart) -> None:
            """Cart move callback"""
            self._on_move_ceg(cart_ctl, c, helmet_prio)

        def on_load(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo load callback"""
            self._on_load_ceg(c, cargo_req, "helmet", helmet_src, helmet_prio)

        def on_unload(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo unload callback"""
            self._on_unload_ceg(c, cargo_req, "helmet", helmet_dst, helmet_prio)

        # Setup Cart
        cart_dev = Cart(2, 500, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        cart_ctl = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet_src = "A"
        helmet_dst = "C"
        helmet_prio = False
        helmet = CargoReq(helmet_src, helmet_dst, 100, "helmet")
        helmet.onload = on_load
        helmet.onunload = on_unload

        # Setup Plan
        Jarvis.reset_scheduler()
        Jarvis.plan(0, self._add_load, (cart_ctl, helmet))

        # Exercise + Verify indirect output
        Jarvis.run()

        # Verify direct output
        self._assert_final_cart_ceg(cart_ctl, cart_dev, [helmet], helmet_dst)

    def test_ceg_pickup_priority(self) -> None:
        """CEG: 3
        Normal task + Priority task. Successful delivery.
        """
        def on_move(c: Cart) -> None:
            """Cart move callback"""
            if braceletR in c.slots:
                self._on_move_ceg(cart_ctl, c, braceletR_prio)
            else:
                self._on_move_ceg(cart_ctl, c, False)

        def on_load(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo load callback"""
            if cargo_req.content == "helmet":
                self._on_load_ceg(c, cargo_req, "helmet", helmet_src, helmet_prio)
            else:
                self._on_load_ceg(c, cargo_req, "braceletR", braceletR_src, braceletR_prio)

        def on_unload(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo unload callback"""
            if cargo_req.content == "helmet":
                self._on_unload_ceg(c, cargo_req, "helmet", helmet_dst, helmet_prio)
            else:
                self._on_unload_ceg(c, cargo_req, "braceletR", braceletR_dst, braceletR_prio)

        # Setup Cart
        cart_dev = Cart(1, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        cart_ctl = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet_src = "A"
        helmet_dst = "C"
        helmet_prio = False
        helmet = CargoReq(helmet_src, helmet_dst, 100, "helmet")
        helmet.onload = on_load
        helmet.onunload = on_unload

        braceletR_src = "A"
        braceletR_dst = "D"
        braceletR_prio = True
        braceletR = CargoReq(braceletR_src, braceletR_dst, 100, "braceletR")
        braceletR.onload = on_load
        braceletR.onunload = on_unload

        # Setup Plan
        Jarvis.reset_scheduler()
        Jarvis.plan(0, self._add_load, (cart_ctl, helmet))
        Jarvis.plan(1, self._add_load, (cart_ctl, braceletR))

        # Exercise + Verify indirect output
        Jarvis.run()

        # Verify direct output
        self._assert_final_cart_ceg(cart_ctl, cart_dev, [helmet, braceletR], braceletR_dst)

    def test_ceg_ne_enough_slots(self) -> None:
        """CEG: 4
        Cargo is not picked due to low number of free slots. The unpicked cargo is
        deleted. This deletion should raise an exception.
        """
        def on_move(c: Cart) -> None:
            """Cart move callback"""
            if braceletR in c.slots:
                self._on_move_ceg(cart_ctl, c, braceletR_prio)
            elif braceletL in c.slots:
                self._on_move_ceg(cart_ctl, c, braceletL_prio)
            else:
                self._on_move_ceg(cart_ctl, c, False)

        def on_load(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo load callback"""
            if cargo_req.content == "helmet":
                self._on_load_ceg(c, cargo_req, "helmet", helmet_src, helmet_prio)
            elif cargo_req.content == "braceletR":
                self._on_load_ceg(c, cargo_req, "braceletR", braceletR_src, braceletR_prio)
            else:
                self._on_load_ceg(c, cargo_req, "braceletL", braceletL_src, braceletL_prio)

        def on_unload(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo unload callback"""
            if cargo_req.content == "helmet":
                self._on_unload_ceg(c, cargo_req, "helmet", helmet_dst, helmet_prio)
            elif cargo_req.content == "braceletR":
                self._on_unload_ceg(c, cargo_req, "braceletR", braceletR_dst, braceletR_prio)
            else:
                self._on_unload_ceg(c, cargo_req, "braceletL", braceletL_dst, braceletL_prio)

        # Setup Cart
        cart_dev = Cart(1, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        cart_ctl = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet_src = "B"
        helmet_dst = "A"
        helmet_prio = False
        helmet = CargoReq(helmet_src, helmet_dst, 100, "helmet")
        helmet.onload = on_load
        helmet.onunload = on_unload

        braceletR_src = "D"
        braceletR_dst = "A"
        braceletR_prio = True
        braceletR = CargoReq(braceletR_src, braceletR_dst, 100, "braceletR")
        braceletR.onload = on_load
        braceletR.onunload = on_unload

        braceletL_src = "D"
        braceletL_dst = "A"
        braceletL_prio = True
        braceletL = CargoReq(braceletL_src, braceletL_dst, 100, "braceletL")
        braceletL.onload = on_load
        braceletL.onunload = on_unload

        # Setup Plan
        Jarvis.reset_scheduler()
        Jarvis.plan(0, self._add_load, (cart_ctl, helmet))
        Jarvis.plan(1, self._add_load, (cart_ctl, braceletR))
        Jarvis.plan(2, self._add_load, (cart_ctl, braceletL))

        # Exercise + Verify indirect output
        Jarvis.run()

        # Verify direct output
        # Unpicked cargo is deleted.
        if braceletR.context is None:
            self._assert_final_cart_ceg(cart_ctl, cart_dev, [helmet, braceletL], braceletL_dst)
            self.assertIsNotNone(braceletL.context)
        else:
           self._assert_final_cart_ceg(cart_ctl, cart_dev, [helmet, braceletR], braceletL_dst)


    def test_ceg_cargo_overload(self) -> None:
        def on_move(c: Cart) -> None:
            """Cart move callback"""
            self._on_move_ceg(cart_ctl, c, False)

        def on_load(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo load callback"""
            self.fail("The cargo should not be loaded. It is too heavy.")

        def on_unload(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo unload callback"""
            self.fail("There is nothing to unload.")

        # Setup Cart
        cart_dev = Cart(2, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        cart_ctl = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet_src = "B"
        helmet_dst = "A"
        helmet = CargoReq(helmet_src, helmet_dst, 500, "helmet")
        helmet.onload = on_load
        helmet.onunload = on_unload

        # Setup Plan
        Jarvis.reset_scheduler()
        Jarvis.plan(0, self._add_load, (cart_ctl, helmet))

        # Exercise + Verify indirect output
        Jarvis.run()

        # Verify direct output
        # Unpicked cargo is deleted.
        self.assertIsNone(helmet.context)
        self._assert_final_cart_ceg(cart_ctl, cart_dev, [])

    def test_ceg_not_picked(self) -> None:
        """CEG: 6
        Normal task + Priority task + Unpicked priority cargo. The unpicked cargo is
        deleted. This deletion should raise an exception.
        """
        def on_move(c: Cart) -> None:
            """Cart move callback"""
            if braceletR in c.slots:
                self._on_move_ceg(cart_ctl, c, braceletR_prio)
            elif braceletL in c.slots:
                self._on_move_ceg(cart_ctl, c, braceletL_prio)
            else:
                self._on_move_ceg(cart_ctl, c, False)

        def on_load(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo load callback"""
            if cargo_req.content == "helmet":
                self._on_load_ceg(c, cargo_req, "helmet", helmet_src, helmet_prio)
            elif cargo_req.content == "braceletR":
                self._on_load_ceg(c, cargo_req, "braceletR", braceletR_src, braceletR_prio)
            else:
                self._on_load_ceg(c, cargo_req, "braceletL", braceletL_src, braceletL_prio)

        def on_unload(c: Cart, cargo_req: CargoReq) -> None:
            """Cargo unload callback"""
            if cargo_req.content == "helmet":
                self._on_unload_ceg(c, cargo_req, "helmet", helmet_dst, helmet_prio)
            elif cargo_req.content == "braceletR":
                self._on_unload_ceg(c, cargo_req, "braceletR", braceletR_dst, braceletR_prio)
            else:
                self._on_unload_ceg(c, cargo_req, "braceletL", braceletL_dst, braceletL_prio)

        # Setup Cart
        cart_dev = Cart(2, 150, 0)
        cart_dev.onmove = on_move

        # Setup Cart Controller
        cart_ctl = CartCtl(cart_dev, Jarvis)

        # Setup Cargo to move
        helmet_src = "A"
        helmet_dst = "C"
        helmet_prio = False
        helmet = CargoReq(helmet_src, helmet_dst, 100, "helmet")
        helmet.onload = on_load
        helmet.onunload = on_unload

        braceletR_src = "A"
        braceletR_dst = "C"
        braceletR_prio = True
        braceletR = CargoReq(braceletR_src, braceletR_dst, 100, "braceletR")
        braceletR.onload = on_load
        braceletR.onunload = on_unload

        braceletL_src = "A"
        braceletL_dst = "D"
        braceletL_prio = True
        braceletL = CargoReq(braceletL_src, braceletL_dst, 100, "braceletL")
        braceletL.onload = on_load
        braceletL.onunload = on_unload

        # Setup Plan
        Jarvis.reset_scheduler()
        Jarvis.plan(0, self._add_load, (cart_ctl, helmet))
        Jarvis.plan(1, self._add_load, (cart_ctl, braceletR))
        Jarvis.plan(2, self._add_load, (cart_ctl, braceletL))

        # Exercise + Verify indirect output
        Jarvis.run()

        # Verify direct output
        # Unpicked cargo is deleted.
        if braceletR.context is None:
            self._assert_final_cart_ceg(cart_ctl, cart_dev, [helmet, braceletL], braceletL_dst)
            self.assertIsNotNone(braceletL.context)
        else:
           self._assert_final_cart_ceg(cart_ctl, cart_dev, [helmet, braceletR], braceletL_dst)


if __name__ == "__main__":
    unittest.main()
