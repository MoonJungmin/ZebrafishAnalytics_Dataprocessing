#!/home/jmmoon/anaconda2/bin/python
import os, sys
import argparse
from contextlib import closing
from paramiko import SSHConfig, SSHClient
import io, json
import subprocess
import time


options = {}
options['ip'] = "moonstone.unist.ac.kr"

options['path'] = ['/David/180602_median2_160515_SWiFT_60nmpx_singles/']
options['emerald'] = "/scratch/jmmoon/180602_median2_160515_SWiFT_120nmpx_singles_distribute/"

options['file_type'] = "png"
options['user'] = "hvcl"
options['password'] = "vis@123"
options['port'] = 17771

out_root_path = "/scratch/jmmoon/180602_median2_160515_SWiFT_120nmpx_singles_distribute_rename/"


# TargeFileList = {}
orig_name_list = []
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
					orig_name_list.append(p + filename)
			
orig_name_list.sort()



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

my_name_list = search(options['emerald'], ".png")
my_name_list.sort()


if not os.path.isdir(out_root_path):
	os.makedirs(out_root_path)


for p in my_name_list:
	number = int(p.split(options['emerald'])[-1].split(".png")[0])
	phys_number = orig_name_list[number].split(options['path'][0])[-1].split(".png")[0]
	cmd = "cp " + p + " " + out_root_path + phys_number + ".png"
	os.system(cmd)