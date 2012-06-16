#!/usr/bin/env python
#
# runSpikingIntegrator.py
# Created by nicain on 9/14/11.

################################################################################
################################## Preamble ####################################
################################################################################

# Import Packages:
import sys
import os
import argparse
import time
from subprocess import Popen as Popen
from subprocess import PIPE as PIPE
import compNeuroPython as CNP
import numpy as np

maxTDefault = 5000

# Parse input arguments:
parser = argparse.ArgumentParser(description='Spiking Integrator')


parser.add_argument('-C','--Coh',
                    default=0,
                    type=float,
                    action='store',
                    dest='C',
                    help='Dot coherence value')

parser.add_argument('--tOn',
                    default=0,
                    action='store',
                    type=float,
                    dest='tOn',
                    help='On time')
parser.add_argument('--tOff',
                    default=maxTDefault,
                    action='store',
                    type=float,
                    dest='tOff',
                    help='Off time')
parser.add_argument('--tMax',
                    default=maxTDefault,
                    action='store',
                    type=float,
                    dest='tMax',
                    help='Max time')
parser.add_argument('-corr',
                    default=0,
                    action='store',
                    type=float,
                    dest='correlation',
                    help='SCC of input pool')
parser.add_argument('-s','--saveResults',
                    default=0,
                    action='store_const',
                    const=1,
                    dest='saveResults',
                    help='Save result option')
parser.add_argument('--recordBGSpikes',
                    default=0,
                    action='store_const',
                    const=1,
                    dest='recordBGSpikes',
                    help='Record BG Spikes?')
parser.add_argument('--recordInputSpikes',
                    default=0,
                    action='store_const',
                    const=1,
                    dest='recordInputSpikes',
                    help='Record Input Spikes?')
parser.add_argument('--fr',
                    default=0,
                    action='store_const',
                    const=1,
                    dest='fr',
                    help='.ntf to .fr?')
parser.add_argument('--thresholdTest',
                    default=0,
                    action='store_const',
                    const=1,
                    dest='thresholdTest',
                    help='Perform threshold test?')

args = parser.parse_args(sys.argv[1:])

################################################################################
#################################### Run #######################################
################################################################################
    
executable = "./spikingIntegrator"
    
tBegin = time.mktime(time.localtime())

callString = executable
for arg in [args.C, args.tOn, args.tOff, args.tMax, args.correlation,
            args.saveResults, args.recordBGSpikes, args.recordInputSpikes]:
    callString += " " + str(arg)


process = Popen(callString.split(), shell=False, stdout=PIPE)
currUUID = process.communicate()[0].strip().split("\n")[0]

print currUUID

if args.fr == 1:
    fileNamePrefixList = ["GESel1","GESel2"]
    for fileNamePrefix in fileNamePrefixList:
        fileNamePrefix += "_" + currUUID
        for setting in callString.split()[1:]:
            fileNamePrefix += "_" + setting
        
        CNP.ntfToFRFile(fileNamePrefix + ".ntf")

if args.thresholdTest == 1:
    CNP.thresholdTestUUID(currUUID, list(np.linspace(.1,40,5000)),tOn=args.tOn-50)
    CNP.thresholdTestUUIDDiff(currUUID, list(np.linspace(.1,40,5000)),tOn=args.tOn-50)
    CNP.thresholdTestCurrentUUID(currUUID, list(np.linspace(.00001,.02,5000)),tOn=args.tOn)
    CNP.thresholdTestSpikesBGTooUUID(currUUID, list(np.linspace(1,7000,5000)),tOn=args.tOn, beta=0)
    CNP.splitCurrentAnalysisBG(currUUID, list(np.linspace(1,.02,5000)))
    CNP.splitCurrentAnalysisAll(currUUID, list(np.linspace(1,.2,5000)))

    BGESel1FileName = CNP.findFileName([currUUID, ".ntf", "BGESel1"])[0]
    BGESel2FileName = CNP.findFileName([currUUID, ".ntf", "BGESel2"])[0]

    os.remove(BGESel1FileName)
    os.remove(BGESel2FileName)

           
tEnd = time.mktime(time.localtime())
print 'Total Computation Time: ', time.strftime("H:%H M:%M S:%S",time.gmtime(tEnd - tBegin))
