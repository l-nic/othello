#!/usr/bin/env python

import sys, os
import matplotlib
import matplotlib.pyplot as plt
import argparse
import numpy as np

def plot_stats(*files):
    # parse the logged pcap files
    log_stats = []
    for f in files:
        stats = {}
        stats['label'] = os.path.basename(f).replace('.plotme', '')
        stats['duration'] = parse_file(f)
        log_stats.append(stats)

    print 'Creating plots ...'

    # plot CDF of durations
    f1 = plt.figure()
    for stats in log_stats:
        plot_cdf(stats['duration'], stats['label'])
    plt.title("CDF of Othello Search Durations on Single Core")
    plt.xlabel("Duration (ns)")
    plt.ylabel("CDF")
    plt.grid()
    plt.legend(loc='lower right')
    ax = plt.gca()
    ax.autoscale()

    # print stats
    for stats in log_stats:
        print '{} Statistics:'.format(stats['label'])
        print '\t99% = {}'.format(np.percentile(stats['duration'], 99))
        print '\t50% = {}'.format(np.percentile(stats['duration'], 50))

def plot_cdf(data, label):
    sortData = np.sort(data)
    yvals = np.arange(len(sortData))/float(len(sortData))
    plt.plot(sortData, yvals, label=label, linestyle='-', marker='o')

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
    parser.add_argument('--files', nargs='+', help='<Required> Files that contains samples to plot', required=True)
    args = parser.parse_args()

    plot_stats(*args.files)

    font = {'family' : 'normal',
            'weight' : 'bold',
            'size'   : 32}
    matplotlib.rc('font', **font)
    plt.show()

if __name__ == '__main__':
    main()

