import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1


root_path = "/scratch/jmmoon/180602_median2_160515_SWiFT_120nmpx_singles_distribute/"
target_path =  "/scratch/jmmoon/180602_median2_160515_SWiFT_120nmpx_singles_distribute_raw/"
def search(aDirname, aType):
    filelist = []
    filenames = os.listdir(aDirname)
    for filename in filenames:
        full_filename = os.path.join(aDirname, filename)
        ext = os.path.splitext(full_filename)[-1]
        if ext == aType:
            filelist.append(full_filename)

    return filelist


filelist = search(root_path, ".png")
filelist.sort();

for i in range(0, 512):
    name = str(node_num*512 + i).zfill(5) +".raw"
    f = open(target_path + name + ".raw", 'wb')
    
    if os.path.isfile(root_path + name):
        im = numpy.asarray(Image.open(root_path + name)) / 255
        im = im.astype(numpy.int8)
        im2arr = im.tobytes()
        f.write(im2arr)
    else:
        im = numpy.zeros(5120*4608, dtype=numpy.int8)
        im2arr = im.tobytes()
        f.write(im2arr)

    f.close()
    print (name)
	# im.save(name+".raw")	
