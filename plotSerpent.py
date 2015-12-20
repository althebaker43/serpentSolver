#!/usr/bin/python

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import Serpent


def WriteSerpentImage(serpent, fileName):

    figure = GetSerpentFigure(serpent)
    figure.savefig(fileName)
    plt.close(figure)


def GetSerpentFigure(serpent):

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

    axes.set_xlim(left=-14, right=14)
    axes.set_ylim(bottom=-14, top=14)
    axes.set_zlim(bottom=-14, top=14)

    axes.plot(xVals, yVals, zVals)

    return figure


if (__name__ == '__main__'):

    serpent = Serpent.Serpent.CreateFromPath('serpent.path')
    serpent.openStepFile('procedure.steps')

    for step in range(65):
        WriteSerpentImage(serpent, 'img/step_' + str(step) + '.png')
        if (serpent.step() == False):
            break

    serpent.closeStepFile()

