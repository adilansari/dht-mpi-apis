
all: htmls
# phps pdfs

# phps: userguide.php

htmls: manual.html

# pdfs: userguide.pdf

# chunked-userguide: userguide.xml
#	./build-chunked-userguide.sh

manual.inc.xml: manual.xml
	xmllint --xinclude $(<) > $(@)
	sed -i "s/INSERT_DATE/`date`/" $(@)

index.html: manual.inc.xml formatting/swiftsh_html.xsl
	xsltproc --nonet formatting/swiftsh_html.xsl $(<)
	chmod a+r $(@)

manual.html: index.html
	sed -e "s/index.html#/#/g" $(<) > $(@)
	chmod a+r $(@)

#%.pdf: %.xml formatting/vdl2_fo.xsl
#	fop/fop -xsl formatting/vdl2_fo.xsl -xml $< -pdf $@

%.html: %.php
	cp $< $@

clean:
	rm -fv index.html manual.html manual.inc.xml

