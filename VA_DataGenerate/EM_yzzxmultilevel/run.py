import os, sys
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

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



root_path = "/scratch/jmmoon/SubregionBlock/"

dirlist = search_dir(root_path)
dirlist.sort()

for p in dirlist:
	name = p.split(root_path)[-1]
	save_path = root_path + name +"/"
	#print ("./build/app " + str(node_num) + " " + save_path)
	os.system("./build/app " + str(node_num) + " " + save_path)



