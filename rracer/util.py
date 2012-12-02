import pyglet
from pymunk import BB

class Grid(object):


    def __init__(self, world_box, step=1.0):
        self.box = BB(*world_box)
        self.step = step

        left = self.box.left
        right = self.box.right
        step = self.step
        
        color = [255, 255, 255]
        vertices = []
        vcount = 0
        colors = []
        while left <= right:
            bottom = self.box.bottom
            top = self.box.top
            while bottom <= top:
                colors.extend(color)
                vertices.extend([left, bottom])
                vcount += 1
                bottom += step
            left += step

        self.vertices = vertices
        self.vcount = vcount
        self.colors = colors

    def render(self):
        pyglet.graphics.draw(
            self.vcount,
            pyglet.gl.GL_POINTS,
            ('v2f', self.vertices),
            ('c3B', self.colors),
            )
        
                
class SkidMarks(object):

    def __init__(self, max_size=100, color=(128, 128, 128)):
        self.max = max_size
        self.skidmarks = []
        self.colors = []
        for _ in xrange(self.max):
            self.colors.extend(color)
            

    def __iadd__(self, skidmark):
        self.skidmarks.extend(skidmark)
        self.skidmarks = self.skidmarks[-self.max * 2:]
        return self
    

    def render(self):
        if not self.skidmarks:
            return
        c = len(self.skidmarks) / 2
        colors = self.colors[:c * 3]

        pyglet.graphics.draw(
            c,
            pyglet.gl.GL_POINTS,
            ('v2f', self.skidmarks),
            ('c3B', colors),
            )
             
