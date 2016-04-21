#!/usr/bin/python3

import sys, random

try:
	col_count = int(sys.argv[1])
	row_count = int(sys.argv[2])
	
	if col_count <= 0 or row_count <= 0:
		raise ValueError()

except (IndexError, ValueError):
	print("Usage: %s col_count row_count" % sys.argv[0], file=sys.stderr)
	sys.exit(1)

columns = [ random.sample(range(10 * row_count), row_count) for _ in range(col_count) ]

for row in range(row_count):
	print(', '.join(str(columns[col][row]) for col in range(col_count) ))

