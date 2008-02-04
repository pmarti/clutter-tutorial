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

static gboolean
on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer data)
{
  gint x = 0;
  gint y = 0;
  clutter_event_get_coords (event, &x, &y);

  g_print ("Stage clicked at (%d, %d)\n", x, y);

  return TRUE; /* Stop further handling of this event. */
}

int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff }; /* Black */

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 200, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Connect a signal handler to handle mouse clicks and key presses on the stage: */ 
  g_signal_connect (stage, "button-press-event",
    G_CALLBACK (on_stage_button_press), NULL);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
