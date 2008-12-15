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

ClutterActor *entry = NULL;

int main(int argc, char *argv[])
{
  ClutterColor stage_color = { 0x00, 0x00, 0x00, 0xff };
  ClutterColor actor_color = { 0xff, 0xff, 0xcc, 0xff };

  clutter_init (&argc, &argv);

  /* Get the stage and set its size and color: */
  ClutterActor *stage = clutter_stage_get_default ();
  clutter_actor_set_size (stage, 600, 200);
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  /* Add an entry to the stage: */
  entry = clutter_entry_new ();
  clutter_entry_set_color (CLUTTER_ENTRY (entry), &actor_color);
  clutter_entry_set_text (CLUTTER_ENTRY (entry), 
    "Wizard imps and sweat sock pimps, interstellar mongrel nymphs.");
  clutter_entry_set_font_name  (CLUTTER_ENTRY (entry), "Sans 12");
  clutter_actor_set_size (entry, 590, 100);
  clutter_actor_set_position (entry, 5, 5);
  clutter_container_add_actor (CLUTTER_CONTAINER (stage), entry);
  clutter_actor_show (entry);

  /* Set focus to handle key presses on the stage: */
  clutter_stage_set_key_focus (CLUTTER_STAGE (stage), entry);

  /* Show the stage: */
  clutter_actor_show (stage);

  /* Start the main loop, so we can respond to events: */
  clutter_main ();

  return EXIT_SUCCESS;

}
