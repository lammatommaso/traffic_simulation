import numpy
from matplotlib import pyplot as plt 

def read_file(file_name):
    with open(file_name, 'r') as data:
        x = []
        y = []
        for line in data:
            p = line.split()
            x.append([float(p[0])])
            y.append([float(p[1])])
    a = numpy.array(x).ravel()
    b = numpy.array(y).ravel()
    return a,b

plt.style.use('my_style')

# x = numpy.array([0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8])
# y = numpy.array([0.00021, 0.00022, 0.00022, 0.00023, 0.00025, 0.00027, 0.00031, 0.00037, 0.00040])

# plt.scatter(x,y,label='slope')
# plt.title('Oneway Fraction Dependence')
# plt.xlabel('Oneway Fraction')
# plt.ylabel('Slope')

x = numpy.linspace(0,1,1000)
a,b = read_file('slopedata.txt')
fit = numpy.polynomial.polynomial.polyfit(a,b,([0,2,4,6]))
plt.plot(x,fit[0]+fit[2]*x**2+fit[3]*x**3+fit[4]*x**4+fit[5]*x**5+fit[6]*x**6, color='black', linestyle='dashed', label='polynomial fit')
print(f'polynomial fit : {fit[0]} + {fit[2]}x^2 + {fit[4]}x^4 + {fit[6]}x^6')
plt.scatter(a,b,label='slope')
plt.title('Oneway Fraction Dependence')
plt.xlabel('Oneway Fraction')
plt.ylabel('Slope')

plt.legend(loc='upper left')

plt.savefig('oneway_fraction_dependence.png')

