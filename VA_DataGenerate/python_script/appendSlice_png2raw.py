import os
import numpy, json, PIL
from PIL import Image, ImageSequence, TiffImagePlugin
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

index_path = "/scratch/jmmoon/diamond/EM/180602_median2_160515_SWiFT_120nmpx_singles/"

root_path = "/scratch/jmmoon/diamond/Subregion/"
target_path =  "/scratch/jmmoon/Subregion/"

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


InputDirList = search_dir(root_path)
indexfilelist = search(index_path, ".png")
IndexList = []
for i in indexfilelist:
    index = int(i.split(index_path)[-1].split(".png")[0])
    IndexList.append(index)

IndexList.sort()
#print (len(IndexList))


SubregionNameList = []

for f in InputDirList:
    name = f.split(root_path)[-1]
    if name == "GangliaAnteriorLateralLineLeft" or name == "GangliaAnteriorLateralLineRight" or name == "GangliaPosteriorLateralLineLeft" or name == "GangliaPosteriorLateralLineRight" or name == "EyeLeft" or name =="EyeRight":
        tpath = target_path +  name
        if not os.path.exists(tpath):
            os.makedirs(tpath)

        SubregionNameList.append(name)

SubregionNameList.sort()


print SubregionNameList

for subregionname in SubregionNameList:
    for i in range(0, 512):
        index = node_num*512 + i
#        print IndexList[index]
        cov_index = 0
        if index < len(IndexList)-1:
            cov_index = IndexList[index]

            

        spath = root_path + subregionname + "/" + str(cov_index).zfill(5) + ".png"
        tpath = target_path + subregionname + "/" + str(index).zfill(5) + ".raw"
        #print spath
        
        f = open(tpath, 'wb')
        if os.path.isfile(spath):
            im = numpy.asarray(Image.open(spath))
            im = im.astype(numpy.uint8)
            im2arr = im.tobytes()
            f.write(im2arr)
        else:
            im = numpy.zeros(5120*4608, dtype=numpy.uint8)
            im2arr = im.tobytes()
            f.write(im2arr)
            print "miss"

