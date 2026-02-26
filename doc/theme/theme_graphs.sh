#!/bin/sh
#───────────────────────────────────────────────
#  Usage: sh doc/theme/theme_graphs.sh doc/html

DIR="${1:-doc/html}"

THEME_CSS='/* -- MiniDEVIL graph theme -- */
.node polygon, .node ellipse { fill: #1e1e24 !important; stroke: #3a1e22 !important; stroke-width: 1.5 !important; }
.node text { fill: #d5d2cb !important; font-family: "Fira Code","Consolas",monospace !important; }
.node a text { fill: #e0654a !important; }
g#node1 polygon { fill: #2a1a20 !important; stroke: #9b1b30 !important; }
g#node1 text { fill: #d5d2cb !important; }
.edge path { stroke: #605d56 !important; }
.edge polygon { fill: #605d56 !important; stroke: #605d56 !important; }
.edge text { fill: #908d85 !important; }
.edge:hover path { stroke: #e0654a !important; }
.edge:hover polygon { stroke: #e0654a !important; fill: #e0654a !important; }'

COUNT=0
for svg in "$DIR"/*.svg; do
	[ -f "$svg" ] || continue
	# skip if already themed
	grep -q 'MiniDEVIL graph theme' "$svg" && continue

	if grep -q ']]></style>' "$svg"; then
		# interactive SVG
		awk -v css="$THEME_CSS" '
		/\]\]><\/style>/ { print css }
		{ print }
		' "$svg" > "$svg.tmp" && mv "$svg.tmp" "$svg"
	else
		# non interactive SVG
		awk -v css="$THEME_CSS" '
		/<g id="graph0"/ && !done {
			print "<defs><style type=\"text/css\">"
			print css
			print "</style></defs>"
			done = 1
		}
		{ print }
		' "$svg" > "$svg.tmp" && mv "$svg.tmp" "$svg"
	fi
	COUNT=$((COUNT + 1))
done

printf "  themed %d SVG graph(s) in %s\n" "$COUNT" "$DIR"
