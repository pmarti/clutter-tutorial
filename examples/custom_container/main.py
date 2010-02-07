import sys

import clutter

from examplebox import ExampleBox


def main():
    stage_color = clutter.Color(0, 0, 0, 255)
    actor_color = clutter.Color(255, 255, 255, 153)
    actor_color2 = clutter.Color(16, 64, 144, 255)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(stage_color)

    # Add our custom container to the stage
    box = ExampleBox()

    # Set the size to the preferred size of the container
    box.set_size(-1, -1)
    box.set_position(20, 20)
    stage.add(box)
    box.show()

    # Add some actors to our container
    actor = clutter.Rectangle(color=actor_color)
    actor.set_size(75, 75)
    box.add(actor)
    actor.show()

    actor2 = clutter.Rectangle(color=actor_color2)
    actor2.set_size(75, 75)
    box.add(actor2)
    actor2.show()

    # Show the stage
    stage.show()
    stage.connect('destroy', clutter.main_quit)

    box.do_remove_all()

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())

