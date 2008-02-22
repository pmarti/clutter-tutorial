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

#ifndef _CLUTTER_TUTORIAL_TRIANGLE_ACTOR_H
#define _CLUTTER_TUTORIAL_TRIANGLE_ACTOR_H

#include <glib-object.h>
#include <clutter/clutter-actor.h>
#include <clutter/clutter-color.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_TRIANGLE clutter_triangle_get_type()

#define CLUTTER_TRIANGLE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CLUTTER_TYPE_TRIANGLE, ClutterTriangle))

#define CLUTTER_TRIANGLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CLUTTER_TYPE_TRIANGLE, ClutterTriangleClass))

#define CLUTTER_IS_TRIANGLE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CLUTTER_TYPE_TRIANGLE))

#define CLUTTER_IS_TRIANGLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CLUTTER_TYPE_TRIANGLE))

#define CLUTTER_TRIANGLE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CLUTTER_TYPE_TRIANGLE, ClutterTriangleClass))

typedef struct _ClutterTriangle        ClutterTriangle;
typedef struct _ClutterTriangleClass   ClutterTriangleClass;
typedef struct _ClutterTrianglePrivate ClutterTrianglePrivate;

struct _ClutterTriangle
{
  ClutterActor           parent;

  /*< private >*/
  ClutterTrianglePrivate *priv;
}; 

struct _ClutterTriangleClass 
{
  ClutterActorClass parent_class;
};

GType clutter_triangle_get_type (void) G_GNUC_CONST;

ClutterActor *clutter_triangle_new              (void);
ClutterActor *clutter_triangle_new_with_color   (const ClutterColor *color);

void          clutter_triangle_get_color        (ClutterTriangle   *triangle,
                                                  ClutterColor       *color);
void          clutter_triangle_set_color        (ClutterTriangle   *triangle,
						  const ClutterColor *color);

G_END_DECLS

#endif
