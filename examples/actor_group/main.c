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

  /* Add a group to the stage: */
  ClutterActor *group = clutter_group_new ();
  clutter_actor_set_position (group, 40, 40);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), group);
  clutter_actor_show (group);
  
  /* Add a rectangle to the group: */
  ClutterActor *rect = clutter_rectangle_new_with_color (&actor_color);
  clutter_actor_set_size (rect, 50, 50);
  clutter_actor_set_position (rect, 0, 0);
  clutter_container_add_actor (CLUTTER_CONTAINER (group), rect);
  clutter_actor_show (rect);

  /* Add a label to the group: */
  ClutterActor *label = clutter_text_new_full ("Sans 9", "Some Text", &actor_color);
  clutter_actor_set_position (label, 0, 60);
  clutter_container_add_actor (CLUTTER_CONTAINER (group), label);
  clutter_actor_show (label);

  /* Scale the group 120% along the x axis:
   */
  clutter_actor_set_scale (group, 3.00, 1.0);

  /* Rotate it around the z axis: */
  clutter_actor_set_rotation (group, CLUTTER_Z_AXIS, 10, 0, 0, 0);


  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
