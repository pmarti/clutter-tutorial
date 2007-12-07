/* Copyright 2007 Openismus GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <clutter/clutter.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor rect_color = { 0xff, 0xff, 0xff, 0x99 };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 800, 600);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add a HBox to the stage: */
  ClutterActor *hbox = clutter_vbox_new ();
  clutter_box_set_default_padding (CLUTTER_BOX (hbox), 10, 0, 10, 0);
  clutter_actor_set_position (hbox, 100, 100);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), hbox);
  clutter_actor_show (hbox);

  /* Add a rectangle to the box: */
  ClutterActor *rect1 = clutter_rectangle_new_with_color (&rect_color);
  clutter_actor_set_size (rect1, 100, 100);
  ClutterPadding rect_padding;
  rect_padding.top = CLUTTER_UNITS_FROM_INT (10);
  rect_padding.bottom = CLUTTER_UNITS_FROM_INT (10);
  rect_padding.left = CLUTTER_UNITS_FROM_INT (10);
  rect_padding.right = CLUTTER_UNITS_FROM_INT (10);
  clutter_box_pack (CLUTTER_BOX (hbox), rect1,
    CLUTTER_PACK_START, &rect_padding);
  clutter_actor_show (rect1);

  /* Add another rectangle to the box: */
  ClutterActor *rect2 = clutter_rectangle_new_with_color (&rect_color);
  clutter_actor_set_size (rect2, 120, 120);
  clutter_box_pack (CLUTTER_BOX (hbox), rect2,
    CLUTTER_PACK_START, &rect_padding);
  clutter_actor_show (rect2);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
