import csv
from matplotlib import pyplot as plt

Cedar = []
PlaneTree = []
Palm = []
Pine = []
MaidenhairTree = []
Birch = []
Polar = []

with open('map.csv') as csvfile:
    reader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in reader:
        row = ''.join(row).split(',')
        if row[2] == 'Cedar':
            Cedar.append([row[0],row[1]])
        elif row[2] == 'PlaneTree':
            PlaneTree.append([row[0],row[1]])
        elif row[2] == 'Palm':
            Palm.append([row[0],row[1]])
        elif row[2] == 'Pine':
            Pine.append([row[0],row[1]])
        elif row[2] == 'MaidenhairTree':
            MaidenhairTree.append([row[0],row[1]])
        elif row[2] == 'Birch':
            Birch.append([row[0],row[1]])
        elif row[2] == 'Polar':
            Polar.append([row[0],row[1]])
        else:
            print('ERROR')
            
plt.plot(Cedar,'r.',PlaneTree,'b.',Palm,'m.',Pine,'g.',MaidenhairTree,'c.',Birch,'y.',Polar,'k.')
plt.show()
