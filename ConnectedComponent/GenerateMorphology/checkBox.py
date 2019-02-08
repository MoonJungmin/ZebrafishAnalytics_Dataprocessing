import os, sys



mlist = []
fname = "/scratch/jmmoon/diamond/Cell/Eye/Cell_BoundingBox.dat";
content = []
with open(fname) as f:
	content = f.readlines()

for line in content:
	obj = {}
	l = line.split("\n")[0].split(" ")
	obj['index'] = int(l[0])
	obj['minX'] = int(l[1])
	obj['minY'] = int(l[2])
	obj['minZ'] = int(l[3])
	obj['maxX'] = int(l[4])
	obj['maxY'] = int(l[5])
	obj['maxZ'] = int(l[6])
	obj['seedX'] = int(l[7])
	obj['seedY'] = int(l[8])
	obj['seedZ'] = int(l[9])
	


	mlist.append(obj)


for c in mlist:
	value = c['maxZ'] - c['minZ']

	w = c['maxX'] - c['minX']
	h = c['maxY'] - c['minY']

	if w*h*value > 1*(1000**3):
		print str(c['index']) + " size :" + str(w*h*value)
		
print len(mlist)
# print mlist[-1]['index']
# print len(mlist)




