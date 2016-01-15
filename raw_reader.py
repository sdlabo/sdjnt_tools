import struct
import sys

if (len(sys.argv) != 2):
    print "usage"
    quit()

fp = open(sys.argv[1], mode='rb')

for i in range(0, 2**21):
    d1re = struct.unpack('<d', fp.read(8)) # double little-endian
    d1im = struct.unpack('<d', fp.read(8)) # double little-endian
    print "%d,%f,%f" % (i, d1re[0], d1im[0])
