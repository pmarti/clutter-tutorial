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

  /* Allocation of the box */
  ClutterActorBox allocation;

  /* List of ExampleBoxChild structures */
  GList *children;
};

/**
 * ExampleBoxClass:
 * @pack_child: virtual function, called when packing a new child. All
 *   #ExampleBox subclasses must implement this function
 * @unpack_child: virtual function, called when unpacking a child. All
 *   #ExampleBox subclasses must implement this function
 *
 * Base class for layout containers
 *
 * Since: 0.4
 */
struct _ExampleBoxClass
{
  /*< private >*/
  ClutterActorClass parent_class;
};


GType    example_box_get_type            (void) G_GNUC_CONST;

ClutterActor *example_box_new           (void);

void     example_box_pack                (ExampleBox           *box,
                                          ClutterActor         *actor);
void     example_box_remove_all          (ExampleBox           *box);

G_END_DECLS

#endif /* __EXAMPLE_BOX_H__ */
