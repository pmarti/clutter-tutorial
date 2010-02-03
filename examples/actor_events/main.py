import sys

import clutter


def on_stage_button_press(stage, event):
    print "Clicked stage at (%f, %f)" % (event.x, event.y)

    # Discover whether there is an actor at that position.
    # Note that you can also connect directly to the actor's signals instead.
    rect = stage.get_actor_at_pos(clutter.PICK_ALL, int(event.x), int(event.y))
    if not rect:
        return False

    if isinstance(rect, clutter.Rectangle):
        print "  A rectangle is at that position."

    return True # Stop further handling of this event.


def on_rect_button_press(stage, event):
    print "Clicked rectangle at (%f, %f)" % (event.x, event.y)

    # clutter.main_quit()

    return True # Stop further handling of this event.


def on_rect_button_release(stage, event):
    print "Click-release on rectangle at (%f, %f)" % (event.x, event.y)

    return True # Stop further handling of this event.


def on_rect_motion(stage, event):
    print "Motion in the rectangle."

    return True # Stop further handling of this event.


def on_rect_enter(stage, event):
    print "Entered rectangle."

    return True # Stop further handling of this event.


def on_rect_leave(stage, event):
    print "Left rectangle."

    return True # Stop further handling of this event.


def main():
    stage_color = clutter.Color(0, 0, 0, 255)
    label_color = clutter.Color(255, 255, 255, 153)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(stage_color)

    # Connect signal handlers to handle mouse clicks on the stage
    stage.connect('button-press-event', on_stage_button_press)

    # Add a rectangle to the stage
    rect = clutter.Rectangle(label_color)
    rect.set_size(100, 100)
    rect.set_position(50, 50)
    rect.show()
    stage.add(rect)

    # Allow the actor to emit events.
    # By default only the stage does this.
    rect.set_reactive(True)

    # Connect signal handlers for events
    rect.connect('button-press-event', on_rect_button_press)
    rect.connect('button-release-event', on_rect_button_release)
    rect.connect('motion-event', on_rect_motion)
    rect.connect('enter-event', on_rect_enter)
    rect.connect('leave-event', on_rect_leave)

    # Show the stage
    stage.connect('destroy', clutter.main_quit)
    stage.show()

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())

