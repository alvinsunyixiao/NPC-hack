from tqdm import tqdm
#Written by Jenny Zhan
#Calculate distance between two points
def calS(a,b):
    return ((a[0]-b[0])**2+(a[1]-b[1])**2)**0.5

#Tag the trees, for later catergorization
def Treeid(a):
    if a=='Cedar':
        return 1
    if a=='PlaneTree':
        return 2
    if a=='Palm':
        return 3
    if a=='Pine':
        return 4
    if a=='MaidenhairTree':
        return 5
    if a=='Birch':
        return 6
    if a=='Polar':
        return 7

#'a' is the former level of the trees, 'b' is the later level. Find the optimal track so far to each 'b' point with the help of calS(a,b) function
def OneToOne(a,b):
    for i in range(len(b)):
        distance=calS(b[i],a[0])+a[0][2]
        for j in range(len(a)):
            #Choose the track with the minimum time taken
            if distance>=calS(b[i],a[j])+a[j][2]:
                distance=calS(b[i],a[j])+a[j][2]
                traceid=j#traceid means where(which 'a' point) the minimum time taken came from
        b[i][2]=distance#Store the accumulated time
        b[i][3]=traceid

#Input maplist and guidebooklist
maplist=[]
guidebooklist=[]
import csv
with open('map.csv',newline='') as csvfile:
    reader = csv.reader(csvfile,dialect='excel')
    for row in reader:
        maplist.append([int(row[0]),int(row[1]),row[2]])
with open('guidebook.csv',newline='') as csvfile:
    reader = csv.reader(csvfile,dialect='excel')
    for row in reader:
        guidebooklist=row

#Tag the trees in the map. Categorize trees into species by piling up. Store the coordinates and prepare two more empty element along with them.
TreeSpe=[[]for i in range(8)]
for i in range(len(maplist)):
    TreeSpe[Treeid(maplist[i][2])].append([maplist[i][0],maplist[i][1],[],[]],)

#Create a 'tree' of guidebook list, each level consists of a bunch of choices
FindingTree=[[]for i in range(len(guidebooklist)+1)]
FindingTree[0]=[[0,0,0,0]]
from copy import deepcopy
for i in range(1,len(guidebooklist)+1):
    FindingTree[i]=deepcopy(TreeSpe[Treeid(guidebooklist[i-1])])

#Find and store the accumulated time to each coordinates, with the help of OneToOne(a,b)Function
for u in tqdm(range(len(guidebooklist))):
    OneToOne(FindingTree[u],FindingTree[u+1])

#Find the minimum total walking time
minimum=FindingTree[len(FindingTree)-1][0][2]
minimumlocation=0
for i in range(len(FindingTree[-1])):
    if minimum>FindingTree[-1][i][2]:
        minimum=FindingTree[-1][i][2]
        minimumlocation=i

#Trace back the optimum track using traceid
##Initializing Track and determine the first and last elements in the Track list
Track=[[]for i in range(len(guidebooklist)+1)]
Track[0]=[0,0,'Start']
Track[-1]=FindingTree[-1][minimumlocation][:2]
traceid=FindingTree[-1][minimumlocation][3]
##Tracing back, alternating traceid
for i in range(1,len(Track)-1):
    Track[len(Track)-i-1]=FindingTree[len(Track)-i-1][traceid][:2]
    traceid=FindingTree[len(Track)-i-1][traceid][3]
##Add the corresponding tree name to each coordinate
for i in range(1,len(Track)):
    Track[i].append(guidebooklist[i-1])
Track.remove(Track[0])

#Write to a file
FileName=('solution-'+str(round(minimum))+'.csv')
with open(FileName, 'w',newline='') as csvfile:
    spamwriter = csv.writer(csvfile,dialect='excel')
    for i in Track:
        spamwriter.writerow(i)
