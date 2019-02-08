#!/home/jmmoon/anaconda2/bin/python

##################

import os, sys

from contextlib import closing
from paramiko import SSHConfig, SSHClient
import numpy
import io, json, time

		



PROJ_PATH = "/home/jmmoon/Projects/FileManager/"

node_number = int(sys.argv[1])

with open(PROJ_PATH+'information/options.json') as json_file:
	options = json.load(json_file)

with open(PROJ_PATH+'information/data.json') as json_file:
	targets = json.load(json_file)

with open(PROJ_PATH+'information/node_size.json') as json_file:
	node_size_array = json.load(json_file)


index_array = numpy.zeros(int(options['node'])+1, dtype=numpy.int32)

for i in range(int(options['node'])+1):
	for j in range(i):
		index_array[i] += node_size_array[j]





if not os.path.isdir(options['emerald']):
	os.makedirs(options['emerald'])


os.chdir(options['emerald'])


# print len(os.walk(options['emerald']).next()[2]), node_size_array[node_number]

while len(os.walk(options['emerald']).next()[2]) < int(node_size_array[node_number]):
	print "Emerald_" + str(node_number+1)
	# break
	# if True:
	try:
		with closing(SSHClient()) as ssh:
			ssh.load_system_host_keys() #NOTE: no AutoAddPolicy() 
			ssh.connect(options['ip'], username=options['user'], password=options['password'], port=options['port'])
			print "Emerald_" + str(node_number+1) + ": connected"
			with closing(ssh.open_sftp()) as sftp:
				for i in range(index_array[node_number], index_array[node_number+1]):
					if os.path.isfile(str('{0:05}'.format(i)) + "." + options['file_type']):
						if sftp.stat(targets[str(i)]).st_size == os.path.getsize(str('{0:05}'.format(i)) + "." + options['file_type']):
							continue
					sftp.get(targets[str(i)], str('{0:05}'.format(i)) + "." + options['file_type'])						

	except:
		# print 
		print "Reconnect"

