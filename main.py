import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

def plot(name):
    arr = np.fromfile(f'{name}.arr', dtype=np.float32)
    covar = np.fromfile(f'{name}.covar', dtype=np.float32)

    fig, axs = plt.subplots(figsize=(8,4), nrows=1, ncols=2)
    axs[0].hist(arr, bins=20, histtype='step')
    axs[1].plot(covar, 'o')
    axs[1].axhline(y=1./12, color='red', linestyle='--')
    fig.savefig(f'{name}.png')

def plot3(n1,n2,n3):
    a1 = np.fromfile(f'{n1}.arr', dtype=np.float32)
    a2 = np.fromfile(f'{n2}.arr', dtype=np.float32)
    a3 = np.fromfile(f'{n3}.arr', dtype=np.float32)

    fig = plt.figure(figsize=(8,5), dpi=100)
    ax = fig.add_axes([0.1,0.1,0.9,0.9], projection='3d')
    # https://stackoverflow.com/questions/14674885/how-to-turn-off-transparency-in-matplotlibs-3d-scatter-plot
    ax.scatter3D(a1,a2,a3,c=None, depthshade=False)

    fig.savefig(f'{n1}-{n2}-{n3}_3d.png')

plot('good1')
plot('good2')
plot('good3')
plot3('good1','good2','good3')
plot('lcr1')
plot('lcr2')
plot('lcr3')
plot3('lcr1','lcr2','lcr3')
