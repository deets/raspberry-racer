#!/usr/bin/arch -i386  /Library/Frameworks/Python.framework/Versions/2.7/bin/python2.7
import pyglet
from pyglet.window import key
## from car import Car
## from debug import DebugDraw
## from track import Track
## from camera import Camera
## from util import Grid, SkidMarks


## track = Track("""
##  +----->-----+
##  |           |
##  |         +-+
##  |         |  
##  +---------+  
## """, (0, 0), 10.0)

## skidmarks = SkidMarks()
## car = Car(world, (0, 0), skidmarks=skidmarks)
## grid = Grid(worldAABB, step=3.0)
## camera = Camera(window, zoom=20.0)
## camera.follow(car)


class PlayerState(object):


    def __init__(self, throttle_key):
        self.throttle = .0
        self.braking = True
        self.throttle_key = throttle_key
        

    def update(self, dt, keys):
        if keys[self.throttle_key]:
            self.throttle = 1.0
            self.braking = False
        else:
            self.throttle = .0
            self.braking = True

def update(dt):
    ## debugDraw.pre_world_step()
    ## track.update(dt, keys)
    ## camera.update(dt)
    ## world.Step(dt, vel_iters, pos_iters)


    ## camera.worldProjection()
    ## #car.blit(*body.position)
    ## grid.render()
    ## track.render()
    ## _, (slot_position, _) = track.nearest_point_on_slot(car.guide_position)
    ## if slot_position:
    ##     debugDraw.DrawCircle(slot_position, 2.0, (1.0, 0, 0))
    ## skidmarks.render()
    ## debugDraw.render()
    ## camera.hudProjection()
    ## fps_display.draw()
    ## ci_display.draw()
    pass


def main():
    window = pyglet.window.Window()

    keys = key.KeyStateHandler()
    window.push_handlers(keys)
        
    pyglet.clock.schedule_interval(update, 1/60.0) # update at 60Hz

    @window.event
    def on_draw():
        window.clear()

    pyglet.app.run()



## player_state = PlayerState(key.UP)
## ci = track.add_car(car, player_state)
## track.reset()


## class CarInfoDisplay(object):

##     def __init__(self, window, car_info):
##         self.window = window
##         self.car_info = car_info


##     def draw(self):
##         label = pyglet.text.Label(
##             'Position: %.2f' % self.car_info.track_position, 
##             font_name='Times New Roman', 
##             font_size=20,
##             x=20, y=self.window.height - 30,
##             anchor_x='left', anchor_y='center')
##         label.draw()

##         label = pyglet.text.Label(
##             'Rounds: %i' % self.car_info.rounds,
##             font_name='Times New Roman', 
##             font_size=20,
##             x=20, y=self.window.height - 30 - 25,
##             anchor_x='left', anchor_y='center')
##         label.draw()

        
        
## ci_display = CarInfoDisplay(window, ci)


