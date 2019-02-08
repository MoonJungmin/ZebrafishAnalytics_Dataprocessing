import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform


node_num = int(platform.node().split("emerald")[-1]) - 1
root_path = "/scratch/jmmoon/Cell/Ganglia/Block/"


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



filelistXY = search(root_path+"XY/", ".block")
filelistYZ = search(root_path+"YZ/", ".block")
filelistZX = search(root_path+"ZX/", ".block")

for file in filelistXY:
		remove(file)
for file in filelistYZ:
		remove(file)
for file in filelistZX:
		remove(file)

	#print (str2)
	#print (str3)

