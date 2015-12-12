#!/usr/bin/python

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import Serpent


serpent = Serpent.Serpent.CreateFromPath('serpent.path')
coordinates = serpent.getBlocksPos()

xVals = []
yVals = []
zVals = []

for i in range(coordinates.getSize()):
    xVals.append(coordinates.getXVal(i))
    yVals.append(coordinates.getYVal(i))
    zVals.append(coordinates.getZVal(i))

figure = plt.figure()
axes = figure.add_subplot(111, projection='3d')

axes.plot(xVals, yVals, zVals)

figure.savefig('serpent.png')

