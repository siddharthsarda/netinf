NETINF
=============
This is a working version of the NETINF algorithm downloaded from http://snap.stanford.edu/netinf/. The original code has some issues and does not compile correctly. This version does. The original code does not even run with the example files given. I have changed this one to work. Or at least the basic version.

Usage
-------
Input Format: The input file to NETINF, with information about the cascades, should have two blocks separated by a blank line. 
Each line in the first block contains the id and name of a site: 
id,name
Each line in the second block contains information about one cascade:
id,timestamp,id,timestamp....

You can run the code then using ./netinf -i:example-cascades.txt
