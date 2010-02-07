import sys

import clutter

from triangle import Triangle


def main():
    """"""
    stage_color = clutter.Color(0, 0, 0, 255)
    actor_color = clutter.Color(255, 255, 255, 153)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(stage_color)

    # Add our custom actor to the stage
    actor = Triangle(actor_color)
    actor.set_size(100, 100)
    actor.set_position(20, 20)
    stage.add(actor)
    actor.show()

    # Show the stage
    stage.show()
    stage.connect('destroy', clutter.main_quit)

    #actor.set_color('Red')

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())
