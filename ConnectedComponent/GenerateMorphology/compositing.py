import os, sys
import numpy


tempList_Box = []
tempList_Morpho = []


fname = "/scratch/jmmoon/diamond/Cell/Brain/Cell_BoundingBox.dat";
box_container = []
with open(fname) as f:
	box_container = f.readlines()

for line in box_container:
	obj = {}
	l = line.split("\n")[0].split(" ")
	obj['index'] = int(l[0])
	obj['minX'] = int(l[1])
	obj['minY'] = int(l[2])
	obj['minZ'] = int(l[3])
	obj['maxX'] = int(l[4])
	obj['maxY'] = int(l[5])
	obj['maxZ'] = int(l[6])
	#obj['seedX'] = int(l[7])
	#obj['seedY'] = int(l[8])
	#obj['seedZ'] = int(l[9])
		
	obj['volume'] = 0
	obj['surfacearea'] = 0
	obj['sphericity'] = 0
	obj['eccentricity'] = 0
	obj['intensity'] = 0


	tempList_Box.append(obj)


for i in range(0,16):
	fname2 = "/home/jmmoon/Projects/ConnectedComponent/shared/Morphological_" + str(i) + ".dat";
	morpho_container = []
	with open(fname2) as f:
		morpho_container = f.readlines()

	for line in morpho_container:
		obj = {}
		l = line.split("\n")[0].split(" ")
		if tempList_Box[int(l[0])-1]['index'] == int(l[0]):
			tempList_Box[int(l[0])-1]['volume'] = int(l[1])
			tempList_Box[int(l[0])-1]['surfacearea'] = int(numpy.float64(l[2]))
			tempList_Box[int(l[0])-1]['sphericity'] = numpy.float64(l[3])
			tempList_Box[int(l[0])-1]['eccentricity'] = numpy.float64(l[4]) 
			if l[5] != "-nan":
				tempList_Box[int(l[0])-1]['intensity'] = numpy.float64(l[5])
			else:
				tempList_Box[int(l[0])-1]['intensity'] = -1
		
		# obj['index'] = int(l[0])
		# obj['volume'] = int(l[1])
		# obj['surfacearea'] = int(l[2])
		# obj['sphericity'] = int(l[3])
		# obj['eccentricity'] = int(l[4])
		# obj['intensity'] = int(l[5])
		

		# tempList_Morpho.append(obj)

# mlist.sort(key=lambda x: x['index'], reverse=True)
resultlist = sorted(tempList_Box, key=lambda k: k['index']) 

print tempList_Box[0]['index'];
print len(tempList_Box);




f = open("/home/jmmoon/Projects/ConnectedComponent/shared/MorphologicalFeatureComp.dat", 'w')
for t in resultlist:
	data = str(t['index']) + " " + str(t['minX']) + " " + str(t['minY']) + " " + str(t['minZ']) \
			+ " " + str(t['maxX']) + " " + str(t['maxY']) + " " + str(t['maxZ']) \
			+ " " + str(t['volume']) + " " + str(t['surfacearea']) + " " + "%.9f" % t['sphericity']+ " " + str(t['eccentricity'])+ " " + str(t['intensity']) + "\n"
	f.write(data)

f.close()