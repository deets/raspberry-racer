import sys
from math import pi, acos

import pyglet
from pymunk.vec2d import Vec2d


class TrackTile(object):

    def __init__(self, width=None, start_tile=False):
        assert width is not None
        self.width = width
        self.compute_bounding_box()
        self.start_tile = start_tile
        

    def compute_bounding_box(self):
        left = bottom = sys.maxint
        right = top = -sys.maxint

        for i in xrange(11):
            step = float(i) / 10.0
            point, tangent = self.point_and_tangent(step)
            a = Vec2d(-tangent.y, tangent.x) * self.width / 2.0
            b = Vec2d(tangent.y, -tangent.x) * self.width / 2.0
            for v in (point + a, point + b):
                left = min(left, v.x)
                right = max(right, v.x)
                bottom = min(bottom, v.y)
                top = max(top, v.y)

        self.left, self.right, self.bottom, self.top = left, right, bottom, top
        


    def __contains__(self, point):
        x, y = point
        return self.left <= x <= self.right and self.bottom <= y <= self.top


class StraightTile(TrackTile):


    def __init__(self, start, direction, **kwargs):
        self.start = Vec2d(*start)
        self.direction = Vec2d(*direction)
        self.length = self.direction.get_length()
        super(StraightTile, self).__init__(**kwargs)
        


    def point_and_tangent(self, d):
        return self.start + self.direction * d, self.direction / self.direction.get_length()
    
        
    def nearest_point_on_slot(self, point):
        P = Vec2d(point)
        S = self.start
        D = self.direction
        l = (P.x * D.x + P.y * D.y - D.x * S.x - S.y * D.y) / (D.x ** 2 + D.y ** 2)
        return l * D + S, l


    def render(self, color):
        S, D = self.start, self.direction
        T = S + D
        vertices = [S.x, S.y, T.x, T.y]
        colors = color + color
        pyglet.graphics.draw(
            2,
            pyglet.gl.GL_LINES,
            ('v2f', vertices),
            ('c3B', colors),
            )

        vertices = [
            self.left, self.bottom, self.right, self.bottom,
            self.right, self.top, self.left, self.top,
            ]
        s = len(vertices) / 2
        colors = color * s
        pyglet.graphics.draw(
            s,
            pyglet.gl.GL_LINE_LOOP,
            ('v2f', vertices),
            ('c3B', colors),
            )

        if not self.start_tile:
            return

        vertices = []
        one = D / 10.0
        two = Vec2d(one.y, -one.x)

        for i in xrange(-4, 5, 2):
            s = S + two * i
            vertices.extend(s)
            vertices.extend(s + one)
            vertices.extend(s + one + two)
            vertices.extend(s + two)

        for i in xrange(-5, 4, 2):
            s = S + two * i + one
            vertices.extend(s)
            vertices.extend(s + one)
            vertices.extend(s + one + two)
            vertices.extend(s + two)
    

        s = len(vertices) / 2
        colors = (255,255,255) * s

        pyglet.graphics.draw(
            s,
            pyglet.gl.GL_QUADS,
            ('v2f', vertices),
            ('c3B', colors),
            )


class CurveTile(TrackTile):


    def __init__(self, start, direction, radius, curve, *args, **kwargs):
        start = Vec2d(*start)
        self.start = start
        self.radius = radius
        self.curve = curve
        direction = Vec2d(*direction)
        self.direction = direction / direction.get_length()
        v = self.direction * radius
        if curve >= 0: # turning left
            v = Vec2d(-v.y, v.x)
        else:
            v = Vec2d(v.y, -v.x)
            
        self.center = start + v
        self.hand = -v
        self.length = radius * abs(curve)
        super(CurveTile, self).__init__(*args, **kwargs)
        

    def point_and_tangent(self, step):
        d = self.curve * step
        tangent = self.direction.rotated(d)
        point = self.hand.rotated(d) + self.center
        return point, tangent
        

    def nearest_point_on_slot(self, point):
        point = Vec2d(*point)
        p = point - self.center
        np = self.center + (p * (self.radius / p.get_length()))

        a = self.hand / self.hand.get_length()
        b = np - self.center
        b = b / b.get_length()
        relpos = acos(b2Dot(a, b)) / abs(self.curve)
        return np, relpos


    def render(self, color):
        vertices = []
        for i in xrange(17):
            point, _ = self.point_and_tangent(float(i) / 16.0)
            vertices.extend(point)

        l = len(vertices) / 2
        colors = tuple(color) * l
            
        pyglet.graphics.draw(
            l,
            pyglet.gl.GL_LINE_STRIP,
            ('v2f', vertices),
            ('c3B', colors),
            )


class CarInfo(object):

    def __init__(self, track, car, player_state):
        self.car = car
        self.player_state = player_state
        self.track = track
        self.rounds = 0
        self.current_tile = None
        self.first_tile = track.tiles[0]
        self.last_tile = track.tiles[-1]
        self.on_last_tile = False
        self.track_position = .0
        
    def place_to_facing(self, point, dir):
        self.car.place_to_facing(point, dir)


    def constrain_car(self):
        return 
        guide_p = self.car.guide_position
        tile, (slot_p, relpos) = self.track.nearest_point_on_slot(guide_p)
        self.current_tile = tile
        self.track_position = tile.offset + tile.length * relpos
        if tile is None:
            return
        push_v = slot_p - guide_p
        l = push_v.get_length()
        push_v = push_v / l
        f = l
        if l < self.track.CONSTRAINT_THRESHOLD:
            return
        self.car.apply_impulse(push_v * f * self.track.IMPULSE_FACTOR, guide_p)


    def update(self, dt, keys):
        ps = self.player_state
        ps.update(dt, keys)
        if ps.braking:
            self.car.brake()
        else:
            self.car.drive(ps.throttle)
            
        self.constrain_car()
        self.car.update(dt)
        
        # first, check for transition
        # of tiles
        if self.on_last_tile and self.current_tile is self.first_tile:
            self.rounds += 1

        # update on_last_tile
        self.on_last_tile = self.current_tile is self.last_tile
        
        
class Track(object):
    CONSTRAINT_THRESHOLD = .02
    IMPULSE_FACTOR = 100.0


    def __init__(self, definition, position, scale=100, track_color=(255, 255, 0)):
        self.track_color = track_color
        self.cars = []
        
        definition = definition.strip("\n")
        defarray = []
        sx, sy = position
        for line in definition.split("\n"):
            la = list(line)
            defarray.append(la)

        width = max(len(line) for line in defarray)
        height = len(defarray)

        defmap = {}
        start_x, start_y = -1, -1
        
        for x in xrange(width):
            for y in xrange(height):
                tc = defarray[y][x]
                if tc in "><":
                    start_x, start_y = x, y
                if tc in " \n\r\t":
                    continue
                defmap[(x, y)] = tc

        assert start_x != -1

        self.position = position = Vec2d(*position)
        
        stc = defmap[(start_x, start_y)]
        if stc == ">":
            d = 1, 0
        else:
            raise NotImplementedError

        def direction():
            return Vec2d(d[0], -d[1])
        
        tiles = []
        tiles.append(
            StraightTile(position, direction() * scale, width=scale, start_tile=True),
            )

        x, y = start_x, start_y

        while True:
            x += d[0]
            y += d[1]
            # continue from the last tile
            point, tangent = tiles[-1].point_and_tangent(1.0)

            tc = defmap[(x, y)]
            if tc in "><":
                break
            if tc in "-|":
                new_tile = StraightTile(
                    point,
                    tangent * scale,
                    width=scale,
                    )
            elif tc == "+":
                sd = d[1], -d[0]
                sx, sy = x + sd[0], y + sd[1]
                if (sx, sy) not in defmap:
                    sd = -d[1], d[0]
                    sx, sy = x + sd[0], y + sd[1]
                    if (sx, sy) not in defmap:
                        raise Exception("Found no continuation after curve")
                    else:
                        curve = -pi / 2
                else:
                    curve = pi / 2
                    
                new_tile = CurveTile(
                    point,
                    tangent,
                    scale / 2.0,
                    curve=curve,
                    width=scale,
                    )
                d = sd
            tiles.append(new_tile)
                
        self.tiles = tiles
        self.start_tile = self.tiles[0]

        length = 0.0
        for tile in tiles:
            tile.offset = length
            length += tile.length
        self.length = length
        

    def reset(self):
        for car in self.cars:
            car.place_to_facing(self.start_tile.start, self.start_tile.direction)
            

    def render(self):
        for tile in self.tiles:
            tile.render(self.track_color)



    def update(self, dt, keys):
        for car_info in self.cars:
            car_info.update(dt, keys)
            
         
    def nearest_point_on_slot(self, guide_flag):
        for tile in self.tiles:
            if guide_flag in tile:
                return tile, tile.nearest_point_on_slot(guide_flag)
        return None, (None, None)


    def add_car(self, car, player_state):
        ci = CarInfo(self, car, player_state)
        self.cars.append(ci)
        return ci
    
        

             
         
         
if __name__ == "__main__":
    a = StraightTile((0, 0), (10, 0), width=10.0)
    p = (1, 2)
    assert p in a
    np = a.nearest_point_on_slot(p)
    assert np == Vec2d(1, 0)


    a = StraightTile((0, 0), (0, 10), width=10.0)
    p = (1, 2)
    assert p in a
    np = a.nearest_point_on_slot(p)
    assert np == Vec2d(0, 2)
    
    a = CurveTile((0, 0), (1, 0), 5, pi / 2, width=10.0)

    p = (1, 2)
    assert p in a
    np = a.nearest_point_on_slot(p)
    print np
    
    
    track = Track("""
+---->------+
|           |
|         +-+
|         |  
+---------+  
""", (0, 0), 100)

    assert any(isinstance(tile, CurveTile) for tile in  track.tiles)
