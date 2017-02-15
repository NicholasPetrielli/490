import os
import sys

f = open("whatToRun", 'r')

for line in f:
	os.system("espeak -f'" + str(line).strip('\n').strip(' ') + "' -s 135 2>/dev/null")
#	print "espeak -f '" + str(line).strip('\n').strip(' ') + "' -s 135 2>/dev/null" 
#	print line.strip('\n')
