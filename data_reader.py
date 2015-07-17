import struct

fp = open("./sdlab_20150718_012717_00000000_5.data", mode='rb')

x_re = []
x_im = []
p_a = []
p_b = []

for i in range(0, 2**20):
    d = struct.unpack('<d', fp.read(8)) # unsigned long, little-endian
    x_re.append(d)

for i in range(0, 2**20):
    d = struct.unpack('<d', fp.read(8)) # unsigned long, little-endian
    x_im.append(d)

for i in range(0, 2**20):
    d = struct.unpack('<d', fp.read(8)) # unsigned long, little-endian
    p_a.append(d)

for i in range(0, 2**20):
    d = struct.unpack('<d', fp.read(8)) # unsigned long, little-endian
    p_b.append(d)

for i in range(0, 1000):
    print "%s" % p_a[i]

