#!/home/jmmoon/anaconda2/bin/python

##################

import os, sys
from contextlib import closing
from paramiko import SSHConfig, SSHClient
import numpy
import io, json, time
import platform

node_number = int(platform.node().split("emerald")[-1])

options = {}
options['ip'] = "diamond.unist.ac.kr"
options['user'] = "jmmoon"
options['password'] = "132435a"
options['port'] = 22

options['path_xy'] = "/home/SharedHDD/cell_block/XY/"
options['path_yz'] = "/home/SharedHDD/cell_block/YZ/"
options['path_zx'] = "/home/SharedHDD/cell_block/ZX/"


options['input_xy'] = "/scratch/jmmoon/Cell/Block/XY/"
options['input_yz'] = "/scratch/jmmoon/Cell/Block/YZ/"
options['input_zx'] = "/scratch/jmmoon/Cell/Block/ZX/"

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

my_name_list = search(options['input_xy'], ".block")
my_name_list.sort()

try:
	with closing(SSHClient()) as ssh:
		ssh.load_system_host_keys() #NOTE: no AutoAddPolicy() 
		ssh.connect(options['ip'], username=options['user'], password=options['password'], port=options['port'])
		with closing(ssh.open_sftp()) as sftp:
			p = options['path_xy']

			for ipath in my_name_list:
				file_name = ipath.split(options['input_xy'])[-1]
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



my_name_list = search(options['input_yz'], ".block")
my_name_list.sort()

try:
	with closing(SSHClient()) as ssh:
		ssh.load_system_host_keys() #NOTE: no AutoAddPolicy() 
		ssh.connect(options['ip'], username=options['user'], password=options['password'], port=options['port'])
		with closing(ssh.open_sftp()) as sftp:
			p = options['path_yz']

			for ipath in my_name_list:
				file_name = ipath.split(options['input_yz'])[-1]
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


my_name_list = search(options['input_zx'], ".block")
my_name_list.sort()

try:
	with closing(SSHClient()) as ssh:
		ssh.load_system_host_keys() #NOTE: no AutoAddPolicy() 
		ssh.connect(options['ip'], username=options['user'], password=options['password'], port=options['port'])
		with closing(ssh.open_sftp()) as sftp:
			p = options['path_zx']

			for ipath in my_name_list:
				file_name = ipath.split(options['input_zx'])[-1]
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
