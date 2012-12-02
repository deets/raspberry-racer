from pymunk import Space, Poly


class World(object):


    def __init__(self, debug_draw=None):
        self.space = Space()
        self.debug_draw = debug_draw


    def step(self, elapsed):
        if self.debug_draw:
            self.debug_draw.pre_world_step()
        self.space.step(elapsed)

        if self.debug_draw:
            for shape in self.space.shapes:
                if isinstance(shape, Poly):
                    self.debug_draw.DrawPolygon(shape)


    def add(self, body, shape):
        self.space.add(body, shape)


    def render(self):
        if self.debug_draw:
            self.debug_draw.render()


    
