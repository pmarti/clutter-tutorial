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

ClutterActor *rect = NULL;

gint rotation_angle = 0;
gint color_change_count = 0;

void
on_timeline_new_frame (ClutterTimeline *timeline, 
  gint frame_num, gpointer data)
{
  rotation_angle += 1;
  if(rotation_angle >= 360)
    rotation_angle = 0;

  /* Rotate the rectangle clockwise around the z axis, around it's top-left corner: */
  clutter_actor_set_rotation (rect, CLUTTER_X_AXIS, 
    rotation_angle, 0, 0, 0);

  /* Change the color
   * (This is a silly example, making the rectangle flash):
   */
  ++color_change_count;
  if(color_change_count > 100)
    color_change_count = 0;

  if(color_change_count == 0)
  {
    ClutterColor rect_color = { 0xff, 0xff, 0xff, 0x99 };
    clutter_rectangle_set_color (CLUTTER_RECTANGLE (rect), &rect_color);
  }
  else if (color_change_count == 50)
  {
    ClutterColor rect_color = { 0x10, 0x40, 0x90, 0xff };
    clutter_rectangle_set_color (CLUTTER_RECTANGLE (rect), &rect_color);
  }
}

void
on_timeline_marker_reached (ClutterTimeline* timeline,
                            gchar*           marker_name,
                            gint             frame_num,
                            gpointer         user_data)
{
  printf ("Reached marker %s at frame %d.\n",
    marker_name, frame_num);
}

int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor rect_color = { 0xff, 0xff, 0xff, 0x99 };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 200, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add a rectangle to the stage: */
  rect = clutter_rectangle_new_with_color (&rect_color);
  clutter_actor_set_size (rect, 70, 70);
  clutter_actor_set_position (rect, 50, 100);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), rect);
  clutter_actor_show (rect);

  /* Show the stage: */
  clutter_actor_show (stage);

  ClutterTimeline *timeline = clutter_timeline_new(10 /* frames */, 120 /* frames per second. */);
  clutter_timeline_add_marker_at_frame (timeline, "clutter-tutorial", 5);
  g_signal_connect (timeline, "new-frame", G_CALLBACK (on_timeline_new_frame), NULL);
  g_signal_connect (timeline, "marker-reached", G_CALLBACK (on_timeline_marker_reached), NULL);
  clutter_timeline_set_loop(timeline, TRUE); 
  clutter_timeline_start(timeline);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  g_object_unref (timeline);

  return EXIT_SUCCESS;

}
