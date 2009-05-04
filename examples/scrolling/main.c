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
#include "scrollingcontainer.h"
#include <stdlib.h>

ClutterActor *scrolling = NULL;


static gboolean
on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer data)
{
  printf ("Scrolling\n");

  /* Scroll the container: */
  example_scrolling_container_scroll_left (
    EXAMPLE_SCROLLING_CONTAINER (scrolling), 10);

  return TRUE; /* Stop further handling of this event. */
}

int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor actor_color = { 0x7f, 0xae, 0xff, 0xff };
  ClutterColor actor_color2 = { 0xff, 0x7f, 0xae, 0xff };
  ClutterColor actor_color3 = { 0xae, 0xff, 0x7f, 0xff };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 200, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add our scrolling container to the stage: */
  scrolling = example_scrolling_container_new ();
  clutter_actor_set_size (scrolling, 180, 100);
  clutter_actor_set_position (scrolling, 10, 10);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), scrolling);
  clutter_actor_show (scrolling);

 /* Add some actors to our container: */
  ClutterActor *actor = clutter_rectangle_new_with_color (&actor_color);
  clutter_actor_set_size (actor, 75, 75);
  clutter_container_add_actor (CLUTTER_CONTAINER (scrolling), actor);
  clutter_actor_show (actor);

  ClutterActor *actor2 = clutter_rectangle_new_with_color (&actor_color2);
  clutter_actor_set_size (actor2, 75, 75);
  clutter_container_add_actor (CLUTTER_CONTAINER (scrolling), actor2);
  clutter_actor_show (actor2);

  ClutterActor *actor3 = clutter_rectangle_new_with_color (&actor_color3);
  clutter_actor_set_size (actor3, 75, 75);
  clutter_container_add_actor (CLUTTER_CONTAINER (scrolling), actor3);
  clutter_actor_show (actor3);


  /* Show the stage: */
  clutter_actor_show (stage);

  /* Connect signal handlers to handle mouse clicks on the stage: */ 
  g_signal_connect (stage, "button-press-event",
    G_CALLBACK (on_stage_button_press), NULL);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
