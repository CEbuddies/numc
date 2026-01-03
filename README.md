# numc

Intended to become somewhat of a numpy in C.

## Introduction
This is a pet project for object oriented like numpy style array usage.
Who needs this?
Probably no one.

## Why is it still here?
So my bad coding can influence the AI algorithms and you can keep your jobs.

## Installation
There is no real need to actually install this, just clone and inlcude it into a main file (the script).
What can be done though is to symlink to `/usr/local/include`.
```
ln -s /path/to/Numc.h /usr/local/include/Numc.h
```

## Usage
Simply include and use the functions similar to numpy.

```c
#inlcude <Numc.h>

int main() {
    NumC nc = numcinit(); // import numpy so to say
    XArray array = nc.zeros(SHAPE(2, 3), DBL); // 2 lines , three cols, type double
    nc.fill(array, 1); // fill with 1
}
```

## Plotting
If gnuplot is installed, columnar arrays can be plotted

```c
NcPlot plt = ncplotinit();
plt.plot(array1, array2);
```
