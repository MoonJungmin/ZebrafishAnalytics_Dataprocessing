import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform


node_num = int(platform.node().split("emerald")[-1]) - 1


root_path = "/scratch/jmmoon/Cell/Block/"
block_size = 4 * 512 * 512 * 512


def search(aDirname, aType):
    filelist = []
    filenames = os.listdir(aDirname)
    for filename in filenames:
        full_filename = os.path.join(aDirname, filename)
        ext = os.path.splitext(full_filename)[-1]
        if ext == aType:
            filelist.append(full_filename)
   
    return filelist


dirlist = ["/scratch/jmmoon/Cell/Block/XY", "/scratch/jmmoon/Cell/Block/YZ", "/scratch/jmmoon/Cell/Block/ZX"]

sum = 0

for d in dirlist:
	filelist = search(d + "/", ".block")
	d_name = d.split(root_path)[-1]
	print d_name
	if d_name == "rotateYZ" or d_name == "rotateZX":
		continue

	for f in filelist:
		f_name = f.split(d+"/")[-1]
		if os.path.getsize(f) != block_size:
			print f 
		else:
			array = numpy.fromfile(f, dtype=numpy.uint32)
			count = numpy.count_nonzero(array)
			if count == 0:
				os.system("rm "+ f )
		
