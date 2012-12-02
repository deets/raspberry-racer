from math import atan2, pi

import pyglet
from pymunk import (
    Body,
    Poly,
    moment_for_box,
    moment_for_circle,
    )


class WorldObject(object):


    def __init__(self, world, body, shape):
        self.world = world
        self.body = body
        self.shape = shape
        self.world.add(body, shape)
        

    @property
    def center(self):
        return self.body.GetWorldCenter()
    
    
    @property
    def lateral_velocity(self):
        currentRightNormal = self.body.local_to_world( (1,0) )
        return box2d.b2Dot( currentRightNormal, self.body.GetLinearVelocity() ) * currentRightNormal


    @property
    def forward_velocity(self):
        currentRightNormal = self.body.local_to_world( (0,1) )
        return box2d.b2Dot( currentRightNormal, self.body.GetLinearVelocity() ) * currentRightNormal

    @property
    def position(self):
        return self.body.position


    @property
    def angle(self):
        return self.body.angle


    def apply_force(self, force):
        self.body.ApplyForce(force, self.center)


    def apply_torque(self, torque):
        self.body.ApplyTorque(torque)


    def apply_impulse(self, impulse, position):
        self.body.ApplyImpulse( impulse, position)




class Tire(WorldObject):


    MAX_DRIVE_FORCE = 140
    BRAKE_FORCE = 100
    MAX_LATERAL_IMPULSE = 10.0
    DRAG_COEFFICIENT = .8
    
    
    def __init__(self, car, position, width=10.0, length=20.0, steerable=True, skidmarks=None, powered=True, density=1.0):
        world = car.world
        self.steerable = steerable
        self.powered = powered


        mass = width * length * density
        inertia = moment_for_box(mass, length, width)
        self.world = world
        body = Body(mass, inertia, )
        body.position = position
        shape= Poly.create_box(body, size=(width, length))
        super(Tire, self).__init__(world, body, shape)
        
        ## self.bodyDef = box2d.b2BodyDef()
        ## self.bodyDef.position = position

        
        ## body = world.CreateBody(self.bodyDef)
        ## super(Tire, self).__init__(world, body)

        ## self.shapeDef = box2d.b2PolygonDef()
        ## self.shapeDef.density = 1
        ## self.shapeDef.SetAsBox(width, length)
        ## self.shap = self.body.CreateShape(self.shapeDef)
        ## self.body.SetMassFromShapes()

        # our joint

        ## jointDef = box2d.b2RevoluteJointDef()
        ## jointDef.Initialize(car.body, self.body, self.center)
        ## jointDef.lowerAngle    = 0
        ## jointDef.upperAngle    = 0
        ## jointDef.enableLimit   = True
        ## jointDef.collideConnected = False
        ## self.joint = world.CreateJoint(jointDef)
        ## self.skidmarks = skidmarks
        

    def update(self, dt):
        return
    
        impulse = self.body.GetMass() * -self.lateral_velocity
        il = impulse.get_length()
        mli = self.MAX_LATERAL_IMPULSE
        if self.steerable:
            mli *= 1.5
        if il > mli:
            impulse *= mli / il
            if self.skidmarks:
                self.skidmarks += self.center
                
        self.body.ApplyImpulse( impulse, self.center)
        self.apply_force(self.forward_velocity * -self.DRAG_COEFFICIENT)


    def drive(self, direction):
        if self.powered:
            currentForwardNormal = self.body.local_to_world( (0,1) )
            force = self.MAX_DRIVE_FORCE * direction
            self.apply_force( force * currentForwardNormal)


    def brake(self):
        n = self.forward_velocity
        if not n.get_length():
            return
        if n.get_length() > 0:
            n = n / n.get_length()
        self.apply_force(-n * self.BRAKE_FORCE)


    def steer(self, direction):
        if self.steerable:
            self.joint.lowerAngle = direction
            self.joint.upperAngle = direction
        
    


class Car(WorldObject):


    def __init__(self, world, position, width=1.6, length=4.0, tire_width=.25, tire_length=.8, skidmarks=None, body_density=1.0):
        mass = width * length * body_density
        inertia = moment_for_box(mass, length, width)
        self.world = world
        body = Body(mass, inertia, )
        body.position = position
        shape= Poly.create_box(body, size=(width, length))
        super(Car, self).__init__(world, body, shape)

        slot_density = .01
        slot_radius = .1
        slot_mass = slot_density * (slot_radius ** 2) * pi
        slot_inertia = moment_for_circle(slot_mass, 0.0, slot_radius)
        self.slot = Body(slot_mass, slot_inertia)


        flpos = position[0] - width / 2.0 - tire_width * 2, position[1] + length / 2.0
        self.front_left = Tire(self, flpos, tire_width, tire_length, skidmarks=skidmarks, powered=False)


        frpos = position[0] + width / 2.0 + tire_width * 2, position[1] + length / 2.0
        self.front_right = Tire(self, frpos, tire_width, tire_length, skidmarks=skidmarks, powered=False)
        
        rlpos = position[0] - width / 2.0 - tire_width * 2, position[1] - length / 2.0
        self.rear_left = Tire(self, rlpos, tire_width, tire_length, steerable=False, skidmarks=skidmarks)

        rrpos = position[0] + width / 2.0 + tire_width * 2, position[1] - length / 2.0
        self.rear_right = Tire(self, rrpos, tire_width, tire_length, steerable=False, skidmarks=skidmarks)
        self.tires = [self.front_left, self.front_right, self.rear_left, self.rear_right]


    def render(self):
        #print self.shape.get_points()
        pass
        


    def drive(self, direction):
        return
        for tire in self.tires:
            tire.drive(direction)


    def brake(self):
        return
        for tire in self.tires:
            tire.brake()
            

    def steer(self, direction):
        return
        for tire in self.tires:
            tire.steer(direction)


    def update(self, dt):
        for tire in self.tires:
            tire.update(dt)


    @property
    def guide_position(self):
        return self.body.local_to_world(self.slot.position)



    def place_to_facing(self, guide_position, direction):
        n = direction / direction.get_length()
        a = -atan2(*n)
        da = a - self.angle
        tire_vecs = [tire.position - self.position for tire in self.tires]
        return
        tire_tf = box2d.b2Mat22(da)
        tire_vecs = [box2d.b2Mul(tire_tf, v) for v in tire_vecs]
        self.body.setAngle(a)
        po = self.position - self.guide_position
        self.body.setPosition(guide_position + po)
        for vec, tire in zip(tire_vecs, self.tires):
            tire.body.setPosition(guide_position + po + vec)
            tire.body.setAngle(a)
