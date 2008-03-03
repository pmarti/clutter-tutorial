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

/**
 * SECTION:clutter-entry
 * @short_description: Example of a multi-line text entry actor, 
 * based on ClutterEntry.
 */

#include "multiline_entry.h"

#include <clutter/clutter-enum-types.h>
#include <clutter/clutter-keysyms.h>
#include <clutter/clutter-main.h>
#include <clutter/clutter-rectangle.h>
#include <clutter/clutter-units.h>
#include <clutter/pangoclutter.h>
#include <clutter/clutter-backend.h>
#include <string.h>

#define DEFAULT_FONT_NAME	"Sans 10"
#define ENTRY_CURSOR_WIDTH      1

G_DEFINE_TYPE (ExampleMultilineEntry, example_multiline_entry, CLUTTER_TYPE_ACTOR);

/* For the font map: */
static PangoClutterFontMap  *_font_map = NULL;
static PangoContext         *_context  = NULL;

enum
{
  PROP_0,

  PROP_FONT_NAME,
  PROP_TEXT,
  PROP_COLOR
};

enum
{
  TEXT_CHANGED,
  CURSOR_EVENT,

  LAST_SIGNAL
};

static guint entry_signals[LAST_SIGNAL] = { 0, };

#define EXAMPLE_MULTILINE_ENTRY_GET_PRIVATE(obj) \
(G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_MULTILINE_ENTRY, ExampleMultilineEntryPrivate))

static void
example_multiline_entry_delete_chars (ExampleMultilineEntry *entry, guint num);

static void
example_multiline_entry_insert_unichar (ExampleMultilineEntry *entry, gunichar wc);

static void
example_multiline_entry_delete_text (ExampleMultilineEntry *entry, gssize start_pos, gssize end_pos);

struct _ExampleMultilineEntryPrivate
{
  PangoContext         *context;
  PangoFontDescription *desc;

  ClutterColor          fgcol;

  gchar                *text;
  gchar                *font_name;

  gint                  width;
  gint                  n_chars;

  gint                  position;
  gint                  text_x;

  PangoAttrList        *effective_attrs;
  PangoLayout          *layout;

  ClutterGeometry       cursor_pos;
  ClutterActor         *cursor;
};


static void
example_multiline_entry_set_property (GObject      *object,
			    guint         prop_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
  ExampleMultilineEntry        *entry;
  ExampleMultilineEntryPrivate *priv;

  entry = EXAMPLE_MULTILINE_ENTRY (object);
  priv = entry->priv;

  switch (prop_id)
    {
    case PROP_FONT_NAME:
      example_multiline_entry_set_font_name (entry, g_value_get_string (value));
      break;
    case PROP_TEXT:
      example_multiline_entry_set_text (entry, g_value_get_string (value));
      break;
    case PROP_COLOR:
      example_multiline_entry_set_color (entry, g_value_get_boxed (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
example_multiline_entry_get_property (GObject    *object,
			    guint       prop_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
  ExampleMultilineEntry        *entry;
  ExampleMultilineEntryPrivate *priv;
  ClutterColor         color;

  entry = EXAMPLE_MULTILINE_ENTRY(object);
  priv = entry->priv;

  switch (prop_id)
    {
    case PROP_FONT_NAME:
      g_value_set_string (value, priv->font_name);
      break;
    case PROP_TEXT:
      g_value_set_string (value, priv->text);
      break;
    case PROP_COLOR:
      example_multiline_entry_get_color (entry, &color);
      g_value_set_boxed (value, &color);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
example_multiline_entry_ensure_layout (ExampleMultilineEntry *entry, gint width)
{
  ExampleMultilineEntryPrivate  *priv;

  priv   = entry->priv;

  if (!priv->layout)
    {
      priv->layout = pango_layout_new (_context);

      if (priv->effective_attrs)
	pango_layout_set_attributes (priv->layout, priv->effective_attrs);

      pango_layout_set_single_paragraph_mode (priv->layout,
					      FALSE );

      pango_layout_set_font_description (priv->layout, priv->desc);

      pango_layout_set_text (priv->layout, priv->text, priv->n_chars);

      pango_layout_set_wrap  (priv->layout, PANGO_WRAP_WORD);

      if (width > 0)
	pango_layout_set_width (priv->layout, width * PANGO_SCALE);
      else
	pango_layout_set_width (priv->layout, -1);
    }
}

static void
example_multiline_entry_clear_layout (ExampleMultilineEntry *entry)
{
  if (entry->priv->layout)
    {
      g_object_unref (entry->priv->layout);
      entry->priv->layout = NULL;
    }
}

static gint
offset_to_bytes (const gchar *text, gint pos)
{
  gchar *c = NULL;
  gint i, j, len;

  if (pos < 1)
    return pos;

  c = g_utf8_next_char (text);
  j = 1;
  len = strlen (text);

  for (i = 0; i < len; i++)
    {
      if (&text[i] == c)
      {
        if (j == pos)
          break;
        else
          {
            c = g_utf8_next_char (c);
            j++;
          }
      }
    }
  return i;
}


static void
example_multiline_entry_ensure_cursor_position (ExampleMultilineEntry *entry)
{
  ExampleMultilineEntryPrivate  *priv;
  gint                  index_;
  PangoRectangle        rect;

  priv = entry->priv;

  if (priv->position == -1)
    index_ = strlen (priv->text);
  else
    index_ = offset_to_bytes (priv->text, priv->position);

  pango_layout_get_cursor_pos (priv->layout, index_, &rect, NULL);
  priv->cursor_pos.x = rect.x / PANGO_SCALE;
  priv->cursor_pos.y = rect.y / PANGO_SCALE;
  priv->cursor_pos.width = ENTRY_CURSOR_WIDTH;
  priv->cursor_pos.height = rect.height / PANGO_SCALE;

  g_signal_emit (entry, entry_signals[CURSOR_EVENT], 0, &priv->cursor_pos);
}

static void
example_multiline_entry_clear_cursor_position (ExampleMultilineEntry *entry)
{
  entry->priv->cursor_pos.width = 0;
}

void
example_multiline_entry_paint_cursor (ExampleMultilineEntry *entry)
{
  ExampleMultilineEntryPrivate  *priv;

  priv   = entry->priv;

  clutter_actor_set_size (CLUTTER_ACTOR (priv->cursor),
                              priv->cursor_pos.width,
                              priv->cursor_pos.height);

  clutter_actor_set_position (priv->cursor,
                                  priv->cursor_pos.x,
                                  priv->cursor_pos.y);

  clutter_actor_paint (priv->cursor);
}

static void
example_multiline_entry_paint (ClutterActor *self)
{
  ExampleMultilineEntry         *entry;
  ExampleMultilineEntryPrivate  *priv;
  PangoRectangle        logical;
  gint                  width, actor_width;
  gint                  text_width;
  gint                  cursor_x;

  entry  = EXAMPLE_MULTILINE_ENTRY(self);
  priv   = entry->priv;

  if (priv->desc == NULL || priv->text == NULL)
    {
      return;
    }

  if (priv->width < 0)
    width = clutter_actor_get_width (self);
  else
    width = priv->width;

  clutter_actor_set_clip (self, 0, 0,
                          width,
                          clutter_actor_get_height (self));

  actor_width = width;
  example_multiline_entry_ensure_layout (entry, actor_width);
  example_multiline_entry_ensure_cursor_position (entry);

  pango_layout_get_extents (priv->layout, NULL, &logical);
  text_width = logical.width / PANGO_SCALE;

  if (actor_width < text_width)
    {
      /* We need to do some scrolling */
      cursor_x = priv->cursor_pos.x;

      /* If the cursor is at the begining or the end of the text, the placement
       * is easy, however, if the cursor is in the middle somewhere, we need to
       * make sure the text doesn't move until the cursor is either in the
       * far left or far right
       */

      if (priv->position == 0)
        priv->text_x = 0;
      else if (priv->position == -1)
        {
          priv->text_x = actor_width - text_width;
          priv->cursor_pos.x += priv->text_x;
        }
      else
        {
           if (priv->text_x <= 0)
             {
               gint diff = -1 * priv->text_x;

               if (cursor_x < diff)
                 priv->text_x += diff - cursor_x;
               else if (cursor_x > (diff + actor_width))
                 priv->text_x -= cursor_x - (diff+actor_width);
             }

           priv->cursor_pos.x += priv->text_x;
        }

    }
  else
    {
      priv->text_x = 0;
    }

  priv->fgcol.alpha = clutter_actor_get_opacity (self);
  pango_clutter_render_layout (priv->layout,
                               priv->text_x, 0,
                               &priv->fgcol, 0);

  if (EXAMPLE_MULTILINE_ENTRY_GET_CLASS (entry)->paint_cursor)
    EXAMPLE_MULTILINE_ENTRY_GET_CLASS (entry)->paint_cursor (entry);
}

static void
example_multiline_entry_request_coords (ClutterActor    *self,
			      ClutterActorBox *box)
{
  ExampleMultilineEntry *entry = EXAMPLE_MULTILINE_ENTRY (self);
  ExampleMultilineEntryPrivate *priv = entry->priv;
  gint width;

  width = CLUTTER_UNITS_TO_DEVICE (box->x2 - box->x1);

  if (priv->width != width)
    {
      example_multiline_entry_clear_layout (entry);
      example_multiline_entry_ensure_layout (entry, width);

      priv->width = width;
    }

  CLUTTER_ACTOR_CLASS (example_multiline_entry_parent_class)->request_coords (self, box);
}

static void
example_multiline_entry_dispose (GObject *object)
{
  ExampleMultilineEntry         *self = EXAMPLE_MULTILINE_ENTRY(object);
  ExampleMultilineEntryPrivate  *priv;

  priv = self->priv;

  if (priv->layout)
    {
      g_object_unref (priv->layout);
      priv->layout = NULL;
    }

  if (priv->context)
    {
      g_object_unref (priv->context);
      priv->context = NULL;
    }

  G_OBJECT_CLASS (example_multiline_entry_parent_class)->dispose (object);
}

static void
example_multiline_entry_finalize (GObject *object)
{
  ExampleMultilineEntryPrivate *priv = EXAMPLE_MULTILINE_ENTRY (object)->priv;

  if (priv->desc)
    pango_font_description_free (priv->desc);

  g_free (priv->text);
  g_free (priv->font_name);

  G_OBJECT_CLASS (example_multiline_entry_parent_class)->finalize (object);
}

static void
example_multiline_entry_class_init (ExampleMultilineEntryClass *klass)
{
  GObjectClass        *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  klass->paint_cursor = example_multiline_entry_paint_cursor;

  actor_class->paint           = example_multiline_entry_paint;
  actor_class->request_coords  = example_multiline_entry_request_coords;

  gobject_class->finalize     = example_multiline_entry_finalize;
  gobject_class->dispose      = example_multiline_entry_dispose;
  gobject_class->set_property = example_multiline_entry_set_property;
  gobject_class->get_property = example_multiline_entry_get_property;

  /**
   * ExampleMultilineEntry:font-name:
   *
   * The font to be used by the entry, expressed in a string that
   * can be parsed by pango_font_description_from_string().
   *
   */
  g_object_class_install_property
    (gobject_class, PROP_FONT_NAME,
     g_param_spec_string ("font-name",
			  "Font Name",
			  "Pango font description",
			  NULL,
			  G_PARAM_READABLE | G_PARAM_WRITABLE));
  /**
   * ExampleMultilineEntry:text:
   *
   * The text inside the entry.
   *
   */
  g_object_class_install_property
    (gobject_class, PROP_TEXT,
     g_param_spec_string ("text",
			  "Text",
			  "Text to render",
			  NULL,
			  G_PARAM_READABLE | G_PARAM_WRITABLE));
  /**
   * ExampleMultilineEntry:color:
   *
   * The color of the text inside the entry.
   *
   */
  g_object_class_install_property
    (gobject_class, PROP_COLOR,
     g_param_spec_boxed ("color",
			 "Font Colour",
			 "Font Colour",
			 CLUTTER_TYPE_COLOR,
			 G_PARAM_READABLE | G_PARAM_WRITABLE));


  /**
   * ExampleMultilineEntry::text-changed:
   * @entry: the actor which received the event
   *
   * The ::text-changed signal is emitted after @entry's text changes
   */
  entry_signals[TEXT_CHANGED] =
    g_signal_new ("text-changed",
                  G_TYPE_FROM_CLASS (gobject_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (ExampleMultilineEntryClass, text_changed),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__VOID,
                  G_TYPE_NONE, 0);

  /**
   * ExampleMultilineEntry::cursor-event:
   * @entry: the actor which received the event
   * @geometry: a #ClutterGeometry
   *
   * The ::cursor-event signal is emitted each time the input cursor's geometry
   * changes, this could be a positional or size change. If you would like to
   * implement your own input cursor, set the cursor-visible property to %FALSE,
   * and connect to this signal to position and size your own cursor.
   *
   */
   entry_signals[CURSOR_EVENT] =
    g_signal_new ("cursor-event",
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (ExampleMultilineEntryClass, cursor_event),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__BOXED,
		  G_TYPE_NONE, 1,
		  CLUTTER_TYPE_GEOMETRY | G_SIGNAL_TYPE_STATIC_SCOPE);

  g_type_class_add_private (gobject_class, sizeof (ExampleMultilineEntryPrivate));
}

static void
example_multiline_entry_init (ExampleMultilineEntry *self)
{
  ExampleMultilineEntryPrivate *priv;
  gdouble resolution;
  gint font_size;

  self->priv = priv = EXAMPLE_MULTILINE_ENTRY_GET_PRIVATE (self);

  resolution = clutter_backend_get_resolution (clutter_get_default_backend ());
  if (resolution < 0)
    resolution = 96.0; /* fall back */

  if (G_UNLIKELY (_context == NULL))
    {
      _font_map = PANGO_CLUTTER_FONT_MAP (pango_clutter_font_map_new ());
      pango_clutter_font_map_set_resolution (_font_map, resolution);
      _context = pango_clutter_font_map_create_context (_font_map);
    }

  priv->layout        = NULL;
  priv->text          = NULL;
  priv->position      = -1;
  priv->text_x        = 0;

  priv->fgcol.red     = 0;
  priv->fgcol.green   = 0;
  priv->fgcol.blue    = 0;
  priv->fgcol.alpha   = 255;

  priv->font_name     = g_strdup (DEFAULT_FONT_NAME);
  priv->desc          = pango_font_description_from_string (priv->font_name);

  /* We use the font size to set the default width and height, in case
   * the user doesn't call clutter_actor_set_size().
   */
  font_size = PANGO_PIXELS (pango_font_description_get_size (priv->desc))
              * resolution
              / 72.0;
  clutter_actor_set_size (CLUTTER_ACTOR (self), font_size * 20, 50);

  priv->cursor        = clutter_rectangle_new_with_color (&priv->fgcol);
  clutter_actor_set_parent (priv->cursor, CLUTTER_ACTOR (self));
}


/**
 * example_multiline_entry_new:
 *
 * Creates a new, empty #ExampleMultilineEntry.
 *
 * Returns: the newly created #ExampleMultilineEntry
 */
ClutterActor *
example_multiline_entry_new (void)
{
  ClutterActor *entry =  g_object_new (CLUTTER_TYPE_MULTILINE_ENTRY,
                                       NULL);
  clutter_actor_set_size (entry, 50, 50);

  return entry;
}

/**
 * example_multiline_entry_get_text:
 * @entry: a #ExampleMultilineEntry
 *
 * Retrieves the text displayed by @entry.
 *
 * Return value: the text of the entry.  The returned string is
 *   owned by #ExampleMultilineEntry and should not be modified or freed.
 *
 */
G_CONST_RETURN gchar *
example_multiline_entry_get_text (ExampleMultilineEntry *entry)
{
  g_return_val_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry), NULL);

  return entry->priv->text;
}

/**
 * example_multiline_entry_set_text:
 * @entry: a #ExampleMultilineEntry
 * @text: the text to be displayed
 *
 * Sets @text as the text to be displayed by @entry. The
 * ExampleMultilineEntry::text-changed signal is emitted.
 */
void
example_multiline_entry_set_text (ExampleMultilineEntry *entry,
		        const gchar  *text)
{
  ExampleMultilineEntryPrivate  *priv;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));
  g_return_if_fail (text != NULL);

  priv = entry->priv;

  g_object_ref (entry);

  g_free (priv->text);

  priv->text = g_strdup (text);
  priv->n_chars = g_utf8_strlen (priv->text, -1);

  example_multiline_entry_clear_layout (entry);
  example_multiline_entry_clear_cursor_position (entry);

  if (CLUTTER_ACTOR_IS_VISIBLE (entry))
    clutter_actor_queue_redraw (CLUTTER_ACTOR (entry));

  g_signal_emit (G_OBJECT (entry), entry_signals[TEXT_CHANGED], 0);

  g_object_notify (G_OBJECT (entry), "text");
  g_object_unref (entry);
}

/**
 * example_multiline_entry_get_font_name:
 * @entry: a #ExampleMultilineEntry
 *
 * Retrieves the font used by @entry.
 *
 * Return value: a string containing the font name, in a format
 *   understandable by pango_font_description_from_string().  The
 *   string is owned by #ExampleMultilineEntry and should not be modified
 *   or freed.
 */
G_CONST_RETURN gchar *
example_multiline_entry_get_font_name (ExampleMultilineEntry *entry)
{
  g_return_val_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry), NULL);

  return entry->priv->font_name;
}

/**
 * example_multiline_entry_set_font_name:
 * @entry: a #ExampleMultilineEntry
 * @font_name: a font name and size, or %NULL for the default font
 *
 * Sets @font_name as the font used by @entry.
 *
 * @font_name must be a string containing the font name and its
 * size, similarly to what you would feed to the
 * pango_font_description_from_string() function.
 */
void
example_multiline_entry_set_font_name (ExampleMultilineEntry *entry,
		             const gchar  *font_name)
{
  ExampleMultilineEntryPrivate *priv;
  PangoFontDescription *desc;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));

  if (!font_name || font_name[0] == '\0')
    font_name = DEFAULT_FONT_NAME;

  priv = entry->priv;

  if (strcmp (priv->font_name, font_name) == 0)
    return;

  desc = pango_font_description_from_string (font_name);
  if (!desc)
    {
      g_warning ("Attempting to create a PangoFontDescription for "
		 "font name `%s', but failed.",
		 font_name);
      return;
    }

  g_object_ref (entry);

  g_free (priv->font_name);
  priv->font_name = g_strdup (font_name);

  if (priv->desc)
    pango_font_description_free (priv->desc);

  priv->desc = desc;

  if (entry->priv->text && entry->priv->text[0] != '\0')
    {
      example_multiline_entry_clear_layout (entry);

      if (CLUTTER_ACTOR_IS_VISIBLE (entry))
	clutter_actor_queue_redraw (CLUTTER_ACTOR (entry));
    }

  g_object_notify (G_OBJECT (entry), "font-name");
  g_object_unref (entry);
}


/**
 * example_multiline_entry_set_color:
 * @entry: a #ExampleMultilineEntry
 * @color: a #ClutterColor
 *
 * Sets the color of @entry.
 */
void
example_multiline_entry_set_color (ExampleMultilineEntry       *entry,
		         const ClutterColor *color)
{
  ClutterActor *actor;
  ExampleMultilineEntryPrivate *priv;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));
  g_return_if_fail (color != NULL);

  priv = entry->priv;

  g_object_ref (entry);

  priv->fgcol.red = color->red;
  priv->fgcol.green = color->green;
  priv->fgcol.blue = color->blue;
  priv->fgcol.alpha = color->alpha;

  actor = CLUTTER_ACTOR (entry);

  clutter_actor_set_opacity (actor, priv->fgcol.alpha);

  clutter_rectangle_set_color (CLUTTER_RECTANGLE (priv->cursor), &priv->fgcol);

  if (CLUTTER_ACTOR_IS_VISIBLE (actor))
    clutter_actor_queue_redraw (actor);

  g_object_notify (G_OBJECT (entry), "color");
  g_object_unref (entry);
}

/**
 * example_multiline_entry_get_color:
 * @entry: a #ExampleMultilineEntry
 * @color: return location for a #ClutterColor
 *
 * Retrieves the color of @entry.
 */
void
example_multiline_entry_get_color (ExampleMultilineEntry *entry,
			 ClutterColor *color)
{
  ExampleMultilineEntryPrivate *priv;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));
  g_return_if_fail (color != NULL);

  priv = entry->priv;

  color->red = priv->fgcol.red;
  color->green = priv->fgcol.green;
  color->blue = priv->fgcol.blue;
  color->alpha = priv->fgcol.alpha;
}

/**
 * example_multiline_entry_set_cursor_position:
 * @entry: a #ExampleMultilineEntry
 * @position: the position of the cursor.
 *
 * Sets the position of the cursor. The @position must be less than or
 * equal to the number of characters in the entry. A value of -1 indicates
 * that the position should be set after the last character in the entry.
 * Note that this position is in characters, not in bytes.
 */
static void
example_multiline_entry_set_cursor_position (ExampleMultilineEntry *entry,
                                   gint          position)
{
  ExampleMultilineEntryPrivate *priv;
  gint len;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));

  priv = entry->priv;

  if (priv->text == NULL)
    return;

  len = g_utf8_strlen (priv->text, -1);

  if (position < 0 || position >= len)
    priv->position = -1;
  else
    priv->position = position;

  example_multiline_entry_clear_cursor_position (entry);

  if (CLUTTER_ACTOR_IS_VISIBLE (entry))
    clutter_actor_queue_redraw (CLUTTER_ACTOR (entry));
}


/**
 * example_multiline_entry_handle_key_event:
 * @entry: a #ExampleMultilineEntry
 * @kev: a #ClutterKeyEvent
 *
 * This function will handle a #ClutterKeyEvent, like those returned in a
 * key-press/release-event, and will translate it for the @entry. This includes
 * non-alphanumeric keys, such as the arrows keys, which will move the
 * input cursor. You should use this function inside a handler for the
 * ClutterStage::key-press-event or ClutterStage::key-release-event.
 */
void
example_multiline_entry_handle_key_event (ExampleMultilineEntry    *entry,
                                ClutterKeyEvent *kev)
{
  ExampleMultilineEntryPrivate *priv;
  gint pos = 0;
  gint len = 0;
  gint keyval = clutter_key_event_symbol (kev);

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));

  priv = entry->priv;

  pos = priv->position;

  if (priv->text)
    len = g_utf8_strlen (priv->text, -1);

  switch (keyval)
    {
      case CLUTTER_Escape:
      case CLUTTER_Shift_L:
      case CLUTTER_Shift_R:
        /* Ignore these - Don't try to insert them as characters. */
        break;

      case CLUTTER_BackSpace:
        /* Delete the current character: */
        if (pos != 0 && len != 0)
          example_multiline_entry_delete_chars (entry, 1);
        break;

      case CLUTTER_Delete:
      case CLUTTER_KP_Delete:
        /* Delete the current character: */
        if (len && pos != -1)
          example_multiline_entry_delete_text (entry, pos, pos+1);;
        break;

      case CLUTTER_Left:
      case CLUTTER_KP_Left:
        /* Move the cursor one character left: */
        if (pos != 0 && len != 0)
          {
            if (pos == -1)
              example_multiline_entry_set_cursor_position (entry, len - 1);
            else
              example_multiline_entry_set_cursor_position (entry, pos - 1);
          }
        break;

      case CLUTTER_Right:
      case CLUTTER_KP_Right:
        /* Move the cursor one character right: */
        if (pos != -1 && len != 0)
          {
            if (pos != len)
              example_multiline_entry_set_cursor_position (entry, pos + 1);
          }
        break;

      case CLUTTER_Up:
      case CLUTTER_KP_Up:
        /* TODO: Calculate the index of the position on the line above, 
           and set the cursor to it. */
           break;

      case CLUTTER_Down:
      case CLUTTER_KP_Down:
        /* TODO: Calculate the index of the position on the line below, 
           and set the cursor to it. */
           break;

      case CLUTTER_End:
      case CLUTTER_KP_End:
        /* Move the cursor to the end of the text: */
        example_multiline_entry_set_cursor_position (entry, -1);
        break;

      case CLUTTER_Begin:
      case CLUTTER_Home:
      case CLUTTER_KP_Home:
        /* Move the cursor to the start of the text: */
        example_multiline_entry_set_cursor_position (entry, 0);
        break;

    
      default:
        example_multiline_entry_insert_unichar (entry,
                                      clutter_keysym_to_unicode (keyval));
        break;
    }
}

/**
 * example_multiline_entry_insert_unichar:
 * @entry: a #ExampleMultilineEntry
 * @wc: a Unicode character
 *
 * Insert a character to the right of the current position of the cursor,
 * and updates the position of the cursor.
 *
 */
static void
example_multiline_entry_insert_unichar (ExampleMultilineEntry *entry,
                              gunichar      wc)
{
  ExampleMultilineEntryPrivate *priv;
  GString *new = NULL;
  glong pos;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));
  g_return_if_fail (g_unichar_validate (wc));

  if (wc == 0)
    return;

  priv = entry->priv;

  g_object_ref (entry);

  new = g_string_new (priv->text);
  pos = offset_to_bytes (priv->text, priv->position);
  new = g_string_insert_unichar (new, pos, wc);

  example_multiline_entry_set_text (entry, new->str);

  if (priv->position >= 0)
    example_multiline_entry_set_cursor_position (entry, priv->position + 1);

  g_string_free (new, TRUE);

  g_object_notify (G_OBJECT (entry), "text");
  g_object_unref (entry);
}

/**
 * example_multiline_entry_delete_chars:
 * @entry: a #ExampleMultilineEntry
 * @len: the number of characters to remove.
 *
 * Characters are removed from before the current postion of the cursor.
 *
 */
static void
example_multiline_entry_delete_chars (ExampleMultilineEntry *entry,
                            guint         num)
{
  ExampleMultilineEntryPrivate *priv;
  GString *new = NULL;
  gint len;
  gint pos;
  gint num_pos;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));

  priv = entry->priv;

  if (!priv->text)
    return;

  g_object_ref (entry);

  len = g_utf8_strlen (priv->text, -1);
  new = g_string_new (priv->text);

  if (priv->position == -1)
   {
     num_pos = offset_to_bytes (priv->text, len - num);
     new = g_string_erase (new, num_pos, -1);
   }
  else
  {
    pos = offset_to_bytes (priv->text, priv->position - num);
    num_pos = offset_to_bytes (priv->text, priv->position);
    new = g_string_erase (new, pos, num_pos-pos);
  }
  example_multiline_entry_set_text (entry, new->str);

  if (priv->position > 0)
    example_multiline_entry_set_cursor_position (entry, priv->position - num);

  g_string_free (new, TRUE);

  g_object_notify (G_OBJECT (entry), "text");
  g_object_unref (entry);
}

/**
 * example_multiline_entry_delete_text:
 * @entry: a #ExampleMultilineEntry
 * @start_pos: the starting position.
 * @end_pos: the end position.
 *
 * Deletes a sequence of characters. The characters that are deleted are
 * those characters at positions from @start_pos up to, but not including,
 * @end_pos. If @end_pos is negative, then the characters deleted will be
 * those characters from @start_pos to the end of the text.
 *
 */
void
example_multiline_entry_delete_text (ExampleMultilineEntry       *entry,
                           gssize              start_pos,
                           gssize              end_pos)
{
  ExampleMultilineEntryPrivate *priv;
  GString *new = NULL;
  gint start_bytes;
  gint end_bytes;

  g_return_if_fail (EXAMPLE_IS_MULTILINE_ENTRY (entry));

  priv = entry->priv;

  if (!priv->text)
    return;

  start_bytes = offset_to_bytes (priv->text, start_pos);
  end_bytes = offset_to_bytes (priv->text, end_pos);

  new = g_string_new (priv->text);
  new = g_string_erase (new, start_bytes, end_bytes - start_bytes);

  example_multiline_entry_set_text (entry, new->str);

  g_string_free (new, TRUE);
}


