/* Copyright 2008 Openismus GmbH, 
 * based on ClutterBox and ClutterHBox from Clutter 0.4
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

#include "scrollingcontainer.h"
#include <clutter/clutter-container.h>
#include <clutter/clutter-group.h>
#include <clutter/clutter-rectangle.h>

#include <string.h>
#include <stdio.h>

/**
 * SECTION:example-scrolling-container
 * @short_description: This container shows only a small area
 * of its child actors, and the child actors can be scrolled 
 * left under that area.
 */


static void clutter_container_iface_init (ClutterContainerIface *iface);

G_DEFINE_TYPE_WITH_CODE (ExampleScrollingContainer,
                         example_scrolling_container,
                         CLUTTER_TYPE_ACTOR,
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_CONTAINER,
                                                clutter_container_iface_init));


/* An implementation for the ClutterContainer::add() vfunc: */
static void
example_scrolling_container_add (ClutterContainer *container,
                                 ClutterActor     *actor)
{
  example_scrolling_container_pack (EXAMPLE_SCROLLING_CONTAINER (container), actor);
}

/* An implementation for the ClutterContainer::remove() vfunc: */
static void
example_scrolling_container_remove (ClutterContainer *container,
                                    ClutterActor     *actor)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (container);
  GList *l;

  g_object_ref (actor);

  for (l = self->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      if (child == actor)
        {
          clutter_container_remove_actor (CLUTTER_CONTAINER (self->group), child);

          self->children = g_list_remove_link (self->children, l);
          g_list_free (l);

          g_signal_emit_by_name (container, "actor-removed", actor);

          clutter_actor_queue_relayout (CLUTTER_ACTOR (self));

          break;
        }
    }

  g_object_unref (actor);
}

/* An implementation for the ClutterContainer::foreach() vfunc: */
static void
example_scrolling_container_foreach (ClutterContainer *container,
                                     ClutterCallback   callback,
                                     gpointer          user_data)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (container);
  clutter_container_foreach (CLUTTER_CONTAINER (self->group), callback, user_data);
}

static void
clutter_container_iface_init (ClutterContainerIface *iface)
{
  /* Provide implementations for ClutterContainer vfuncs: */
  iface->add = example_scrolling_container_add;
  iface->remove = example_scrolling_container_remove;
  iface->foreach = example_scrolling_container_foreach;
}

/* An implementation for the ClutterActor::show_all() vfunc,
   showing all the child actors: */
static void
example_scrolling_container_show_all (ClutterActor *actor)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (actor);
  GList *l;

  for (l = self->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      clutter_actor_show (child);
    }

  clutter_actor_show (actor);
}

/* An implementation for the ClutterActor::hide_all() vfunc,
   hiding all the child actors: */
static void
example_scrolling_container_hide_all (ClutterActor *actor)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (actor);
  GList *l;

  clutter_actor_hide (actor);

  for (l = self->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      clutter_actor_hide (child);
    }
}

/* An implementation for the ClutterActor::paint() vfunc,
   painting all the child actors: */
static void
example_scrolling_container_paint (ClutterActor *actor)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (actor);
  clutter_actor_paint (self->group);
}

/* An implementation for the ClutterActor::pick() vfunc,
   picking all the child actors: */
static void
example_scrolling_container_pick (ClutterActor *actor, 
                                  const ClutterColor *color)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (actor);
  clutter_actor_pick (self->group, color);
}

/* An implementation for the ClutterActor::allocate() vfunc: */
static void
example_scrolling_container_allocate (ClutterActor          *actor,
                                      const ClutterActorBox *box,
                                      gboolean               absolute_origin_changed)
{
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (actor);

  /* Make sure that the children adapt their positions: */
  ClutterUnit width = box->x2 - box->x1;
  ClutterUnit height = box->y2 - box->y1;

  if (width < 0)
    width = 0;

  if (height < 0)
    height = 0;

  /* Arrange the group: */
  ClutterActorBox child_box = { 0, };
  child_box.x1 = 0;
  child_box.x2 = width;

  /* Position the child at the top of the container: */
  child_box.y1 = 0;
  child_box.y2 = height;

  clutter_actor_allocate (self->group, &child_box, absolute_origin_changed);

  /* Make sure that the group only shows the specified area, by clipping: */
  clutter_actor_set_clip (self->group, 0, 0, CLUTTER_UNITS_TO_DEVICE(width), CLUTTER_UNITS_TO_DEVICE(height));

  /* Show a rectangle border to show the area: */
  clutter_actor_allocate (self->rect, &child_box, absolute_origin_changed);
  clutter_actor_lower (self->rect, NULL);

  /* Look at each child actor: */
  gint child_x = -(self->offset);
  GList *l = NULL;
  for (l = self->children; l; l = l->next)
    {
      ClutterActor *child = l->data;
      ClutterUnit width, height;

      clutter_actor_get_preferred_size (child, NULL, NULL, &width, &height);

      child_box.x1 = CLUTTER_UNITS_FROM_DEVICE (child_x);
      child_box.y1 = 0;
      child_box.x2 = child_box.x1 + width;
      child_box.y2 = child_box.y1 + height;

      clutter_actor_allocate (child, &child_box, absolute_origin_changed);

      child_x += CLUTTER_UNITS_TO_DEVICE (width);
   }

  CLUTTER_ACTOR_CLASS (example_scrolling_container_parent_class)->allocate (actor, box, absolute_origin_changed);
}


static void
example_scrolling_container_dispose (GObject *gobject)
{
  /* Destroy each child actor when this container is destroyed: */
  ExampleScrollingContainer *self = EXAMPLE_SCROLLING_CONTAINER (gobject);
  GList *l;

  for (l =  self->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      clutter_actor_destroy (child);
    }

  g_list_free (self->children);
  self->children = NULL;

  if (self->group)
    {
      clutter_actor_destroy (self->group);
      self->group = NULL; 
  }

  G_OBJECT_CLASS (example_scrolling_container_parent_class)->dispose (gobject);
}

static void
example_scrolling_container_class_init (ExampleScrollingContainerClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  gobject_class->dispose = example_scrolling_container_dispose;

  /* Provide implementations for ClutterActor vfuncs: */
  actor_class->show_all = example_scrolling_container_show_all;
  actor_class->hide_all = example_scrolling_container_hide_all;
  actor_class->paint = example_scrolling_container_paint;
  actor_class->pick = example_scrolling_container_pick;
  actor_class->allocate = example_scrolling_container_allocate;
}

static void
example_scrolling_container_init (ExampleScrollingContainer *self)
{
  self->group = clutter_group_new ();
  clutter_actor_show (self->group);
  self->offset = 0;

  /* A rectange to show the bounds: */
  ClutterColor actor_color = { 0xff, 0xff, 0xcc, 0xff };
  self->rect = clutter_rectangle_new_with_color (&actor_color);
  clutter_container_add_actor (CLUTTER_CONTAINER (self->group), self->rect);
  clutter_actor_show (self->rect);
}

/*
 * Public API
 */

/**
 * example_scrolling_container_pack:
 * @self: a #ExampleScrollingContainer
 * @actor: a #ClutterActor to pack into the self
 *
 * Packs @actor into @self.
 */
void
example_scrolling_container_pack (ExampleScrollingContainer           *self,
                  ClutterActor         *actor)
{

  g_return_if_fail (EXAMPLE_IS_SCROLLING_CONTAINER (self));
  g_return_if_fail (CLUTTER_IS_ACTOR (actor));

  self->children = g_list_prepend (self->children, actor);
  clutter_container_add_actor (CLUTTER_CONTAINER (self->group), actor);

  clutter_actor_queue_relayout (CLUTTER_ACTOR (self));
}


/**
 * example_scrolling_container_remove_all:
 * @self: a #ExampleScrollingContainer
 *
 * Removes all child actors from the #ExampleScrollingContainer
 */
void
example_scrolling_container_remove_all (ExampleScrollingContainer *self)
{
  GList *children;

  g_return_if_fail (EXAMPLE_IS_SCROLLING_CONTAINER (self));

  children = self->children;
  while (children)
    {
      ClutterActor *child = children->data;
      children = children->next;

      clutter_container_remove_actor (CLUTTER_CONTAINER (self), child);
    }
}


/**
 * example_scrolling_container_new:
 *
 * Creates a new self.
 *
 * Return value: the newly created #ExampleScrollingContainer
 */
ClutterActor *
example_scrolling_container_new (void)
{
  return g_object_new (EXAMPLE_TYPE_SCROLLING_CONTAINER, NULL);
}

/**
 * example_scrolling_container_scroll_left:
 * @self: a #ExampleScrollingContainer
 * @distance: The number of pixels by which to scroll left.
 *
 * Scroll all the child widgets left, 
 * resulting in some parts being hidden, 
 * and some parts becoming visible.
 */
void example_scrolling_container_scroll_left (ExampleScrollingContainer *self, gint distance)
{
  g_return_if_fail (EXAMPLE_IS_SCROLLING_CONTAINER (self));

  self->offset += distance;

  clutter_actor_queue_relayout (CLUTTER_ACTOR (self));
}

