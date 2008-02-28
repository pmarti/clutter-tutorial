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

#ifndef __EXAMPLE_SCROLLING_CONTAINER_H__
#define __EXAMPLE_SCROLLING_CONTAINER_H__

#include <clutter/clutter-actor.h>
#include <clutter/clutter-types.h>

G_BEGIN_DECLS

#define EXAMPLE_TYPE_SCROLLING_CONTAINER                (example_scrolling_container_get_type ())
#define EXAMPLE_SCROLLING_CONTAINER(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), EXAMPLE_TYPE_SCROLLING_CONTAINER, ExampleScrollingContainer))
#define EXAMPLE_IS_SCROLLING_CONTAINER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EXAMPLE_TYPE_SCROLLING_CONTAINER))
#define EXAMPLE_SCROLLING_CONTAINER_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), EXAMPLE_TYPE_SCROLLING_CONTAINER, ExampleScrollingContainerClass))
#define EXAMPLE_IS_SCROLLING_CONTAINER_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), EXAMPLE_TYPE_SCROLLING_CONTAINER))
#define EXAMPLE_SCROLLING_CONTAINER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), EXAMPLE_TYPE_SCROLLING_CONTAINER, ExampleScrollingContainerClass))


typedef struct _ExampleScrollingContainerChild         ExampleScrollingContainerChild;
typedef struct _ExampleScrollingContainer              ExampleScrollingContainer; 
typedef struct _ExampleScrollingContainerClass         ExampleScrollingContainerClass;

struct _ExampleScrollingContainer
{
  /*< private >*/
  ClutterActor parent_instance;

  /* Allocation of the container: */
  ClutterActorBox allocation;

  /* List of ExampleScrollingContainerChild structures */
  GList *children;
  
  /* All the child actors are in this group: */
  ClutterActor *group;
  gint offset;

  /* A rectange to show the bounds: */
  ClutterActor *rect;
};

struct _ExampleScrollingContainerClass
{
  /*< private >*/
  ClutterActorClass parent_class;
};


GType example_scrolling_container_get_type (void) G_GNUC_CONST;

ClutterActor *example_scrolling_container_new (void);
void example_scrolling_container_pack (ExampleScrollingContainer *self, ClutterActor *actor);
void example_scrolling_container_remove_all (ExampleScrollingContainer *self);

void example_scrolling_container_scroll_left (ExampleScrollingContainer *self, gint distance);

G_END_DECLS

#endif /* __EXAMPLE_SCROLLING_CONTAINER_H__ */
