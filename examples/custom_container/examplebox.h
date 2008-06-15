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

#ifndef __EXAMPLE_BOX_H__
#define __EXAMPLE_BOX_H__

#include <clutter/clutter-actor.h>
#include <clutter/clutter-types.h>

G_BEGIN_DECLS

#define EXAMPLE_TYPE_BOX                (example_box_get_type ())
#define EXAMPLE_BOX(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), EXAMPLE_TYPE_BOX, ExampleBox))
#define EXAMPLE_IS_BOX(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EXAMPLE_TYPE_BOX))
#define EXAMPLE_BOX_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), EXAMPLE_TYPE_BOX, ExampleBoxClass))
#define EXAMPLE_IS_BOX_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), EXAMPLE_TYPE_BOX))
#define EXAMPLE_BOX_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), EXAMPLE_TYPE_BOX, ExampleBoxClass))


typedef struct _ExampleBoxChild         ExampleBoxChild;
typedef struct _ExampleBox              ExampleBox; 
typedef struct _ExampleBoxClass         ExampleBoxClass;

struct _ExampleBox
{
  /*< private >*/
  ClutterActor parent_instance;

  /* List of ExampleBoxChild structures */
  GList *children;
};

struct _ExampleBoxClass
{
  /*< private >*/
  ClutterActorClass parent_class;
};


GType example_box_get_type (void) G_GNUC_CONST;

ClutterActor *example_box_new (void);
void example_box_pack (ExampleBox*box, ClutterActor *actor);
void example_box_remove_all (ExampleBox *box);

G_END_DECLS

#endif /* __EXAMPLE_BOX_H__ */
