import sys

import clutter


def main():
    stage_color = clutter.Color(0, 0, 0, 255)
    actor_color = clutter.Color(255, 255, 255, 153)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(stage_color)

    # Add a group to the stage
    group = clutter.Group()
    group.set_position(40, 40)
    stage.add(group)
    group.show()

    # Add a rectangle to the group
    rect = clutter.Rectangle(actor_color)
    rect.set_size(50, 50)
    rect.set_position(0, 0)
    group.add(rect)
    rect.show()

    # Add a label to the group
    label = clutter.Text("Sans 9", "Some Text", actor_color)
    label.set_position(0, 60)
    group.add(label)
    label.show()

    # Scale the group 120% along the x axis
    group.set_scale(3.0, 1.0)

    # Rotate it around the z axis
    group.set_rotation(clutter.Z_AXIS, 10, 0, 0, 0)

    # Show the stage
    stage.connect("destroy", clutter.main_quit)
    stage.show_all()

    # Start the main loop, so we can respond to events
    clutter.main()


if __name__ == '__main__':
    sys.exit(main())
