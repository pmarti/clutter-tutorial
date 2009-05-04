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

#include <gtk/gtk.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>
#include <stdlib.h>

ClutterActor *stage = NULL;

static gboolean
on_button_clicked (GtkButton *button, gpointer user_data)
{
  static gboolean already_changed = FALSE;
  if(already_changed)
  {
    ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff }; /* Black */
    clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);
  }
  else
  {
    ClutterColor stage_color = { 0x20, 0x20, 0xA0, 0xff };
    clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);
  }

  already_changed = !already_changed;

  return TRUE; /* Stop further handling of this event. */
}

static gboolean
on_stage_button_press (ClutterStage *stage, ClutterEvent *event, gpointer user_data)
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

  gtk_clutter_init (&argc, &argv);

  /* Create the window and some child widgets: */
  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  GtkWidget *vbox = gtk_vbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);
  GtkWidget *button = gtk_button_new_with_label ("Change Color");
  gtk_box_pack_end (GTK_BOX (vbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);
  g_signal_connect (button, "clicked",
    G_CALLBACK (on_button_clicked), NULL);

  /* Stop the application when the window is closed: */
  g_signal_connect (window, "hide",
                    G_CALLBACK (gtk_main_quit), NULL);

  /* Create the clutter widget: */
  GtkWidget *clutter_widget = gtk_clutter_embed_new ();
  gtk_box_pack_start (GTK_BOX (vbox), clutter_widget, TRUE, TRUE, 0);
  gtk_widget_show (clutter_widget);

  /* Set the size of the widget, 
   * because we should not set the size of its stage when using GtkClutterEmbed.
   */ 
  gtk_widget_set_size_request (clutter_widget, 200, 200);

  /* Get the stage and set its size and color: */
  stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter_widget));
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Connect a signal handler to handle mouse clicks and key presses on the stage: */ 
  g_signal_connect (stage, "button-press-event",
    G_CALLBACK (on_stage_button_press), NULL);

  /* Show the window: */
  gtk_widget_show (GTK_WIDGET (window));

  /* Start the main loop, so we can respond to events: */
  gtk_main ();

  return EXIT_SUCCESS;

}
