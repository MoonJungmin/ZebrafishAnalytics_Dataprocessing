import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

# source_path1 =  "/scratch/jmmoon/rotateYZ/"
# source_path2 =  "/scratch/jmmoon/rotateZX/"
# target_path1 =  "/scratch/jmmoon/180602_median2_160515_SWiFT_120nmpx_singles_distribute_raw_Tile/YZ/"
# target_path2 =  "/scratch/jmmoon/cell_Tile/ZX/"

dimX = 5120
dimY = 4608
dimZ = 8192

countX = dimX / 512
countY = dimY / 512
countZ = dimZ / 512


def search(aDirname, aType):
    filelist = []
    filenames = os.listdir(aDirname)
    for filename in filenames:
        full_filename = os.path.join(aDirname, filename)
        ext = os.path.splitext(full_filename)[-1]
        if ext == aType:
            filelist.append(full_filename)

    return filelist


source_path1 =  "/scratch/jmmoon/Cell/Ganglia/Block/rotateYZ/"
source_path2 =  "/scratch/jmmoon/Cell/Ganglia/Block/rotateZX/"
target_path1 =  "/scratch/jmmoon/Cell/Ganglia/Block/YZ/"
target_path2 =  "/scratch/jmmoon/Cell/Ganglia/Block/ZX/"


for y in range(0, countY):
    for x in range(0, countX):
        if os.path.isfile(source_path1 + str(node_num) + "_" + str(y) + "_" + str(x) + "_0.block"):
            cmdYZ = "rsync -az " + source_path1 + str(node_num) + "_" + str(y) + "_" + str(x) + "_0.block ib" + str(x+1).zfill(2) + ":" + target_path1  
            #print cmdYZ
            if x+1 == 1:
                os.system(cmdYZ)
        if os.path.isfile(source_path2 + str(node_num) + "_" + str(y) + "_" + str(x) + "_0.block"):
            cmdZX = "rsync -az " + source_path2 + str(node_num) + "_" + str(y) + "_" + str(x) + "_0.block ib" + str(y+1).zfill(2) + ":" + target_path2
            #print cmdZX
            if y+1 == 1:
                os.system(cmdZX)


        #break