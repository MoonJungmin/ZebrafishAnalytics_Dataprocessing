import os, sys
import argparse
from contextlib import closing
from paramiko import SSHConfig, SSHClient
import io, json
import subprocess
import time

PROJ_PATH = "/home/jmmoon/Projects/FileManager/"
IMPORTING_EXE = "importing.py"
EXPORTING_EXE = "exporting.py"

opts = {}
#Path to a emerald's node directory.

# /David/180706_affnt_180602_SWiFT_120nmpx_median2 
opts['emerald'] = "/scratch/jmmoon/Cell/Ganglia/raw_file/"

#The number of emerald nodes to use.
opts['node'] = 16

#File distribute type and size( manual : 1 or default : 0)
opts['distribute_type'] = 1
opts['distribute_size'] = 512
#Remainder contorl (throw out : 0 , attach last node : 1)
opts['distribute_remainder'] = 1

#Address to a destination or source directory.
opts['ip'] = "10.20.12.52"

#Path to a destination or source directory.
opts['path'] = ['/home/SharedHDD/Cell/Ganglia/labeled_rawfile/']

#Filetype.
opts['file_type'] = "raw"

#Exporting data from emerald nodes:0, Importing data from source path:1
opts['type'] = 1

#User account for destination or source server
opts['user'] = "jmmoon"

#User password for destination or source server
opts['password'] = "132435a"

#SSH port for destination or source server
opts['port'] = 22






class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def directory_check(path):
    if not os.path.isdir(path):
        err_msg = "path is not a directory (%s)"
        raise argparse.ArgumentTypeError(err_msg)
    return path

def file_check(path):
    if not os.path.isfile(path):
        err_msg = "path is not a file (%s)"
        raise argparse.ArgumentTypeError(err_msg)
    return path

def yes_or_no_choice(message, default='y'):
	choices = 'Y/n' if default.lower() in ('y', 'yes') else 'y/N'
	choice = raw_input("%s (%s) " % (message, choices))
	values = ('y', 'yes', '') if choices == 'Y/n' else ('y', 'yes')
	return choice.strip().lower() in values


def generate_each_node_size(options, whole_size):
	node_size = whole_size / options['node']
	extra = whole_size % options['node']
	size_arr = []

	if options['distribute_type'] == 0:
		for i in range(options['node']):
			if i+1 == options['node']:
				size_arr.append(node_size+extra)
			else:
				size_arr.append(node_size)

	else:
		full_size = whole_size
		for i in range(options['node']):
			if full_size >= options['distribute_size']:
				size_arr.append(options['distribute_size'])
				full_size -= options['distribute_size']

			elif full_size < options['distribute_size']:
				size_arr.append(full_size)
				full_size = 0

		if options['distribute_remainder'] == 1 and full_size > 0:
			size_arr[-1] += full_size

	return size_arr


def print_cluster(options, whole_size, node_size_array):
	tab_size = len(str(whole_size))
	print "\n\t",	
	for i in range(0, options['node']/2): 
		print bcolors.BOLD + str(i+1).center(tab_size) + bcolors.ENDC,
	
	print "\n\t",
	for i in range(0, options['node']/2):
		print str(node_size_array[i]).center(tab_size),
	
	print "\n\n\t",
	for i in range(options['node']/2, options['node']): 
		print bcolors.BOLD + str(i+1).center(tab_size) + bcolors.ENDC,
	
	print "\n\t",
	
	for i in range(options['node']/2, options['node']):
		print str(node_size_array[i]).center(tab_size),
	
	print "\n"



def Importing(options):
	print "\n=>  " + bcolors.BOLD + "Importing start" + bcolors.ENDC


	TargeFileList = {}
	targetRawList = []
	with closing(SSHClient()) as ssh:
		ssh.load_system_host_keys() #NOTE: no AutoAddPolicy() 
		ssh.connect(options['ip'], port=options['port'], username=options['user'], password=options['password'])
		with closing(ssh.open_sftp()) as sftp:
			for p in options['path']:
				sftp.chdir(p)

				for filename in sftp.listdir():
					if filename[0] != ".":
						# v = int(filename.split("v")[-1].split("_")[0])
						# s = int(filename.split("s")[-1].split( "." + options['file_type'])[0])
						targetRawList.append(p + filename)
				

	targetRawList.sort()
	index = 0
	for t in targetRawList:
		TargeFileList[index] = t
		index += 1

	size = len(TargeFileList)
	node_size = size / options['node']
	extra = size % options['node']


	print "=>  " + bcolors.OKGREEN + options['ip'] + bcolors.ENDC + " has " + bcolors.WARNING +str(size) + bcolors.ENDC + " files.." + bcolors.ENDC
	print "=>  " + bcolors.WARNING +str(size) + bcolors.ENDC + " files will be distribute at " + bcolors.WARNING + str(options['node']) + bcolors.ENDC + " nodes (" + bcolors.OKGREEN + str(node_size) + bcolors.ENDC + " per node, remainder is " + bcolors.OKGREEN + str(extra) + bcolors.ENDC + ")"  

	if options['distribute_type'] == 0:
		print "=>  " + bcolors.ENDC + "File distribute type is " + bcolors.OKGREEN + "Defaults setting" + bcolors.ENDC
	elif options['distribute_type'] == 1:
		print "=>  " + bcolors.ENDC + "File distribute type is " + bcolors.OKGREEN + "Manual setting" + bcolors.ENDC

	for p in options['path']:
		print "=>  " + bcolors.ENDC + "Source path : " + bcolors.OKBLUE + options['ip'] + ":" + p + bcolors.ENDC

	print "=>  " + bcolors.ENDC + "Destination path : " + bcolors.OKBLUE + options['emerald'] + bcolors.ENDC


	node_size_array = generate_each_node_size(opts, size)
	print_cluster(opts, size, node_size_array)

	proceed_flag = yes_or_no_choice("=>  Proceed file transfer? ")
	

	if proceed_flag == True:
		with io.open('./information/data.json', 'w') as f1:
			f1.write(json.dumps(TargeFileList, ensure_ascii=False))

		with io.open('./information/options.json', 'w') as f2:
			f2.write(unicode(json.dumps(options, ensure_ascii=False)))

		with io.open('./information/node_size.json', 'w') as f2:
			f2.write(unicode(json.dumps(node_size_array, ensure_ascii=False)))

		
		pids = []

		# i = 14
		print "  =>  " + bcolors.HEADER + "SSH Request " + bcolors.HEADER + "start" + bcolors.ENDC

		
		for i in range(options['node']):
			time.sleep(1)
			p = subprocess.Popen("ssh ib" + str('{0:02}'.format(i+1)) + " \"" + PROJ_PATH + IMPORTING_EXE + " " + str(i) +"\"", stdin=None,stdout=None, shell=True)
			pids.insert(0, p)
		
		print "  =>  " + bcolors.HEADER + "SSH Request " + bcolors.HEADER + "done" + bcolors.ENDC

		for i in range(options['node']):
			pids[i].wait()

		print "  =>  " + bcolors.HEADER + "All Process is " + bcolors.HEADER + "done" + bcolors.ENDC

	else:

		return



def Exporting(options):
	print "\n=>  " + bcolors.BOLD + "Exporting start" + bcolors.ENDC
	
	for p in options['path']:
		print "=>  " + bcolors.ENDC + "Destination path : " + bcolors.OKBLUE + options['ip'] + ":" + p + bcolors.ENDC

	print "=>  " + bcolors.ENDC + "Source path : " + bcolors.OKBLUE + options['emerald'] + bcolors.ENDC




	# print_cluster(opts, size, node_size_array)

	proceed_flag = yes_or_no_choice("=>  Proceed file transfer? ")
	

	if proceed_flag == True:
		
		pids = []

		with io.open('./information/options.json', 'w') as f2:
			f2.write(unicode(json.dumps(options, ensure_ascii=False)))


		# i = 14
		print "  =>  " + bcolors.HEADER + "SSH Request " + bcolors.HEADER + "start" + bcolors.ENDC

		
		for i in range(options['node']):
			time.sleep(1)
			p = subprocess.Popen("ssh ib" + str('{0:02}'.format(i+1)) + " \"" + PROJ_PATH + EXPORTING_EXE + " " + str(i) +"\"", stdin=None,stdout=None, shell=True)
			pids.insert(0, p)
		
		print "  =>  " + bcolors.HEADER + "SSH Request " + bcolors.HEADER + "done" + bcolors.ENDC

		for i in range(options['node']):
			pids[i].wait()

		print "  =>  " + bcolors.HEADER + "All Process is " + bcolors.HEADER + "done" + bcolors.ENDC

	else:

		return







if opts['type'] == 0:
	Exporting(opts)

elif opts['type'] == 1:
	Importing(opts)