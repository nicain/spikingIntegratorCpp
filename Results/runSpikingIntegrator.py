#!/usr/bin/env python
#
# runSpikingIntegrator.py
# Created by nicain on 9/14/11.

################################################################################
################################## Preamble ####################################
################################################################################

# Import Packages:
import sys
import argparse
import time
from subprocess import call as call

maxTDefault = 500

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
parser.add_argument('-p',
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

args = parser.parse_args(sys.argv[1:])

################################################################################
#################################### Run #######################################
################################################################################
	
executable = "../build/Debug/spikingIntegrator"
	
tBegin = time.mktime(time.localtime())

callString = executable
for arg in [args.C, args.tOn, args.tOff, args.tMax, args.correlation,
			args.saveResults,  
			args.recordBGSpikes]:
	callString += " " + str(arg)

call(callString, shell=True)

print callString
		   
tEnd = time.mktime(time.localtime())
print 'Total Computation Time: ', time.strftime("H:%H M:%M S:%S",time.gmtime(tEnd - tBegin))
