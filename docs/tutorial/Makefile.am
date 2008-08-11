include $(top_srcdir)/Makefile_web.am_fragment
clutter_tut_path = $(web_path_docs)tutorial

SUBDIRS = icons figures

DOCBOOK_PHPWEBNOTES_TRANSFORM = docbook_phpwebnotes.xsl

EXTRA_DIST = README insert_example_code.pl clutter-tut.xml clutter-tut-with-examples.xml style.css html $(DOCBOOK_PHPWEBNOTES_TRANSFORM)

DOCBOOK_STYLESHEET ?= http://docbook.sourceforge.net/release/xsl/current/xhtml/chunk.xsl

# Create a DocBook source file that doesn't have the examples' comments blocks:
clutter-tut-with-examples.xml: clutter-tut.xml insert_example_code.pl
	$(PERL_PATH) $(srcdir)/insert_example_code.pl $(top_srcdir)/examples $< >$@

# The new XML DocBook way:
XSLTPROC = xsltproc
html/index.html: clutter-tut-with-examples.xml
	rm -rf html
	$(mkinstalldirs) html
	$(XSLTPROC) \
		--param toc.section.depth 1	\
		--stringparam html.stylesheet "style.css"	\
		--stringparam admon.graphics 1	\
		--stringparam admon.graphics.path "../icons/"	\
		--stringparam admon.graphics.extension ".png"	\
		--stringparam chunker.output.indent yes	\
		--stringparam chunker.output.encoding UTF-8	\
		--stringparam navig.graphics yes	\
		--stringparam navig.graphics.extension ".png"	\
		--stringparam navig.graphics.path "../icons/"	\
		--stringparam toc.list.type "ul"	\
		--stringparam use.id.as.filename "1" \
		-o html/ --xinclude --catalogs $(DOCBOOK_STYLESHEET)	\
		$<

validate_original: clutter-tut.xml
	xmllint --xinclude --valid --noout --catalogs $(DOCBOOK_STYLESHEET) $<

validate: clutter-tut-with-examples.xml
	xmllint --xinclude --valid --noout --catalogs $(DOCBOOK_STYLESHEET) $<


post-html: html/index.html style.css pdf
	rsync $(rsync_args) figures/*.png $$USER@$(web_host):$(clutter_tut_path)/figures/
	rsync $(rsync_args) icons/*.png $$USER@$(web_host):$(clutter_tut_path)/icons/
	rsync $(rsync_args) -r html/ $$USER@$(web_host):$(clutter_tut_path)/html/
	rsync $(rsync_args) *.css $$USER@$(web_host):$(clutter_tut_path)/html/
	rsync $(rsync_args) -r pdf/ $$USER@$(web_host):$(clutter_tut_path)/pdf/


# PDF Generation:

XMLLINT = xmllint
DB2PDF = docbook2pdf

# we need to produce a version with full examples with all of the XIncludes done so that it
# can processed for PDF
programming-with-clutter.xml: clutter-tut-with-examples.xml
	rm -rf html
	$(mkinstalldirs) html
	$(XMLLINT) --xinclude --postvalid $< -o $@

# We have to generate the pdf in a subdirectory (e.g. pdf/) because the tutorial
# specifies the path to the figures as '../figures' so if we build it in this
# directory, it won't find the images.
pdf/programming-with-clutter.pdf: programming-with-clutter.xml
	$(DB2PDF) --output pdf $<

pdf : pdf/programming-with-clutter.pdf



doc-clean:
	-rm -rf html
	-rm -rf html_withcomments
	-rm -f clutter-tut-with-examples.xml
	-rm -r pdf

tutorialdir = $(clutter_docdir)/tutorial/html

install-tutorial: html/index.html style.css
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(tutorialdir)
	@dir='$(<D)'; for p in $$dir/*.html ; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$p $(DESTDIR)$(tutorialdir)/$$f"; \
	  $(INSTALL_DATA) $$p $(DESTDIR)$(tutorialdir)/$$f; \
	done
	$(INSTALL_DATA) $(srcdir)/style.css $(DESTDIR)$(tutorialdir)

uninstall-tutorial: html/index.html style.css
	@$(NORMAL_UNINSTALL)
	@dir='$(<D)'; for p in $$dir/*.html ; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(tutorialdir)/$$f"; \
	  rm -f $(DESTDIR)$(tutorialdir)/$$f; \
	done
	rm -f $(DESTDIR)$(tutorialdir)/style.css
	rm -f $(DESTDIR)$(tutorialdir)/icons

all-local: html/index.html

install-data-local: install-tutorial

uninstall-local: uninstall-tutorial

maintainer-clean-local: doc-clean

.PHONY: post-html doc-clean install-tutorial uninstall-tutorial
