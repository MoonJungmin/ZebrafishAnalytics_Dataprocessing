import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

#root_path_list = ["/scratch/jmmoon/Subregion/Brainmask/", "/scratch/jmmoon/Subregion/EyeLeft/", "/scratch/jmmoon/Subregion/DiencephalonHabenulaLeft/", "/scratch/jmmoon/Subregion/EyeRight/", "/scratch/jmmoon/Subregion/DiencephalonHabenulaRight/", "/scratch/jmmoon/Subregion/MesencephalonTecumNeuropil/"]

root_path = "/scratch/jmmoon/Cell/Ganglia/raw_file/"

def search(aDirname, aType):
    filelist = []
    filenames = os.listdir(aDirname)
    for filename in filenames:
        full_filename = os.path.join(aDirname, filename)
        ext = os.path.splitext(full_filename)[-1]
        if ext == aType:
            filelist.append(full_filename)

    return filelist



# for root_path in root_path_list:
filelist = search(root_path, ".raw")
filelist.sort();

for i in range(0, 512):
    name = str(node_num*512 + i).zfill(5) +".raw"
    
    if os.path.isfile(root_path + name):
        continue
    else:
        f = open(root_path + name, 'wb')
        im = numpy.zeros(5120*4608, dtype=numpy.uint32)
        im2arr = im.tobytes()
        f.write(im2arr)

    f.close()
    print (name)
	# im.save(name+".raw")	
