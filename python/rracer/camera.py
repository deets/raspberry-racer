import pyglet

class Camera(object):

    def __init__(self, win, zoom=480):
        self.win = win
        self.zoom = zoom
        self.center = win.width, win.height
        self.tracking_object = None



    def follow(self, tracking_object):
        self.tracking_object = tracking_object


    def update(self, dt):
        if self.tracking_object is not None:
            self.center = self.tracking_object.position
        

    def worldProjection(self):
        gl = pyglet.gl
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        widthRatio = float(self.win.width) / float(self.win.height)
        gl.gluOrtho2D(
            -self.zoom * widthRatio,
            self.zoom * widthRatio,
            -self.zoom,
            self.zoom)
        gl.glTranslated(-self.center[0], -self.center[1], .0)
        

    def hudProjection(self):
        gl = pyglet.gl
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        gl.gluOrtho2D(0, self.win.width, 0, self.win.height)

