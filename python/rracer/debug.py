from math import pi, sin, cos

import pyglet
from pymunk.vec2d import Vec2d


class DebugDraw(object):


    def __init__(self):
        super(DebugDraw, self).__init__()
        self.draw_ops = []

        self.batch = pyglet.graphics.Batch()
        self.circle_vertices = vertices = []
        steps = 16
        step = pi * 2 / steps
        for i in xrange(steps):
            ov = Vec2d(cos(i * step), sin(i * step))
            vertices.extend(ov)
            

    def pre_world_step(self):
        self.draw_ops = []


    def render(self):
        for draw_op in self.draw_ops:
            draw_op()


    def DrawPoint(self, p, size, color):
        print "DrawPoint"
        

    def DrawAABB(self, aabb, color):
        print "DrawAABB"
        

    def DrawSegment(self, p1, p2, color):
        color = [int(c * 255) for c in color]

        in_vertices = [p1, p2]
        vertices = []
        data = []
        for vertex in in_vertices:
            vertices.extend(vertex)
            data.extend(color)
        
        def render():
            pyglet.graphics.draw(
                len(in_vertices),
                pyglet.gl.GL_LINES,
                ('v2f', vertices),
                ('c3B', data),
                )
        self.draw_ops.append(render)
        
        

    def DrawCircle(self, center, radius, color, drawwidth=1):

        gl = pyglet.gl
        
        vertices = self.circle_vertices
        center = box2d.b2Vec2(*center)

        color = tuple(int(c * 255) for c in color)
        s = len(vertices) / 2
        colors = color * s
        
        def render():
            gl.glMatrixMode(gl.GL_MODELVIEW)
            gl.glPushMatrix()
            gl.glTranslated(center.x, center.y, 0)
            gl.glScaled(radius, radius, 1.0)
            
            pyglet.graphics.draw(
                s,
                pyglet.gl.GL_LINE_LOOP,
                ('v2f', vertices),
                ('c3B', colors),
                )

            gl.glPopMatrix()
            
        self.draw_ops.append(render)

        
    def DrawSolidCircle(self, center_v, radius, axis, color):
        print "DrawSolidCircle"


    def DrawPolygon(self, polygon, color=(1, 1, 0)):
        in_vertices = polygon.get_points()

        color = [int(c * 255) for c in color]
        
        vertices = []
        data = []
        for vertex in in_vertices:
            vertices.extend((vertex[0], vertex[1]))
            data.extend(color)

        
        def render():
            pyglet.graphics.draw(
                len(in_vertices),
                pyglet.gl.GL_LINE_LOOP,
                ('v2f', vertices),
                ('c3B', data),
                )
        self.draw_ops.append(render)



    def DrawXForm(self, xf):
        p1 = xf.position * 1.0 # create a *copy*, not a reference!
        k_axisScale = .5
        p2 = p1 + k_axisScale * xf.R.col1
        p3 = p1 + k_axisScale * xf.R.col2

        def render():
            color = (255,0,0)
            vertices = (p1[0], p1[1], p2[0], p2[1])
            pyglet.graphics.draw(
                2,
                pyglet.gl.GL_LINES,
                ('v2f', vertices),
                ('c3B', (color + color)),
                )

            color = (0,255,0)
            pyglet.graphics.draw(
                2,
                pyglet.gl.GL_LINES,
                ('v2f', (p1[0], p1[1], p3[0], p3[1])),
                ('c3B', (color + color)),
                )

        self.draw_ops.append(render)

        
    def DrawSolidPolygon(self, in_vertices, color):
        color = [int(c * 255) for c in color]
        vertices = []
        data = []
        for vertex in in_vertices:
            vertices.extend(vertex)
            data.extend(color)

        
        def render():
            pyglet.graphics.draw(
                len(in_vertices),
                pyglet.gl.GL_POLYGON,
                ('v2f', vertices),
                ('c3B', data),
                )
        self.draw_ops.append(render)



