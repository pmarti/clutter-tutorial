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
#include "multiline_entry.h"
#include <stdlib.h>

ClutterActor *multiline = NULL;

static gboolean
on_stage_key_press (ClutterStage *stage, ClutterKeyEvent *event, gpointer data)
{
  /* TODO: Ideally the entry would handle this itself. */
  example_multiline_entry_handle_key_event (
    EXAMPLE_MULTILINE_ENTRY (multiline), event);

  return TRUE; /* Stop further handling of this event. */
}

int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor actor_color = { 0xae, 0xff, 0x7f, 0xff };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 400, 400);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add our multiline entry to the stage: */
  multiline = example_multiline_entry_new ();
  example_multiline_entry_set_text(EXAMPLE_MULTILINE_ENTRY(multiline), 
     "And as I sat there brooding on the old, unknown world, I thought of "
     "Gatsby's wonder when he first picked out the green light at the end of "
     "Daisy's dock. He had come a long way to this blue lawn and his dream "
     "must have seemed so close that he could hardly fail to grasp it. He did "
     "not know that it was already behind him, somewhere back in that vast "
     "obscurity beyond the city, where the dark fields of the republic rolled "
     "on under the night.");
  example_multiline_entry_set_color(EXAMPLE_MULTILINE_ENTRY(multiline), 
     &actor_color);
  clutter_actor_set_size (multiline, 380, 380);
  clutter_actor_set_position (multiline, 10, 10);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), multiline);
  clutter_actor_show (multiline);

  /* Connect signal handlers to handle key presses on the stage: */ 
  g_signal_connect (stage, "key-press-event",
    G_CALLBACK (on_stage_key_press), NULL);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();


  return EXIT_SUCCESS;

}
