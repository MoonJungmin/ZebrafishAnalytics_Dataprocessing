import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform


node_num = int(platform.node().split("emerald")[-1]) - 1


root_path = "/scratch/jmmoon/SubregionBlock/"
subregion_list = []
block_size = 512 * 512 * 512


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


def remove(f):
	array = numpy.fromfile(f, dtype=numpy.uint8)
	count = numpy.count_nonzero(array)
	if count == 0:
		print ("rm "+ f)
		os.system("rm "+ f)

dirlist = search_dir(root_path)
dirlist.sort()


for p in dirlist:
	subregion_name = p.split(root_path)[-1]
	subregion_list.append(subregion_name)


for subregion in subregion_list:
	filelistXY = search(root_path+subregion+"/XY/", ".block")
	filelistYZ = search(root_path+subregion+"/YZ/", ".block")
	filelistZX = search(root_path+subregion+"/ZX/", ".block")

	for file in filelistXY:
		remove(file)
	for file in filelistYZ:
		remove(file)
	for file in filelistZX:
		remove(file)

	#print (str2)
	#print (str3)

