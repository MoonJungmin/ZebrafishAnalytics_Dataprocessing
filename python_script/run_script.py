import os, sys
import argparse
from contextlib import closing
from paramiko import SSHConfig, SSHClient
import io, json
import subprocess
import time


PROJ_PATH = "/home/jmmoon/Projects/python_script/"
IMPORTING_EXE = "rename_to_physical_index.py"



pids = []

class bcolors:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OKGREEN = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'


print "  =>  " + bcolors.HEADER + "SSH Request " + bcolors.HEADER + "start" + bcolors.ENDC

for i in range(16):
	time.sleep(1)
	p = subprocess.Popen("ssh ib" + str('{0:02}'.format(i+1)) + " \"" + PROJ_PATH + IMPORTING_EXE + " " + str(i) +"\"", stdin=None,stdout=None, shell=True)
	pids.insert(0, p)

print "  =>  " + bcolors.HEADER + "SSH Request " + bcolors.HEADER + "done" + bcolors.ENDC

for i in range(16):
	pids[i].wait()

print "  =>  " + bcolors.HEADER + "All Process is " + bcolors.HEADER + "done" + bcolors.ENDC
