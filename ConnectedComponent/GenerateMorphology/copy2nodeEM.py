import os, sys
import platform

def search(aDirname, aType):
	filelist = []
	filenames = os.listdir(aDirname)
	for filename in filenames:
		full_filename = os.path.join(aDirname, filename)
		ext = os.path.splitext(full_filename)[-1]
		if ext == aType:
			filelist.append(full_filename)
	return filelist

target_num = int(platform.node().split("emerald")[-1]) - 1
target_name = "ib" + '{0:02}'.format(target_num)

print target_name


myfiles = search("/scratch/jmmoon/EM/EM_morpho/" , ".png")

myfiles.sort()

print myfiles

if target_num > 0:
	idx = 0
	for f in myfiles:
		if idx < 500:
			os.system("rsync -az " + f + " " + target_name + ":/scratch/jmmoon/EM/EM_morpho_copy/")
		idx+=1

# scp /scratch/jmmoon/connectedcomponent_clean/* ib02:/scratch/jmmoon/connectedcomponent_clean/

target_num2 = int(platform.node().split("emerald")[-1]) - 2
target_name2 = "ib" + '{0:02}'.format(target_num2)

if target_num2 > 0:
	idx = 0
	for f in myfiles:
		if idx < 500:
			os.system("rsync -az " + f + " " + target_name2 + ":/scratch/jmmoon/EM/EM_morpho_copy/")
		idx+=1
