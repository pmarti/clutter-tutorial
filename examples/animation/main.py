import sys

import clutter


rect = None

# This must return a value between 0 and 1.0
#
# This will be called as many times per seconds as specified in our call to clutter_timeline_new().
def on_alpha(alpha, data):
    # Get the position in the timeline,
    # so we can base our value upon it
    timeline = alpha.get_timeline()
    return timeline.get_progress()


def main():
    stage_color = clutter.Color(0, 0, 0, 255)
    rect_color = clutter.Color(255, 255, 255, 153)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(stage_color)

    # Add a rectangle to the stage
    global rect
    rect = clutter.Rectangle(rect_color)
    rect.set_size(40, 40)
    rect.set_position(10, 10)
    stage.add(rect)
    rect.show()

    # Show the stage
    stage.connect('destroy', clutter.main_quit)
    stage.show()

    timeline = clutter.Timeline(5000) # milliseconds
    timeline.set_loop(True)
    timeline.start()

    # Create a clutter alpha for the animation
    alpha = clutter.Alpha(timeline)
    alpha.set_func(on_alpha, None)

    # Create an animation to change the properties
    # XXX: There is a bug in pygobject that will refuse to convert between
    # ints and chars. This should be fixed when clutter 1.2 is out and
    # pyclutter targets it. The result is that the opacity won't be animated
    # the bug report is https://bugzilla.gnome.org/show_bug.cgi?id=591800
    # and shall be fixed in the next release, so perhaps by the time you read
    # this is already fixed.
    rect.animate_with_alpha(alpha, 'x', 150.0, 'y', 150.0, 'opacity', 0x0)

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0

if __name__ == '__main__':
    sys.exit(main())
