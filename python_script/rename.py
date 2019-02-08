import os, sys
from skimage import io
import numpy as np
from scipy import stats
from skimage.color import rgb2gray
from skimage.morphology import opening, disk
from skimage.measure import label
import skimage.transform

def search(aDirname, aType):
	filelist = []
	filenames = os.listdir(aDirname)
	for filename in filenames:
		full_filename = os.path.join(aDirname, filename)
		ext = os.path.splitext(full_filename)[-1]
		if ext == aType:
			filelist.append(full_filename)
	return filelist



inpath = "/scratch/jmmoon/connectedcomponent_label/"

os.chdir(inpath)
flist = search(".", ".raw")

index = 0
for v in range(0, 80):
	for s in range(0, 100):
		f1 = "v" + str(v) + "_s" + str(s) +".raw"
		f2 = str(index).zfill(5) +".raw"
		cmd = "mv v" + str(v) + "_s" + str(s) +".raw " + str(index).zfill(5) +".raw"
		print(cmd)
		try:
			os.rename(f1, f2)
		except:
			print ("not found")
		index += 1

#for file in flist:
	
	# imgname = file.split(inpath)[-1]
	# print(imgname)
	# img = io.imread(file)
	# resize_img = skimage.transform.rescale(img, 1.0/16.0)
	# io.imsave(outpath+imgname, resize_img)
	# print(img)
#	print(file)
	# print(outpath+imgname)