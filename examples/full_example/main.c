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

ClutterActor *stage = NULL;
ClutterTimeline *timeline = NULL;

static void
on_texture_button_press (ClutterActor *actor, ClutterEvent *event, gpointer data);

const double angle_step = 30;

typedef struct Item
{
  ClutterActor *actor;
  ClutterBehaviour *ellipse_behaviour;
}
Item;

GSList *list_items = 0;

void on_foreach_clear_list_items(gpointer data, gpointer user_data)
{
  Item *item = (Item*)data;
  clutter_actor_destroy (item->actor);
  g_object_unref (item->ellipse_behaviour);
  g_free (item);
}

void load_images(const gchar* directory_path)
{
  g_return_if_fail(directory_path);

  /* Clear any existing images: */
  g_slist_foreach (list_items, on_foreach_clear_list_items, NULL);
  g_slist_free (list_items);

  /* Create a new list: */
  list_items = NULL;
  
  /* Discover the images in the directory: */
  GError *error = NULL;
  GDir* dir = g_dir_open (directory_path, 0, &error);
  if(error)
  {
    g_warning("g_dir_open() failed: %s\n", error->message);
    g_clear_error(&error);
    return;
  }

  const gchar* filename = NULL;
  while ( (filename = g_dir_read_name(dir)) )
  {
    gchar* path = g_build_filename (directory_path, filename, NULL);

    /* Try to load the file as an image: */
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(path, NULL);
    if(pixbuf)
    {
      Item* item = g_new0(Item, 1);

      /* Add an actor to show this image: */
      item->actor = clutter_texture_new_from_pixbuf (pixbuf);

      /* Make sure that all images are shown with the same height: */
      const int pixbuf_height = gdk_pixbuf_get_height(pixbuf);
      const gdouble scale = pixbuf_height ? 100 /  (gdouble)pixbuf_height : 0;
      clutter_actor_set_scale (item->actor, scale, scale);

      list_items = g_slist_append (list_items, item);
    }

    if(pixbuf)
      g_object_unref (pixbuf);

    g_free (path);
  }
}


void add_to_ellipse_behaviour(ClutterTimeline *timeline, gdouble start_angle, Item *item)
{
  g_return_if_fail (timeline);

  ClutterAlpha *alpha = clutter_alpha_new_full (timeline, CLUTTER_ALPHA_SINE_INC, NULL, NULL);
 
  item->ellipse_behaviour = clutter_behaviour_ellipse_new (alpha, 
    400, 300, /* x, y */
    450, 450, /* width, height */
    CLUTTER_ROTATE_CW,
    start_angle, start_angle + 360);
  clutter_behaviour_ellipse_set_angle_tilt (CLUTTER_BEHAVIOUR_ELLIPSE (item->ellipse_behaviour), 
    CLUTTER_X_AXIS, 45);
  g_object_unref (alpha);

  clutter_behaviour_apply (item->ellipse_behaviour, item->actor);
}

void add_image_actors()
{
  int x = 20;
  int y = 0;
  gdouble angle = 0;
  GSList *list = list_items;
  while (list)
  {
    /* Add the actor to the stage: */
    Item *item = (Item*)list->data;
    ClutterActor *actor = item->actor;
    clutter_container_add_actor (CLUTTER_CONTAINER (stage), actor);

    /* Set an initial position: */
    clutter_actor_set_position (actor, x, y);
    y += 100;

    /* Allow the actor to emit events.
     * By default only the stage does this.
     */
    clutter_actor_set_reactive (actor, TRUE);

    /* Connect signal handlers for events: */
    g_signal_connect (actor, "button-press-event",
      G_CALLBACK (on_texture_button_press), item);

    add_to_ellipse_behaviour (timeline, angle, item);
    angle += angle_step;

    clutter_actor_show (actor);

    list = g_slist_next (list);
  }
}

void rotate_all_until_item_is_at_front(Item *item)
{
  g_return_if_fail (item);

  clutter_timeline_stop(timeline);

  /* Get the item's position in the list: */
  GSList *this_list_item = g_slist_find (list_items, item);
  g_return_if_fail (this_list_item);
  const gint pos = g_slist_position (list_items, this_list_item);
  printf("debug: pos=%d\n", pos); 
  
  /* Calculate the end angle of the first item: */
  gdouble angle_end = 180 - (angle_step * pos);

  /* Set the end angles: */
  GSList *list = list_items;
  while (list)
  {
    Item *this_item = (Item*)list->data;
    clutter_behaviour_ellipse_set_angle_end (
      CLUTTER_BEHAVIOUR_ELLIPSE(this_item->ellipse_behaviour), angle_end);
    printf("debug: angle_end = %f\n", angle_end); 

    /* TODO: This isn't useful, but it's useful for debugging to show what was selected.
    /* Make all items transparent except the selected one: */
    if (this_item != item)
      clutter_actor_set_opacity (this_item->actor, 40);
    else
    {
      clutter_actor_set_opacity (this_item->actor, 255);
      printf("  debug: angle_end for selected item= %f\n", angle_end); 

    }

    angle_end += angle_step;
    list = g_slist_next (list);
  }

  /* clutter_timeline_set_loop (timeline, TRUE); */
  clutter_timeline_start(timeline);
}

static void
on_texture_button_press (ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
  Item *item = (Item*)user_data;
  rotate_all_until_item_is_at_front (item);
}


int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 800, 600);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Allow clutter to emit motion-event, enter-event, and leave-event.
   * By default this is disabled, to improve performance:
   */
  clutter_set_motion_events_enabled(TRUE);

  /* Show the stage: */
  clutter_actor_show (stage);

  timeline = clutter_timeline_new(100 /* frames */, 30 /* frames per second. */);

  /* Add an actor for each image: */
  load_images ("./images/");
  add_image_actors ();

  /* clutter_timeline_set_loop(timeline, TRUE); */

  /* Move them a bit to start with: */
  clutter_timeline_start (timeline);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
