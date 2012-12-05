from unittest import TestCase

## from pymunk import (

##     )
from rracer.car import Tire, Car
from rracer.world import World


class PhysicsTests(TestCase):


    def test_tires(self):
        frame = 1.0 / 30.0
        
        world = World()
        
        car = Car(world, (0, 0))

        rr = car.tires[-1]

        print rr.forward_normal
        print rr.right_normal
        import pdb;pdb.set_trace()
        for _ in xrange(10):
            car.drive(1.0)
            car.update(frame)
            world.step(frame)
            
        print rr.forward_normal
        print rr.right_normal
