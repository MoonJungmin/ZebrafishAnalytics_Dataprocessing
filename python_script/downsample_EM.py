#!/home/jmmoon/anaconda2/bin/python
import os, sys
import numpy as np
from skimage import io
from skimage.transform import rescale, resize, downscale_local_mean


em_root_path = "/scratch/jmmoon/180602_median2_160515_SWiFT_60nmpx_singles_distribute/"
out_root_path = "/scratch/jmmoon/180602_median2_160515_SWiFT_120nmpx_singles_distribute/"

node_number = int(sys.argv[1])

def search(aDirname, aType):
	filelist = []
	filenames = os.listdir(aDirname)
	for filename in filenames:
		full_filename = os.path.join(aDirname, filename)
		ext = os.path.splitext(full_filename)[-1]
		if ext == aType:
			filelist.append(full_filename)
	return filelist

EM_List = search(em_root_path, ".png")
EM_List.sort()


if not os.path.isdir(out_root_path):
	os.makedirs(out_root_path)


for path in EM_List:

	filename = path.split(em_root_path)[-1]
	# print filename.split(".png")[0]
	number = int(filename.split(".png")[0])
	# print filename
	# if number%2 != 0:
	image = io.imread(path)
	image_rescaled = rescale(image, 1.0 / 2.0)
	io.imsave(out_root_path + filename, image_rescaled)
