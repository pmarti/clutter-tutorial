import sys

import clutter


def main():
    stage_color = clutter.Color(0, 0, 0, 255)
    actor_color = clutter.Color(255, 255, 255, 153)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(stage_color)

    # Add a rectangle to the stage
    rect = clutter.Rectangle(actor_color)
    rect.set_size(100, 100)
    rect.set_position(20, 20)
    stage.add(rect)
    rect.show()

    # Add a label to the stage
    label = clutter.Text("Sans 12", "Some Text", actor_color)
    label.set_size(500, 500)
    label.set_position(20, 150)
    stage.add(label)
    label.show()

    # Show the stage
    stage.connect("destroy", clutter.main_quit)
    stage.show_all()

    # Start the main loop, so we can respond to events:
    clutter.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())
