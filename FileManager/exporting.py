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


my_name_list = []

def search(aDirname, aType):
	filelist = []
	filenames = os.listdir(aDirname)
	for filename in filenames:
		full_filename = os.path.join(aDirname, filename)
		ext = os.path.splitext(full_filename)[-1]
		if ext == aType:
			filelist.append(full_filename)
	return filelist

my_name_list = search(options['emerald'], ".raw")
my_name_list.sort()






# print len(os.walk(options['emerald']).next()[2]), node_size_array[node_number]

print "Emerald_" + str(node_number+1)
# break
# if True:
try:
	with closing(SSHClient()) as ssh:
		ssh.load_system_host_keys() #NOTE: no AutoAddPolicy() 
		ssh.connect(options['ip'], username=options['user'], password=options['password'], port=options['port'])
		with closing(ssh.open_sftp()) as sftp:
			print "Emerald_" + str(node_number+1) + ": connected"
			for p in options['path']:
				print p
				for ipath in my_name_list:
					file_name = ipath.split(options['emerald'])[-1]
					# print file_name
					try:
						stat = sftp.stat(p+file_name)
						print "file exist"
						if stat.st_size != os.path.getsize(ipath):
							print "size diff uploading"
							sftp.put(ipath, file_name)
					except IOError:
						print "uploading"
						sftp.put(ipath, p+file_name)
except:
	# print 
	print "Reconnect"

