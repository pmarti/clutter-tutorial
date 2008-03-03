/* Copyright 2008 Openismus GmbH,
 * based on ClutterEntry in Clutter, Copyright OpenedHand
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

#ifndef _HAVE_EXAMPLE_MULTILINE_ENTRY_H
#define _HAVE_EXAMPLE_MULTILINE_ENTRY_H

#include <clutter/clutter-actor.h>
#include <clutter/clutter-color.h>
#include <clutter/clutter-event.h>
#include <pango/pango.h>


G_BEGIN_DECLS

#define CLUTTER_TYPE_MULTILINE_ENTRY (example_multiline_entry_get_type ())

#define EXAMPLE_MULTILINE_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CLUTTER_TYPE_MULTILINE_ENTRY, ExampleMultilineEntry))

#define EXAMPLE_MULTILINE_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CLUTTER_TYPE_MULTILINE_ENTRY, ExampleMultilineEntryClass))

#define EXAMPLE_IS_MULTILINE_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CLUTTER_TYPE_MULTILINE_ENTRY))

#define EXAMPLE_IS_MULTILINE_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CLUTTER_TYPE_MULTILINE_ENTRY))

#define EXAMPLE_MULTILINE_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CLUTTER_TYPE_MULTILINE_ENTRY, ExampleMultilineEntryClass))

typedef struct _ExampleMultilineEntry ExampleMultilineEntry;
typedef struct _ExampleMultilineEntryClass ExampleMultilineEntryClass;
typedef struct _ExampleMultilineEntryPrivate ExampleMultilineEntryPrivate;

struct _ExampleMultilineEntry
{
  /*< private >*/
  ClutterActor parent_instance;

  ExampleMultilineEntryPrivate   *priv;
};

/**
 * ExampleMultilineEntryClass:
 * @paint_cursor: virtual function for subclasses to use to draw a custom
 *   cursor instead of the default one
 * @text_changed: signal class handler for ExampleMultilineEntry::text-changed
 * @cursor_event: signal class handler for ExampleMultilineEntry::cursor-event
 * @activate: signal class handler for ExampleMultilineEntry::activate
 */
struct _ExampleMultilineEntryClass 
{
  /*< private >*/
  ClutterActorClass parent_class;
  
  /*< public >*/
  /* vfuncs, not signals */
  void (* paint_cursor) (ExampleMultilineEntry    *entry);
  
  /* signals */
  void (* text_changed) (ExampleMultilineEntry    *entry);
  void (* cursor_event) (ExampleMultilineEntry    *entry,
                         ClutterGeometry *geometry);
  void (* activate)     (ExampleMultilineEntry    *entry);
}; 

GType example_multiline_entry_get_type (void) G_GNUC_CONST;

ClutterActor *        example_multiline_entry_new                 (void);
void                  example_multiline_entry_set_text            (ExampleMultilineEntry       *entry,
						         const gchar        *text);
G_CONST_RETURN gchar *example_multiline_entry_get_text            (ExampleMultilineEntry       *entry);
void                  example_multiline_entry_set_font_name       (ExampleMultilineEntry       *entry,
						         const gchar        *font_name);
G_CONST_RETURN gchar *example_multiline_entry_get_font_name       (ExampleMultilineEntry       *entry);
void                  example_multiline_entry_set_color           (ExampleMultilineEntry       *entry,
						         const ClutterColor *color);
void                  example_multiline_entry_get_color           (ExampleMultilineEntry       *entry,
						         ClutterColor       *color);

void                  example_multiline_entry_handle_key_event    (ExampleMultilineEntry       *entry,
                                                         ClutterKeyEvent    *kev);

G_END_DECLS

#endif /* _HAVE_EXAMPLE_MULTILINE_ENTRY_H */
