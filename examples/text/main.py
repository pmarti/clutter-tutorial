import sys

import clutter


def main():
    stage_color = clutter.Color(0, 0, 0, 255)
    actor_color = clutter.Color(255, 255, 204, 255)

    # Get the stage and set its size and color
    stage = clutter.Stage()
    stage.set_size(800, 200)
    stage.set_color(stage_color)

    # Add a non-editable text actor to the stage
    text = clutter.Text()

    # Setup text properties
    text.set_color(actor_color)
    text.set_text("Non-editable text: Wizard imps and sweat sock pimps,"
                  "interstellar mongrel nymphs.")
    text.set_font_name("Sans 12")
    text.set_editable(False)
    text.set_line_wrap(False)

    # Discover the preferred height and use that height
    min_height, natural_height = text.get_preferred_height(750)
    text.set_size(750, natural_height)
    text.set_position(5, 5)
    stage.add(text)
    text.show()

    # Add a multi-line editable text actor to the stage
    text = clutter.Text()

    # Setup text properties
    text.set_color(actor_color)
    text.set_text(
        "Editable text: And as I sat there brooding on the old, unknown world, I thought of "
        "Gatsby's wonder when he first picked out the green light at the end of "
        "Daisy's dock. He had come a long way to this blue lawn and his dream "
        "must have seemed so close that he could hardly fail to grasp it. He did "
        "not know that it was already behind him, somewhere back in that vast "
        "obscurity beyond the city, where the dark fields of the republic rolled "
        "on under the night.")
    text.set_font_name("Sans 12")
    text.set_editable(True)
    text.set_line_wrap(True)

    # Discover the preferred height and use that height
    min_height, natural_height = text.get_preferred_height(750)
    text.set_size(750, natural_height)

    text.set_position(5, 50)
    stage.add(text)
    text.show()

    # Set focus to handle key presses on the stage
    stage.set_key_focus(text)

    # Show the stage
    stage.show_all()
    stage.connect('destroy', clutter.main_quit)

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0

if __name__ == '__main__':
    sys.exit(main())
