import csv

#if you write code like this, you will be penalized.
Track=[[19, 18, 'Birch'], [20, 19, 'Cedar'], [27, 26, 'Pine'], [22, 33, 'Cedar'], [27, 26, 'Pine'], [26, 14, 'Polar'], [20, 19, 'Cedar'], [23, 17, 'MaidenhairTree'], [19, 18, 'Birch'], [26, 14, 'Polar']]
with open('solution-90.csv', 'w',newline='') as csvfile:
    spamwriter = csv.writer(csvfile,dialect='excel')
    for i in Track:
        spamwriter.writerow(i)

print('Good Luck')
print('Have Fun')
print('')
input('press any to quit')
