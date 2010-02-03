import sys

import clutter

def on_stage_button_press(stage, event):
    print "Stage clicked at (%f, %f)" % (event.x, event.y)


def main():
    stage = clutter.Stage()

    stage = clutter.Stage()
    stage.set_size(200, 200)
    stage.set_color(clutter.Color(0, 0, 0, 255))

    stage.connect('button-press-event', on_stage_button_press)
    stage.connect("destroy", clutter.main_quit)
    stage.show()

    clutter.main()

    return 0

if __name__ == '__main__':
    sys.exit(main())
