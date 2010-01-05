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


#include "examplebox.h"

#include <clutter/clutter.h>
#include <cogl/cogl.h>

#include <string.h>

/**
 * SECTION:example-box
 * @short_description: Simple example of a container actor.
 *
 * #ExampleBox imposes a specific layout on its children, 
 * unlike #ClutterGroup which is a free-form container.
 *
 * Specifically, ExampleBox lays out its children along an imaginary
 * horizontal line.
 */

static void clutter_container_iface_init (ClutterContainerIface *iface);

G_DEFINE_TYPE_WITH_CODE (ExampleBox,
                         example_box,
                         CLUTTER_TYPE_ACTOR,
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_CONTAINER,
                                                clutter_container_iface_init));

/* An implementation for the ClutterContainer::add() vfunc: */
static void
example_box_add (ClutterContainer *container,
                 ClutterActor     *actor)
{
  example_box_pack (EXAMPLE_BOX (container), actor);
}

/* An implementation for the ClutterContainer::remove() vfunc: */
static void
example_box_remove (ClutterContainer *container,
                    ClutterActor     *actor)
{
  ExampleBox *box = EXAMPLE_BOX (container);
  GList *l;

  g_object_ref (actor);

  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      if (child == actor)
        {
          clutter_actor_unparent (child);
          
          box->children = g_list_remove_link (box->children, l);
          g_list_free (l);

          g_signal_emit_by_name (container, "actor-removed", actor);

          /* queue a relayout of the container */
	  clutter_actor_queue_relayout (CLUTTER_ACTOR (box));

          break;
        }
    }

  g_object_unref (actor);
}

/* An implementation for the ClutterContainer::foreach() vfunc: */
static void
example_box_foreach (ClutterContainer *container,
                     ClutterCallback   callback,
                     gpointer          user_data)
{
  ExampleBox *box = EXAMPLE_BOX (container);
  GList *l;

  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      (* callback) (child, user_data);
    }
}

static void
clutter_container_iface_init (ClutterContainerIface *iface)
{
  /* Provide implementations for ClutterContainer vfuncs: */
  iface->add = example_box_add;
  iface->remove = example_box_remove;
  iface->foreach = example_box_foreach;
}

/* An implementation for the ClutterActor::show_all() vfunc,
   showing all the child actors: */
static void
example_box_show_all (ClutterActor *actor)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;

  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      clutter_actor_show (child);
    }

  clutter_actor_show (actor);
}

/* An implementation for the ClutterActor::hide_all() vfunc,
   hiding all the child actors: */
static void
example_box_hide_all (ClutterActor *actor)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;

  clutter_actor_hide (actor);

  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      clutter_actor_hide (child);
    }
}

/* An implementation for the ClutterActor::paint() vfunc,
   painting all the child actors: */
static void
example_box_paint (ClutterActor *actor)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;

  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      if (CLUTTER_ACTOR_IS_MAPPED (child))
        clutter_actor_paint (child);
    }
}

/* An implementation for the ClutterActor::pick() vfunc,
   picking all the child actors: */
static void
example_box_pick (ClutterActor *actor, 
                  const ClutterColor *color)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l = NULL;
  
  /* Chain up so we get a bounding box painted (if we are reactive) */
  CLUTTER_ACTOR_CLASS (example_box_parent_class)->pick (actor, color);


  /* TODO: Do something with the color?
   * In clutter 0.8 we used it to call clutter_actor_pick() on the children,
   * but now we call clutter_actor_paint() instead.
   */
  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      if (CLUTTER_ACTOR_IS_MAPPED (child))
        clutter_actor_paint (child);
    }
}

/* An implementation for the ClutterActor::get_preferred_width() vfunc: */
static void
example_box_get_preferred_width (ClutterActor *actor,
                                 float for_height,
                                 float *min_width_p,
                                 float *natural_width_p)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;
  float min_width = 0, natural_width = 0;

  /* For this container, the preferred width is the sum of the widths
   * of the children. The preferred width depends on the height provided
   * by for_height.
   */

  /* Calculate the preferred width for this container, 
   * based on the preferred width requested by the children: */
  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;
      
      if (CLUTTER_ACTOR_IS_VISIBLE (child))
        {
          float child_min_width = 0;
          float child_natural_width = 0;
          clutter_actor_get_preferred_width (child, for_height, &child_min_width, &child_natural_width);

          min_width += child_min_width;
          natural_width += child_natural_width;
        }
    }

  if (min_width_p)
    *min_width_p = min_width;

  if (natural_width_p)
    *natural_width_p = natural_width;
}

/* An implementation for the ClutterActor::get_preferred_height() vfunc: */
static void
example_box_get_preferred_height (ClutterActor *actor,
                                  float for_width,
                                  float *min_height_p,
                                  float *natural_height_p)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;
  float min_height = 0, natural_height = 0;

  /* For this container, the preferred height is the maximum height
   * of the children. The preferred height is independent of the given width.
   */

  /* Calculate the preferred height for this container, 
   * based on the preferred height requested by the children: */
  for (l = box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;
      
      if (CLUTTER_ACTOR_IS_VISIBLE (child))
        {
          float child_min_height = 0;
          float child_natural_height = 0;
          clutter_actor_get_preferred_height (child, -1, &child_min_height, &child_natural_height);

          min_height = MAX (min_height, child_min_height);
          natural_height = MAX (natural_height, child_natural_height);
        }
    }

  if (min_height_p)
    *min_height_p = min_height;

  if (natural_height_p)
    *natural_height_p = natural_height;
}

/* An implementation for the ClutterActor::allocate() vfunc: */
static void
example_box_allocate (ClutterActor          *actor,
                      const ClutterActorBox *box,
		                  ClutterAllocationFlags absolute_origin_changed)
{
  ExampleBox *ebox = EXAMPLE_BOX (actor);

  /* Look at each child actor: */
  float child_x = 0;
  GList *l = NULL;
  for (l = ebox->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      /* Discover what size the child wants: */
      float child_width = 0;
      float child_height = 0;
      clutter_actor_get_preferred_size (child, NULL, NULL, &child_width, &child_height);

      /* Calculate the position and size that the child may actually have: */

      /* Position the child just after the previous child, horizontally: */
      ClutterActorBox child_box = { 0, };
      child_box.x1 = child_x;
      child_box.x2 = child_x + child_width;
      child_x = child_box.x2;

      /* Position the child at the top of the container: */
      child_box.y1 = 0;
      child_box.y2 = child_box.y1 + child_height;

      /* Tell the child what position and size it may actually have: */
      clutter_actor_allocate (child, &child_box, absolute_origin_changed);
    }

  CLUTTER_ACTOR_CLASS (example_box_parent_class)->allocate (actor, box, absolute_origin_changed);
}

static void
example_box_dispose (GObject *gobject)
{
  /* Destroy each child actor when this container is destroyed: */
  ExampleBox *box = EXAMPLE_BOX (gobject);
  GList *l;

  for (l =  box->children; l; l = l->next)
    {
      ClutterActor *child = l->data;

      clutter_actor_destroy (child);
    }

  g_list_free (box->children);
  box->children = NULL;

  G_OBJECT_CLASS (example_box_parent_class)->dispose (gobject);
}

static void
example_box_class_init (ExampleBoxClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  gobject_class->dispose = example_box_dispose;

  /* Provide implementations for ClutterActor vfuncs: */
  actor_class->show_all = example_box_show_all;
  actor_class->hide_all = example_box_hide_all;
  actor_class->paint = example_box_paint;
  actor_class->pick = example_box_pick;
  actor_class->get_preferred_width = example_box_get_preferred_width;
  actor_class->get_preferred_height = example_box_get_preferred_height;
  actor_class->allocate = example_box_allocate;
}

static void
example_box_init (ExampleBox *box)
{
  /* The required width depends on a given height in this container */
  g_object_set (G_OBJECT (box),
                "request-mode", CLUTTER_REQUEST_WIDTH_FOR_HEIGHT,
                NULL);
}

/*
 * Public API
 */

/**
 * example_box_pack:
 * @box: a #ExampleBox
 * @actor: a #ClutterActor to pack into the box
 *
 * Packs @actor into @box.
 */
void
example_box_pack (ExampleBox           *box,
                  ClutterActor         *actor)
{

  g_return_if_fail (EXAMPLE_IS_BOX (box));
  g_return_if_fail (CLUTTER_IS_ACTOR (actor));

  box->children = g_list_prepend (box->children, actor);
  clutter_actor_set_parent (actor, CLUTTER_ACTOR (box));

  /* queue a relayout of the container */
  clutter_actor_queue_relayout (CLUTTER_ACTOR (box));
}


/**
 * example_box_remove_all:
 * @box: a #ExampleBox
 *
 * Removes all child actors from the #ExampleBox
 */
void
example_box_remove_all (ExampleBox *box)
{
  GList *children;

  g_return_if_fail (EXAMPLE_IS_BOX (box));

  children = box->children;
  while (children)
    {
      ClutterActor *child = children->data;
      children = children->next;

      clutter_container_remove_actor (CLUTTER_CONTAINER (box), child);
    }
}


/**
 * example_box_new:
 *
 * Creates a new box.
 *
 * Return value: the newly created #ExampleBox
 */
ClutterActor *
example_box_new (void)
{
  return g_object_new (EXAMPLE_TYPE_BOX, NULL);
}


