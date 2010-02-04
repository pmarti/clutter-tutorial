import os
import sys

import clutter


stage = None

# For showing the filename
label_filename = None

# For rotating all images around an ellipse
timeline_rotation = None

# For moving one image up and scaling it
timeline_moveup = None
behaviour_scale = None
behaviour_path = None
behaviour_opacity = None

# The y position of the ellipse of images
ELLIPSE_Y = 390
# The distance from front to back when it's rotated 90 degrees.
ELLIPSE_HEIGHT = 450
IMAGE_HEIGHT = 100

angle_step = 30


class Item(object):

    def __init__(self, actor, filepath, ellipse_behaviour=None):
        super(Item, self).__init__()
        self.actor = actor
        self.filepath = filepath
        self.ellipse_behaviour = ellipse_behaviour

    def __eq__(self, other):
        return self.filepath == other.filepath

    def __ne__(self, other):
        return not self.__eq__(other)


item_at_front = None
list_items = []

def on_foreach_clear_list_items(data, user_data):
    pass


def scale_texture_default(texture):
    pixbuf_height = texture.get_size()[1]

    scale = IMAGE_HEIGHT / float(pixbuf_height) if pixbuf_height else 0
    texture.set_scale(scale, scale)


def load_images(directory_path):
    # Clear any existing images
    if not os.path.exists(directory_path):
        return

    # Clear any existing images
    global list_items
    list_items = []

    # Discover the images in the directory
    for image in os.listdir(directory_path):
        if not image.endswith('.jpg'):
            continue

        path = os.path.join(directory_path, image)

        # Try to load the file as an image
        actor = clutter.Texture(path)
        if actor:
            item = Item(actor, path)

            # Make sure that all images are shown with the same height
            scale_texture_default(item.actor)
            list_items.append(item)


def add_to_ellipse_behaviour(timeline_rotation, start_angle, item):
    if not timeline_rotation:
        return

    alpha = clutter.Alpha(timeline_rotation, clutter.EASE_OUT_SINE)
    item.ellipse_behaviour = clutter.BehaviourEllipse(alpha,
                               320, ELLIPSE_Y, # x, y
                               ELLIPSE_HEIGHT, ELLIPSE_HEIGHT, # width, height
                               start_angle, start_angle + 360)
    item.ellipse_behaviour.set_direction(clutter.ROTATE_CW)
    item.ellipse_behaviour.set_angle_tilt(clutter.X_AXIS, -90)
    item.ellipse_behaviour.apply(item.actor)


def add_image_actors():
    x, y, angle = 20, 0, 0
    global list_items
    global stage
    global timeline_rotation

    for item in list_items:
        # Add the actor to the stage
        stage.add(item.actor)

        # Set an initial position
        item.actor.set_position(x, y)
        y += 100

        # Allow the actor to emit events. By default only the stage does this.
        item.actor.set_reactive(True)

        # Connect signal handlers for events
        item.actor.connect('button-press-event', on_texture_button_press, item)

        add_to_ellipse_behaviour(timeline_rotation, angle, item)
        angle += angle_step

        item.actor.show()


def angle_in_360(angle):
    result = angle
    while result >= 360:
        result -= 360

    return result


# This signal handler is called when the item has finished
# moving up and increasing in size.
def on_timeline_moveup_completed(timeline):
    global timeline_moveup, behaviour_scale, behaviour_path, behaviour_opacity
    timeline_moveup = behaviour_scale = None
    behaviour_path = behaviour_opacity = None


# This signal handler is called when the items have completely
# rotated around the ellipse.
def on_timeline_rotation_completed(timeline):
    # All the items have now been rotated so that the clicked item is at the
    # front.  Now we transform just this one item gradually some more, and
    # show the filename.

    # Transform the image
    global timeline_moveup
    actor = item_at_front.actor
    timeline_moveup = clutter.Timeline(1000) # milliseconds
    alpha = clutter.Alpha(timeline_moveup, clutter.EASE_OUT_SINE)

    # Scale the item from its normal scale to approximately twice the normal scale
    scale_start = actor.get_scale()[0]
    scale_end = scale_start * 1.8

    global behaviour_scale
    behaviour_scale = clutter.BehaviourScale(scale_start, scale_start,
                                             scale_end, scale_end,
                                             alpha=alpha)
    behaviour_scale.apply(actor)

    # Move the item up the y axis
    knots = (
        (int(actor.get_x()), int(actor.get_y())),
        (int(actor.get_x()), int(actor.get_y() - 250)),
    )
    global behaviour_path
    behaviour_path = clutter.BehaviourPath(alpha, knots=knots)
    behaviour_path.apply(actor)

    # Show the filename gradually
    global label_filename
    label_filename.set_text(item_at_front.filepath)
    global behaviour_opacity
    behaviour_opacity = clutter.BehaviourOpacity(0, 255, alpha=alpha)
    behaviour_opacity.apply(label_filename)

    # Start the timeline and handle its "completed" signal so we can unref it
    timeline_moveup.connect('completed', on_timeline_moveup_completed)
    timeline_moveup.start()


def rotate_all_until_item_is_at_front(item):
    if not item:
        return

    global timeline_rotation
    timeline_rotation.stop()

    # Stop the other timeline in case that is active at the same time
    global timeline_moveup
    if timeline_moveup:
        timeline_moveup.stop()

    global label_filename
    label_filename.set_opacity(0)

    # Get the item's position in the list
    global list_items
    pos = list_items.index(item)
    assert pos != -1

    global item_at_front
    if not item_at_front and list_items:
        item_at_front = list_items[0]

    pos_front = 0
    if item_at_front:
        pos_front = list_items.index(item_at_front)
    assert pos_front != -1

    # pos_offset_before_start = pos_front - pos

    # Calculate the end angle of the first item
    angle_front = 180
    angle_start = angle_front - (angle_step * pos_front)
    angle_start = angle_in_360(angle_start)
    angle_end = angle_front - (angle_step * pos)

    angle_diff = 0

    # Set the end angles
    for this_item in list_items:
        # Reset its size
        scale_texture_default(this_item.actor)

        angle_start = angle_in_360(angle_start)
        angle_end = angle_in_360(angle_end)

        # Move 360 instead of 0
        # when moving for the first time,
        # and when clicking on something that is already at the front.
        if item_at_front == item:
            angle_end += 360

        this_item.ellipse_behaviour.set_angle_start(angle_start)
        this_item.ellipse_behaviour.set_angle_end(angle_end)

        if this_item == item:
            if angle_start < angle_end:
                angle_diff = angle_end - angle_start
            else:
                angle_diff = 360 - (angle_start - angle_end)

            # print "    debug: angle diff=%f" % angle_diff

        # TODO: Set the number of frames, depending on the angle.
        # otherwise the actor will take the same amount of time to reach
        # the end angle regardless of how far it must move, causing it to
        # move very slowly if it does not have far to move.
        angle_end += angle_step
        angle_start += angle_step

    # Set the number of frames to be proportional to the distance to travel,
    # so the speed is always the same
    pos_to_move = 0
    if pos_front < pos:
        count = len(list_items)
        pos_to_move = count + (pos - pos_front)
    else:
        pos_to_move = pos_front - pos

    timeline_rotation.set_duration(int(angle_diff * .2))

    # Remember what item will be at the fron when this timeline finishes
    item_at_front = item

    timeline_rotation.start()


def on_texture_button_press(actor, event, item):
    # Ignore the events if the timeline_rotation is running (meaning, if the objects are moving)
    # to simplify things
    global timeline_rotation
    if timeline_rotation and timeline_rotation.is_playing():
        print "on_texture_button_press(): ignoring"
        return False
    else:
        print "on_texture_button_press(): handling"

    rotate_all_until_item_is_at_front(item)
    return True


def main():
    stage_color = clutter.Color(176, 176, 176, 255) # light gray

    # Get the stage and set its size and color
    global stage
    stage = clutter.Stage()
    stage.set_size(800, 600)
    stage.set_color(stage_color)

    # Create and add a label actor, hidden at first
    global label_filename
    label_filename = clutter.Text()
    label_color = clutter.Color(96, 96, 144, 255) # blueish
    label_filename.set_color(label_color)
    label_filename.set_font_name("Sans 24")
    label_filename.set_position(10, 10)
    label_filename.set_opacity(0)
    stage.add(label_filename)
    label_filename.show()

    # Add a plane under the ellipse of images
    rect_color = clutter.Color(255, 255, 255, 255) # white
    rect = clutter.Rectangle(rect_color)
    rect.set_height(ELLIPSE_HEIGHT + 20)
    rect.set_width(stage.get_width() + 100)
    # Position it so that its center is under the images
    rect.set_position(-(rect.get_width() - stage.get_width()) / 2,
                      ELLIPSE_Y + IMAGE_HEIGHT - (rect.get_height() / 2))
    # Rotate it around its center
    rect.set_rotation(clutter.X_AXIS, -90, 0, rect.get_height() / 2, 0)
    stage.add(rect)
    rect.show()

    # show the stage
    stage.connect("destroy", clutter.main_quit)
    stage.show_all()

    global timeline_rotation
    timeline_rotation = clutter.Timeline(2000) # milliseconds
    timeline_rotation.connect('completed', on_timeline_rotation_completed)

    # Add an actor for each image
    load_images("images")
    add_image_actors()

    # timeline_rotation.set_loop(True)

    # Move them a bit to start with
    global list_items
    if list_items:
        rotate_all_until_item_is_at_front(list_items[0])

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())
