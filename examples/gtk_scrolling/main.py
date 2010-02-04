import sys

import cluttergtk   # must be the first to be imported
import clutter
import gtk


def main():
    stage_color = clutter.Color(97, 100, 140, 255)

    if len(sys.argv) != 2:
        raise SystemExit("Usage: python main.py <image file>")

    # Create a toplevel window
    window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    window.set_default_size(640, 480)
    window.connect('destroy', gtk.main_quit)

    # Create a table to hold the scrollbars and the clutter.Embed widget
    table = gtk.Table(2, 2, False)
    window.add(table)
    table.show()

    # Create clutter.Embed widget for the stage
    embed = cluttergtk.Embed()
    table.attach(embed,
            0, 1,
            0, 1,
            gtk.EXPAND | gtk.FILL,
            gtk.EXPAND | gtk.FILL,
            0, 0)
    embed.show()

    #  Init the stage
    stage = embed.get_stage()
    stage.set_color(stage_color)
    stage.set_size(640, 480)

    # Create a viewport actor to be able to scroll actor. By passing NULL it
    # will create new GtkAdjustments
    viewport = cluttergtk.Viewport()
    stage.add(viewport)

    # Load image from first command line argument and add it to viewport
    texture = clutter.Texture(sys.argv[1])
    viewport.add(texture)
    texture.set_position(0, 0)
    viewport.set_size(640, 480)

    # Create scrollbars and connect them to viewport
    h_adjustment, v_adjustment = viewport.get_adjustments()
    scrollbar = gtk.VScrollbar(v_adjustment)
    table.attach(scrollbar,
        1, 2,
        0, 1,
        0, gtk.EXPAND | gtk.FILL,
        0, 0)
    scrollbar.show()

    scrollbar = gtk.HScrollbar(h_adjustment)
    table.attach(scrollbar,
        0, 1,
        1, 2,
        gtk.EXPAND | gtk.FILL, 0,
        0, 0)
    scrollbar.show()

    window.show()

    gtk.main()

    return 0


if __name__ == '__main__':
    sys.exit(main())
