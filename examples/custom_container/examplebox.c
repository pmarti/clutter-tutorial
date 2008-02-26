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

#include "clutter/cogl.h" /* For some helper functions. */

#include "examplebox.h"
#include <clutter/clutter-container.h>

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


static void
layout_children (ExampleBox *box);

/**
 * ExampleBoxChild:
 * @actor: the child #ClutterActor
 * @child_coords: the original coordinates of the child
 *
 * Packing data for children of a #ExampleBox.
 *
 * Since: 0.4
 */
struct _ExampleBoxChild
{
  ClutterActor *actor;
  ClutterActorBox child_coords;
};

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
      ExampleBoxChild *child = l->data;

      if (child->actor == actor)
        {
          clutter_actor_unparent (actor);
          
          box->children = g_list_remove_link (box->children, l);
          g_list_free (l);
          g_slice_free (ExampleBoxChild, child);

          g_signal_emit_by_name (container, "actor-removed", actor);

          layout_children (box);

          if (CLUTTER_ACTOR_IS_VISIBLE (CLUTTER_ACTOR (box)))
            clutter_actor_queue_redraw (CLUTTER_ACTOR (box));
          
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
      ExampleBoxChild *child = l->data;

      (* callback) (child->actor, user_data);
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
      ExampleBoxChild *child = l->data;

      clutter_actor_show (child->actor);
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
      ExampleBoxChild *child = l->data;

      clutter_actor_hide (child->actor);
    }
}

/* An implementation for the ClutterActor::paint() vfunc,
   painting all the child actors: */
static void
example_box_paint (ClutterActor *actor)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;

  cogl_push_matrix ();

  for (l = box->children; l; l = l->next)
    {
      ExampleBoxChild *child = l->data;

      if (CLUTTER_ACTOR_IS_MAPPED (child->actor))
        clutter_actor_paint (child->actor);
    }

  cogl_pop_matrix ();
}

/* An implementation for the ClutterActor::query_coords() vfunc: */
static void
example_box_query_coords (ClutterActor    *actor,
                           ClutterActorBox *coords)
{
  ExampleBox *box = EXAMPLE_BOX (actor);
  GList *l;
  gint width, height;

  /* For this container, 
   * x1 and y1 will just be whatever was provided to request_coords(),
   * but the (desired) height and width (via x2 and y2) will be based 
   * on the height and width desired by the child actors.
   * Other containers might instead decide to reduce the size of the child actors 
   * to fit inside the container's specified height/width.
   */
  coords->x1 = box->allocation.x1;
  coords->y1 = box->allocation.y1;
  
  /* Calculate the x2 and y2 allocation for this container, 
   * based on the allocations requested by the children: */
  width = 0;
  height = 0;

  for (l = box->children; l; l = l->next)
    {
      ExampleBoxChild *child = l->data;
      
      if (CLUTTER_ACTOR_IS_VISIBLE (child->actor))
        {
          guint child_width, child_height;

          clutter_actor_get_size (child->actor, &child_width, &child_height);

          width = width + child_width;

          height = MAX (child_height, height);
        }
    }

  box->allocation.x2 = coords->x2 = 
        coords->x1 + CLUTTER_UNITS_FROM_INT (width);
  box->allocation.y2 = coords->y2 = 
        coords->y1 + CLUTTER_UNITS_FROM_INT (height);
}

/* An implementation for the ClutterActor::request_coords() vfunc: */
static void
example_box_request_coords (ClutterActor    *actor,
                             ClutterActorBox *coords)
{
  ExampleBox *box = EXAMPLE_BOX (actor);

  /* Store the provided allocation.
     But we only store x1 and y1 because the width and height are 
     dependent on on the children. */
  box->allocation.x1 = coords->x1;
  box->allocation.y1 = coords->y1;
  box->allocation.x2 = -1;
  box->allocation.y2 = -1;

  /* Make sure that the children adapt their positions: */
  layout_children (EXAMPLE_BOX (actor));
}


static void
example_box_dispose (GObject *gobject)
{
  /* Destroy each child actor when this container is destroyed: */
  ExampleBox *box = EXAMPLE_BOX (gobject);
  GList *l;

  for (l =  box->children; l; l = l->next)
    {
      ExampleBoxChild *child = l->data;

      clutter_actor_destroy (child->actor);
      g_slice_free (ExampleBoxChild, child);
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
  actor_class->query_coords = example_box_query_coords;
  actor_class->request_coords = example_box_request_coords;
}

static void
example_box_init (ExampleBox *box)
{
  box->allocation.x1 = box->allocation.y1 = 0;
  box->allocation.x2 = box->allocation.y2 = -1;
}


static void
layout_children (ExampleBox *box)
{
  /* Get the size requested by this container: */
  ClutterActorBox allocation = { 0, };
  clutter_actor_query_coords (CLUTTER_ACTOR (box), &allocation);

  ClutterUnit width = allocation.x2 - allocation.x1;
  ClutterUnit height = allocation.y2 - allocation.y1;

  if (width < 0)
    width = 0;

  if (height < 0)
    height = 0;

  /* Look at each child actor: */
  ClutterUnit child_x = 0;
  GList *l = NULL;
  for (l = box->children; l; l = l->next)
    {
      ExampleBoxChild *child = l->data;

      /* Discover what size the child wants: */
      ClutterActorBox child_req = { 0, };
      clutter_actor_query_coords (child->actor, &child_req);

      const ClutterUnit child_width = child_req.x2 - child_req.x1;
      const ClutterUnit child_height = child_req.y2 - child_req.y1;

      /* Calculate the position and size that the child may actually have: */

      /* Position the child just after the previous child, horizontally: */
      ClutterActorBox child_box = { 0, };
      child_box.x1 = child_x;
      child_box.x2 = child_x + child_width;
      child_x = child_box.x2;

      /* Position the child at the top of the container: */
      child_box.y1 = 0;
      child_box.y2 = child_height;

      /* Tell the child what position and size it may actually have: */
      clutter_actor_request_coords (child->actor, &child_box);
   }
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
  ExampleBoxChild *child;

  g_return_if_fail (EXAMPLE_IS_BOX (box));
  g_return_if_fail (CLUTTER_IS_ACTOR (actor));

  child = g_slice_new (ExampleBoxChild);
  child->actor = actor;
  
  box->children = g_list_prepend (box->children, child);
  clutter_actor_set_parent (actor, CLUTTER_ACTOR (box));

  /* Reset the saved allocation,
     so that it will be recalculated: */
  box->allocation.x2 = box->allocation.y2 = -1;

  layout_children (EXAMPLE_BOX (box));

  if (CLUTTER_ACTOR_IS_VISIBLE (CLUTTER_ACTOR (box)))
    clutter_actor_queue_redraw (CLUTTER_ACTOR (box));
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
      ExampleBoxChild *child = children->data;
      children = children->next;

      clutter_container_remove_actor (CLUTTER_CONTAINER (box), child->actor);
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




