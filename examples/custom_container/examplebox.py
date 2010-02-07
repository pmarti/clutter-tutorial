import gobject
import clutter
from clutter import cogl


class ExampleBox(clutter.Actor, clutter.Container):
    """
    Simple example of a container actor

    ExampleBox imposes a specific layout on its children,
    unlike clutter.Group which is a free-form container.
    """
    __gtype_name__ = 'ExampleBox'

    def __init__(self):
        super(ExampleBox, self).__init__()
        self._children = []

    def do_add(self, *children):
        for child in children:
            if child in self._children:
                raise ValueError("Actor %s is already a children of %s" % (
                    child, self))

            self._children.append(child)
            child.set_parent(self)
            self.queue_relayout()

    def do_remove(self, *children):
        for child in children:
            if child in self._children:
                self._children.remove(child)
                child.unparent()
                self.queue_relayout()
            else:
                raise ValueError("Actor %s is not a child of %s" % (
                    child, self))


    def do_remove_all(self):
        """Removes all child actors from the ExampleBox"""
        self.do_remove(*self._children)

    def do_show_all(self):
        for child in self._children:
            child.show()

    def do_hide_all(self):
        for child in self._children:
            child.hide()

    def do_foreach(self, func, data):
        for child in self._children:
            func(child, data)

    def do_paint(self):
        cogl.push_matrix()
        for child in self._children:
            if child.props.mapped:
                child.paint()

        cogl.pop_matrix()

    def do_pick(self, color):
        for child in self._children:
            if child.props.mapped:
                child.do_pick(child, color)

    def do_allocate(self, box, flags):
        child_x = child_y = 0
        for child in self._children:
            # Discover what size the child wants
            child_width, child_height = child.get_preferred_size()[2:]

            # Calculate the position and size that the child may actually have

            # Position the child just after the previous child, horizontally
            child_box = clutter.ActorBox()
            child_box.x1 = child_x
            child_box.x2 = child_x + child_width
            child_x = child_box.x2

            # Position the child at the top of the container
            child_box.y1 = 0
            child_box.y2 = child_box.y1 + child_height

            # Tell the child what position and size it may actually have
            child.allocate(child_box, flags)

        clutter.Actor.do_allocate(self, box, flags)

    def get_preferred_height(self):
        min_height = natural_height = 0
        # For this container, the preferred height is the maximum height
        # of the children. The preferred height is independent of the given width.

        # Calculate the preferred height for this container,
        # based on the preferred height requested by the children
        for child in self._children:
            if not child.props.visible:
                child_min_height, child_natural_height = child.get_preferred_height(-1)
                min_height = max(min_height, child_min_height)
                natural_height = max(natural_height, child_natural_height)

        return min_height, natural_height

    def get_preferred_width(self):
        min_width = natural_width = 0

        # For this container, the preferred width is the sum of the widths
        # of the children. The preferred width depends on the height provided
        # by for_height.

        # Calculate the preferred width for this container,
        # based on the preferred width requested by the children
        for child in self._children:
            if child.props.visible:
                child_min_width, child_natural_width = child.get_preferred_width(for_height)

                min_width += child_min_width
                natural_width += child_natural_width

        return min_width, natural_width


gobject.type_register(ExampleBox)

