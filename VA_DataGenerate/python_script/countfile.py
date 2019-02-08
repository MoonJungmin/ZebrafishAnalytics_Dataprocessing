import os, sys
from subprocess import PIPE, Popen

rootpath = "/scratch/jmmoon/SubregionBlock/"


def cmdline(command):
    process = Popen(
        args=command,
        stdout=PIPE,
        shell=True
    )
    return process.communicate()[0]

def search_dir(aDirname):
    filelist = []
    filenames = os.listdir(aDirname)
    for filename in filenames:
        full_filename = os.path.join(aDirname, filename)
        ext = os.path.splitext(full_filename)[-1]
        filelist.append(full_filename)

    return filelist

def search(aDirname, aType):
    filelist = []
    filenames = os.listdir(aDirname)
    for filename in filenames:
        full_filename = os.path.join(aDirname, filename)
        ext = os.path.splitext(full_filename)[-1]
        if ext == aType:
            filelist.append(full_filename)

    return filelist


dirlist = search_dir(rootpath)
dirlist.sort()



def check(f):
	block_size = 512*512*512
	if os.path.getsize(f) != block_size:
		print "error!"
		return 0
	else:
		return 1



for f in dirlist:
	name = f.split(rootpath)[-1]
	
	
	#os.system(str)
	cmd = "find " + f +"/XY/ -type f | wc -l" 
	xycount = int(cmdline(cmd))

	cmd = "find " + f +"/YZ/ -type f | wc -l" 
	yzcount = int(cmdline(cmd))

	cmd = "find " + f +"/ZX/ -type f | wc -l" 
	zxcount = int(cmdline(cmd))

	print name + " : " + str(xycount) + " " + str(yzcount) + " " + str(zxcount)

	xy_list = search(rootpath+name+"/XY/", ".block")
	yz_list = search(rootpath+name+"/YZ/", ".block")
	zx_list = search(rootpath+name+"/ZX/", ".block")

	block_size = 512*512*512
	for b in xy_list:
		block_name = b.split(rootpath+name+"/XY/")[-1]
		if os.path.getsize(b) != block_size:		
			print block_name
	for b in yz_list:
		block_name = b.split(rootpath+name+"/YZ/")[-1]
		if os.path.getsize(b) != block_size:		
			print block_name
	for b in zx_list:
		block_name = b.split(rootpath+name+"/ZX/")[-1]
		if os.path.getsize(b) != block_size:		
			print block_name


	# if os.path.getsize(f) != block_size:
		# print "error!"
	# flist = search(rootpath+name+"/XY/", ".block")
	# for b in flist:
	# 	b_name = b.split(rootpath+name+"/XY/")[-1]
	# 	if check(b) == 0:
	# 		str = "cp /scratch/jmmoon/SubregionBlockBackup/" + name +"/XY/" + b_name + " " + b
	# 		#os.system(str)
