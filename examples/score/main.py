import sys

import clutter


rect = None
rotation_angle = 0
color_change_count = 0

# Rotate the rectangle and alternate its color.
def on_timeline_rotation_new_frame(timeline, frame_num):
    global rotation_angle
    rotation_angle += 1
    if rotation_angle >= 360:
        rotation_angle = 0

    # Rotate the rectangle clockwise around the z axis, around it's
    # top-left corner
    rect.set_rotation(clutter.X_AXIS, rotation_angle, 0, 0, 0)

    # Change the color
    # (This is a silly example, making the rectangle flash)
    global color_change_count
    color_change_count += 1
    if color_change_count > 100:
        color_change_count = 0
    if color_change_count == 0:
        rect_color = clutter.Color(255, 255, 255, 153)
        rect.set_color(rect_color)
    elif color_change_count == 50:
        rect_color = clutter.Color(16, 64, 144, 255)
        rect.set_color(rect_color)


# Move the rectangle.
def on_timeline_move_new_frame(timeline, frame_num):
    global rect
    x_position = rect.get_x()
    x_position += 1

    if x_position >= 150:
        x_position = 0

    rect.set_x(x_position)


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
    rect.set_size(70, 70)
    rect.set_position(50, 100)
    stage.add(rect)
    rect.show()

    # Show the stage
    stage.connect('destroy', clutter.main_quit)
    stage.show()

    # Create a score and add two timelines to it,
    # so the second timeline starts when the first one stops
    score = clutter.Score()
    score.set_loop(True)

    timeline_rotation = clutter.Timeline(5000) # milliseconds
    timeline_rotation.connect('new-frame', on_timeline_rotation_new_frame)
    score.append(timeline=timeline_rotation)

    timeline_move = clutter.Timeline(5000) # milliseconds
    timeline_move.connect('new-frame', on_timeline_move_new_frame)
    score.append(parent=timeline_rotation, timeline=timeline_move)

    score.start()

    # Start the main loop, so we can respond to events
    clutter.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())
