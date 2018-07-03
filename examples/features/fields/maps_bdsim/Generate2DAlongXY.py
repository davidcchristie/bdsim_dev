#!/usr/bin/env python2.7

import matplotlib.pyplot as _plt
import numpy as _np
import pybdsim
import tarfile

def main():
    # generate x and y points along their own axes
    x = _np.arange(-30, 30+1e-6, 5)
    z = _np.arange(-25, 25+1e-6, 1)
    
    # define functions for each field component
    amplitude = 0.5
    length    = 0.6 #m
    period    = 0.1 #m
    ncells    = length / period
    zStep     = length / 100.
    def fy(x,z):
        return (1 + 0.025*x**2)*_np.sin(2*_np.pi * z / period)
    
    data = []
    # loop over and build up 3d lists of lists of lists
    for xi in x:
        v = []
        for zi in z:
            v.append([xi,zi,0,fy(xi,zi),0])
        data.append(v)

    # convert to numpy array
    data = _np.array(data)
    
    # construct a BDSIM format field object and write it out
    f = pybdsim.Field.Field2D(data, secondColumn='Z')
    f.Write('2dexample-along-xz.dat')

    # compress the result
    tar = tarfile.open("2dexample-along-xz.tar.gz", "w:gz")
    tar.add('2dexample-along-xz.dat')
    tar.close()

    #Plot(data)
    return data

def Plot(array):

    a = array #shortcut
    mag = _np.sqrt(a[:,:,2]**2 + a[:,:,3]**2)

    _plt.figure()
    _plt.quiver(a[:,:,0], a[:,:,1], a[:,:,2], a[:,:,3], mag, cmap=_plt.cm.magma, pivot='mid', scale=None)
    _plt.xlabel('X (cm)')
    _plt.ylabel('Y (cm)')
    _plt.title('2D Example Data')
    _plt.colorbar(label='Tesla')
    _plt.tight_layout()
    _plt.axes().set_aspect('equal', 'datalim')
    _plt.savefig('2dexample.png',dpi=400)
    
if __name__ == "__main__":
    main()
