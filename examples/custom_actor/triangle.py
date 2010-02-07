import gobject
import clutter
from clutter import cogl


class Triangle(clutter.Actor):
    __gtype_name__ = 'Triangle'
    __gproperties__ = {
        'color' : (
            str, 'color', 'Color', 'The color of the triangle', gobject.PARAM_READWRITE,
        ),
    }

    def __init__(self, color=None):
        super(Triangle, self).__init__()
        if color is not None:
            self._color = color

    def do_set_property (self, pspec, value):
        if pspec.name == 'color':
            self._color = clutter.color_from_string(value)
        else:
            raise TypeError('Unknown property %s' % pspec.name)

    def do_get_property (self, pspec):
        if pspec.name == 'color':
            return self._color
        else:
            raise TypeError('Unknown property %s' % pspec.name)

    def do_paint (self):
        # Paint the triangle with the actor's color
        paint_color = self._color

        real_alpha = self.get_paint_opacity() * paint_color.alpha / 255
        paint_color.alpha = real_alpha

        self.__paint_triangle(paint_color)

    def __paint_triangle (self, color):
        cogl.push_matrix()
        cogl.set_source_color(color)

        width, height = self.get_geometry()[2:]

        # Paint a triangle
        cogl.path_polygon(0, 0, 0, height, width, height)
        cogl.path_fill()
        cogl.pop_matrix()

    def do_pick (self, pick_color):
        if self.should_pick_paint():
            self.__paint_triangle(pick_color)

    def get_color(self):
        return self.color

    def set_color(self, color):
        self._color = clutter.color_from_string(color)
        self.set_opacity(self._color.alpha)
        if self.props.visible:
            clutter.Actor.queue_redraw(self)


gobject.type_register(Triangle)
