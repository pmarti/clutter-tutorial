/* Copyright 2008 Openismus GmbH
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
#include "examplebox.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor actor_color = { 0xff, 0xff, 0xff, 0x99 };
  ClutterColor actor_color2 = { 0x10, 0x40, 0x90, 0xff };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 200, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add our custom container to the stage: */
  ClutterActor *box = example_box_new ();

  /* Set the size to the preferred size of the container: */
  clutter_actor_set_size (box, -1, -1);
  clutter_actor_set_position (box, 20, 20);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), box);
  clutter_actor_show (box);

  /* Add some actors to our container: */
  ClutterActor *actor = clutter_rectangle_new_with_color (&actor_color);
  clutter_actor_set_size (actor, 75, 75);
  clutter_container_add_actor (CLUTTER_CONTAINER (box), actor);
  clutter_actor_show (actor);

  ClutterActor *actor2 = clutter_rectangle_new_with_color (&actor_color2);
  clutter_actor_set_size (actor2, 75, 75);
  clutter_container_add_actor (CLUTTER_CONTAINER (box), actor2);
  clutter_actor_show (actor2);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
