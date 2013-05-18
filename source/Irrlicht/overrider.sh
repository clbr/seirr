#!/bin/sh

for file in "$@"; do

	tmp="tmp.$$"

	gawk 'BEGIN {
	  RS=";\n"
	  ORS=""
	}
	/virtual/ && ! /\{/ {
	    RT=" _IRR_OVERRIDE_" RT
	}
	{ print $0RT }
	' "$file" > $tmp

	mv $tmp "$file"

	sed -i '/virtual/s@{@_IRR_OVERRIDE_ {@' "$file"
done
