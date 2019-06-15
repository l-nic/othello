#!/usr/bin/env python

import sys, os
import matplotlib
import matplotlib.pyplot as plt
import argparse
import numpy as np

def plot_cdf(data):
    sortData = np.sort(data)
    yvals = np.arange(len(sortData))/float(len(sortData))
    plt.plot(sortData, yvals, linestyle='-', marker='o')

def parse_file(filename):
    data = []
    with open(filename) as f:
        for line in f:
            try:
                data.append(float(line))
            except:
                print "ERROR: invalid line in file: {}".format(line)
    return data

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, help='<Required> File that contains samples to plot', required=True)
    args = parser.parse_args()

    data = parse_file(args.file)

    # plot the data 
    plot_cdf(data)
    plt.title("Search duration CDF")
    plt.xlabel("Duration (ns)")
    plt.ylabel("CDF")

    font = {'family' : 'normal',
            'weight' : 'bold',
            'size'   : 32}
    matplotlib.rc('font', **font)
    plt.show()

if __name__ == '__main__':
    main()

