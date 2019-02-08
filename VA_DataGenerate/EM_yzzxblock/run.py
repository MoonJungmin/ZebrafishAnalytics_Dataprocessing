import os, sys

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
target_path = "/scratch/jmmoon/SubregionBlockTemp/"

dirlist = search_dir(root_path)
dirlist.sort()

for p in dirlist:
    name = p.split(root_path)[-1]
    if not os.path.isdir(target_path + name):
        os.makedirs(target_path + name)
    if not os.path.isdir(target_path + name + "/YZ"):
        os.makedirs(target_path + name + "/YZ")	
    if not os.path.isdir(target_path + name + "/ZX"):
        os.makedirs(target_path + name + "/ZX")


    in_path = root_path + name + "/XY/"
    out_path1 = target_path + name + "/YZ/"
    out_path2 = target_path + name + "/ZX/"

    #print ("./build/app " + in_path + " " + out_path)
    os.system("./build/app " + in_path + " " + out_path1 + " " + out_path2 )