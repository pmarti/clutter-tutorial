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
  ClutterColor actor_color = { 0xff, 0xff, 0xff, 0x99 };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 200, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add a rectangle to the stage: */
  ClutterActor *rect = clutter_rectangle_new_with_color (&actor_color);
  clutter_actor_set_size (rect, 100, 100);
  clutter_actor_set_position (rect, 20, 20);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), rect);
  clutter_actor_show (rect);

  /* Rotate it 20 degrees away from us around the x axis
   * (around its top edge)
   */
  clutter_actor_set_rotation (rect, CLUTTER_X_AXIS, -20, 0, 0, 0);


  /* Add a label to the stage: */
  ClutterActor *label = clutter_label_new_full ("Sans 12", "Some Text", &actor_color);
  clutter_actor_set_size (label, 500, 500); /* TODO: Has no effect yet. See bug #631. */
  clutter_actor_set_position (label, 20, 150);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), label);
  clutter_actor_show (label);

  /* Scale it 300% along the x axis:
   */
  clutter_actor_set_scale (label, 3.00, 1.0);

  /* Move it up and to the right: */
  clutter_actor_move_by (label, 10, -10);

  /* Move it along the z axis, further from the viewer: */
  clutter_actor_set_depth (label, -20);


  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
