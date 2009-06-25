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
  gfloat x = 0;
  gfloat y = 0;
  clutter_event_get_coords (event, &x, &y);

  g_print ("Clicked stage at (%f, %f)\n", x, y);

  /* Discover whether there is an actor at that position.
   * Note that you can also connect directly to the actor's signals instead.
   */
  ClutterActor *rect = clutter_stage_get_actor_at_pos (stage, CLUTTER_PICK_ALL, x, y);
  if (!rect)
    return FALSE;

  if (CLUTTER_IS_RECTANGLE (rect))
    g_print ("  A rectangle is at that position.\n");

  return TRUE; /* Stop further handling of this event. */
}

static gboolean
on_rect_button_press (ClutterRectangle *rect, ClutterEvent *event, gpointer data)
{
  gfloat x = 0;
  gfloat y = 0;
  clutter_event_get_coords (event, &x, &y);

  g_print ("Clicked rectangle at (%f, %f)\n", x, y);

  /* clutter_main_quit(); */

  return TRUE; /* Stop further handling of this event. */
}

static gboolean
on_rect_button_release (ClutterRectangle *rect, ClutterEvent *event, gpointer data)
{
  gfloat x = 0;
  gfloat y = 0;
  clutter_event_get_coords (event, &x, &y);

  g_print ("Click-release on rectangle at (%f, %f)\n", x, y);

  return TRUE; /* Stop further handling of this event. */
}

static gboolean
on_rect_motion (ClutterRectangle *rect, ClutterEvent *event, gpointer data)
{
  g_print ("Motion in the rectangle.\n");

  return TRUE; /* Stop further handling of this event. */
}

static gboolean
on_rect_enter (ClutterRectangle *rect, ClutterEvent *event, gpointer data)
{
  g_print ("Entered rectangle.\n");

  return TRUE; /* Stop further handling of this event. */
}

static gboolean
on_rect_leave (ClutterRectangle *rect, ClutterEvent *event, gpointer data)
{
  g_print ("Left rectangle.\n");

  return TRUE; /* Stop further handling of this event. */
}



int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor label_color = { 0xff, 0xff, 0xff, 0x99 };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 200, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Connect signal handlers to handle mouse clicks on the stage: */ 
  g_signal_connect (stage, "button-press-event",
    G_CALLBACK (on_stage_button_press), NULL);

  /* Add a Rectangle to the stage: */
  ClutterActor *rect = clutter_rectangle_new_with_color (&label_color);
  clutter_actor_set_size (rect, 100, 100);
  clutter_actor_set_position (rect, 50, 50);
  clutter_actor_show (rect);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), rect);

  /* Allow the actor to emit events.
   * By default only the stage does this.
   */
  clutter_actor_set_reactive (rect, TRUE);

  /* Connect signal handlers for events: */
  g_signal_connect (rect, "button-press-event",
    G_CALLBACK (on_rect_button_press), NULL);
  g_signal_connect (rect, "button-release-event",
    G_CALLBACK (on_rect_button_release), NULL);
  g_signal_connect (rect, "motion-event",
    G_CALLBACK (on_rect_motion), NULL);
  g_signal_connect (rect, "enter-event",
    G_CALLBACK (on_rect_enter), NULL);
  g_signal_connect (rect, "leave-event",
    G_CALLBACK (on_rect_leave), NULL);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
