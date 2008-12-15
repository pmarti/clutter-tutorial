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

#include "triangle_actor.h"

#include <cogl/cogl.h>

G_DEFINE_TYPE (ClutterTriangle, clutter_triangle, CLUTTER_TYPE_ACTOR);

enum
{
  PROP_0,

  PROP_COLOR
};

#define CLUTTER_TRIANGLE_GET_PRIVATE(obj) \
(G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_TRIANGLE, ClutterTrianglePrivate))

struct _ClutterTrianglePrivate
{
  ClutterColor color;
};

static void
do_triangle_paint (ClutterActor *self, const CoglColor *color)
{
  ClutterTriangle        *triangle;
  ClutterTrianglePrivate *priv;
  ClutterGeometry         geom;
  ClutterFixed            coords[6];

  triangle = CLUTTER_TRIANGLE(self);
  priv = triangle->priv;

  cogl_push_matrix();

  clutter_actor_get_geometry (self, &geom);

  cogl_set_source_color (color);

  /* Paint a triangle:
   *
   * The parent paint call will have translated us into position so
   * paint from 0, 0 */
  coords[0] = CLUTTER_INT_TO_FIXED (0);
  coords[1] = CLUTTER_INT_TO_FIXED (0);

  coords[2] = CLUTTER_INT_TO_FIXED (0);
  coords[3] = CLUTTER_INT_TO_FIXED (geom.height);

  coords[4] = CLUTTER_INT_TO_FIXED (geom.width);
  coords[5] = CLUTTER_INT_TO_FIXED (geom.height);

  cogl_path_polygon (coords, 3);
  cogl_path_fill ();

  cogl_pop_matrix();
}

static void
clutter_triangle_paint (ClutterActor *self)
{
  ClutterTrianglePrivate *priv;
  CoglColor color;

  priv = CLUTTER_TRIANGLE(self)->priv;

  /* Paint the triangle with the actor's color: */
  cogl_color_set_from_4ub(&color,
                          priv->color.red,
                          priv->color.green,
                          priv->color.blue,
                          clutter_actor_get_opacity (self));

  do_triangle_paint (self, &color);
}

static void
clutter_triangle_pick (ClutterActor *self, const ClutterColor *color)
{
  CoglColor coglcolor;
  /* Paint the triangle with the pick color, offscreen.
     This is used by Clutter to detect the actor under the cursor 
     by identifying the unique color under the cursor. */
  cogl_color_set_from_4ub(&coglcolor,
                          color->red, color->green, color->blue, color->alpha);
  do_triangle_paint (self, &coglcolor);
}

static void
clutter_triangle_set_property (GObject      *object,
				guint         prop_id,
				const GValue *value,
				GParamSpec   *pspec)
{
  ClutterTriangle *triangle = CLUTTER_TRIANGLE(object);

  switch (prop_id)
    {
    case PROP_COLOR:
      clutter_triangle_set_color (triangle, g_value_get_boxed (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
clutter_triangle_get_property (GObject    *object,
				guint       prop_id,
				GValue     *value,
				GParamSpec *pspec)
{
  ClutterTriangle *triangle = CLUTTER_TRIANGLE(object);
  ClutterColor      color;

  switch (prop_id)
    {
    case PROP_COLOR:
      clutter_triangle_get_color (triangle, &color);
      g_value_set_boxed (value, &color);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


static void
clutter_triangle_finalize (GObject *object)
{
  G_OBJECT_CLASS (clutter_triangle_parent_class)->finalize (object);
}

static void
clutter_triangle_dispose (GObject *object)
{
  G_OBJECT_CLASS (clutter_triangle_parent_class)->dispose (object);
}


static void
clutter_triangle_class_init (ClutterTriangleClass *klass)
{
  GObjectClass        *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  /* Provide implementations for ClutterActor vfuncs: */
  actor_class->paint = clutter_triangle_paint;
  actor_class->pick = clutter_triangle_pick;

  gobject_class->finalize     = clutter_triangle_finalize;
  gobject_class->dispose      = clutter_triangle_dispose;
  gobject_class->set_property = clutter_triangle_set_property;
  gobject_class->get_property = clutter_triangle_get_property;

  /**
   * ClutterTriangle:color:
   *
   * The color of the triangle.
   */
  g_object_class_install_property (gobject_class,
                                   PROP_COLOR,
                                   g_param_spec_boxed ("color",
                                                       "Color",
                                                       "The color of the triangle",
                                                       CLUTTER_TYPE_COLOR,
                                                       G_PARAM_READABLE | G_PARAM_WRITABLE));

  g_type_class_add_private (gobject_class, sizeof (ClutterTrianglePrivate));
}

static void
clutter_triangle_init (ClutterTriangle *self)
{
  ClutterTrianglePrivate *priv;

  self->priv = priv = CLUTTER_TRIANGLE_GET_PRIVATE (self);

  priv->color.red = 0xff;
  priv->color.green = 0xff;
  priv->color.blue = 0xff;
  priv->color.alpha = 0xff;
}

/**
 * clutter_triangle_new:
 *
 * Creates a new #ClutterActor with a rectangular shape.
 *
 * Return value: a new #ClutterActor
 */
ClutterActor*
clutter_triangle_new (void)
{
  return g_object_new (CLUTTER_TYPE_TRIANGLE, NULL);
}

/**
 * clutter_triangle_new_with_color:
 * @color: a #ClutterColor
 *
 * Creates a new #ClutterActor with a rectangular shape
 * and with @color.
 *
 * Return value: a new #ClutterActor
 */
ClutterActor *
clutter_triangle_new_with_color (const ClutterColor *color)
{
  return g_object_new (CLUTTER_TYPE_TRIANGLE,
		       "color", color,
		       NULL);
}

/**
 * clutter_triangle_get_color:
 * @triangle: a #ClutterTriangle
 * @color: return location for a #ClutterColor
 *
 * Retrieves the color of @triangle.
 */
void
clutter_triangle_get_color (ClutterTriangle *triangle,
			     ClutterColor     *color)
{
  ClutterTrianglePrivate *priv;

  g_return_if_fail (CLUTTER_IS_TRIANGLE (triangle));
  g_return_if_fail (color != NULL);

  priv = triangle->priv;

  color->red = priv->color.red;
  color->green = priv->color.green;
  color->blue = priv->color.blue;
  color->alpha = priv->color.alpha;
}

/**
 * clutter_triangle_set_color:
 * @triangle: a #ClutterTriangle
 * @color: a #ClutterColor
 *
 * Sets the color of @triangle.
 */
void
clutter_triangle_set_color (ClutterTriangle   *triangle,
			     const ClutterColor *color)
{
  ClutterTrianglePrivate *priv;

  g_return_if_fail (CLUTTER_IS_TRIANGLE (triangle));
  g_return_if_fail (color != NULL);

  g_object_ref (triangle);

  priv = triangle->priv;

  priv->color.red = color->red;
  priv->color.green = color->green;
  priv->color.blue = color->blue;
  priv->color.alpha = color->alpha;

  clutter_actor_set_opacity (CLUTTER_ACTOR (triangle),
		  	       priv->color.alpha);

  if (CLUTTER_ACTOR_IS_VISIBLE (CLUTTER_ACTOR (triangle)))
    clutter_actor_queue_redraw (CLUTTER_ACTOR (triangle));

  g_object_notify (G_OBJECT (triangle), "color");
  g_object_unref (triangle);
}


