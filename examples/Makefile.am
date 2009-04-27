include $(top_srcdir)/Makefile_web.am_fragment

example_dirs = actor actor_events actor_group actor_transformations behaviour \
               custom_actor custom_container animation entry stage stage_widget \
               timeline score full_example \
               scrolling

# container - Disabled until the new higher-level library exists.

SUBDIRS = $(example_dirs)

EXTRA_DIST	=  Makefile.am_fragment

exclude_args = --delete-excluded --exclude *.o --exclude .libs --exclude .deps --exclude core --exclude .cvsignore --exclude a.out --exclude Makefile --exclude Makefile.in --exclude example --exclude .svn
post-html:
	rsync $(rsync_args) -a $(exclude_args) $(example_dirs) $$USER@$(web_host):$(web_path)examples


