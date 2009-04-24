include $(top_srcdir)/Makefile_web.am_fragment

SUBDIRS = tutorial

EXTRA_DIST = index.html

post-html-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) post-html); \
	done

post-html-local: index.html
	scp $^ $$USER@$(web_host):$(web_path_docs)

post-html: post-html-local post-html-recursive

doc-clean-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) doc-clean); \
	done

doc-clean: doc-clean-recursive

doc-rebuild: doc-clean all
