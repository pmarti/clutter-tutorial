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

#include <gtk/gtk.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{
  ClutterColor stage_color = { 0x61, 0x64, 0x8c, 0xff };

  /* Call gtk_clutter_init() to init both clutter and gtk+ */
  if (gtk_clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
    g_error ("Unable to initialize GtkClutter");

  if (argc != 2)
    g_error ("Usage: example <image file>");
  
  /* Create a toplevel window: */
  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  /* Create a table to hold the scrollbars and the ClutterEmbed widget: */
  GtkWidget *table = gtk_table_new (2, 2, FALSE);
  gtk_container_add (GTK_CONTAINER (window), table);
  gtk_widget_show (table);

  /* Create ClutterEmbed widget for the stage: */
  GtkWidget *embed = gtk_clutter_embed_new ();
  gtk_table_attach (GTK_TABLE (table), embed,
    0, 1,
    0, 1,
    GTK_EXPAND | GTK_FILL,
    GTK_EXPAND | GTK_FILL,
    0, 0);
  gtk_widget_show (embed);

  /* Init the stage: */
  ClutterActor *stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (embed));
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);
  clutter_actor_set_size (stage, 640, 480);

  /* Createa a viewport actor to be able to scroll actor. By passing NULL it
   * will create new GtkAdjustments. */
  ClutterActor *viewport = gtk_clutter_viewport_new (NULL, NULL);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), viewport);

  /* Load image from first command line argument and add it to viewport: */
  ClutterActor *texture = clutter_texture_new_from_file (argv[1], NULL);
  clutter_container_add_actor (CLUTTER_CONTAINER (viewport), texture);
  clutter_actor_set_position (texture, 0, 0);
  clutter_actor_set_position (texture, 0, 0);
  clutter_actor_set_position (viewport, 0, 0);
  clutter_actor_set_size (viewport, 640, 480);

  /* Create scrollbars and connect them to viewport: */
  GtkAdjustment *h_adjustment = NULL;
  GtkAdjustment *v_adjustment = NULL;
  gtk_clutter_scrollable_get_adjustments (GTK_CLUTTER_SCROLLABLE (viewport),
    &h_adjustment, &v_adjustment);
  GtkWidget *scrollbar = gtk_vscrollbar_new (v_adjustment);
  gtk_table_attach (GTK_TABLE (table), scrollbar,
    1, 2,
    0, 1,
    0, GTK_EXPAND | GTK_FILL,
    0, 0);
  gtk_widget_show (scrollbar);
  
  scrollbar = gtk_hscrollbar_new (h_adjustment);
  gtk_table_attach (GTK_TABLE (table), scrollbar,
    0, 1,
    1, 2,
    GTK_EXPAND | GTK_FILL, 0,
    0, 0);

  gtk_widget_show (scrollbar);
  gtk_widget_show (window);

  gtk_main();

  return EXIT_SUCCESS;
}
