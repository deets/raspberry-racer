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


    def add(self, body_or_joint, shape=None):
        if shape is not None:
            self.space.add(body_or_joint, shape)
        else:
            self.space.add(body_or_joint)


    def render(self):
        if self.debug_draw:
            self.debug_draw.render()


    
