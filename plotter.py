import matplotlib
import matplotlib.pyplot as plt
import sys
import os

# Pathname
dir_path = os.path.dirname(os.path.realpath(__file__))

# Config variables
height = 900
width = 1800

if (len(sys.argv) == 1):
    print("You need provide pathname: (*.txt)")
    exit()

try:
    pathname = os.path.join(dir_path, sys.argv[1])
    f = open(pathname, "r")
except OSError:
    print(dir_path)
    print(sys.argv[1:])
    print("Error opening the file, verify and try again, please.")
    exit()

class Generator:
    def __init__(self, xlabels, values):
        self.values = values
        self.xlabels = xlabels
    def plot(self):
        for k in self.values.keys():
            plt.plot(self.xlabels, self.values[k], linewidth=2.0, label = k)


#Temporal for reading lines
lineHolder = f.readline()

#Dictionary of values with the legend as their key
totalValues = {}

#Holds the x values for reference
xlabelValues = lineHolder.replace("\n", "").split(" ")[:-1]
xlabelValues = [int(x) for  x in xlabelValues]
print xlabelValues



#Holds the legend for the graphic representation

vals = []
lines = f.readlines()
for line in lines:
    values = line.replace("\n", "").split(" ")

    values = values[1:-1]
    values_d = []
    for v in values:
        values_d.append(float(v))
    #print(values)

    totalValues[legend] = values_d

plot_gen = Generator(xlabelValues, totalValues)
plot_gen.plot()

# Custom graph
plt.ylabel('seconds')
plt.xlabel('N')
plt.legend()

# Save graph
fig = plt.gcf()
DPI = fig.get_dpi()
fig.set_size_inches(width/float(DPI), height/float(DPI))
plt.savefig(dir_path + '/result.png')

# End
print("Grafico generado correctamente")
exit()
