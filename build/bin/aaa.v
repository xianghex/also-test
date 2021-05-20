INPUT(n1)
INPUT(n2)
INPUT(n3)
OUTPUT(po0)
n0 = gnd
n4 = LUT 0x8e (n1, n2, n3)
n5 = LUT 0xe8 (n0, n3, n4)
po0 = LUT 0x2 (n5)
