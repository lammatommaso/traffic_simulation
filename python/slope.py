import sys
import numpy
import matplotlib.pyplot as plt
import importlib

def plot_simulation(number):
    plt.style.use('my_style')
    city_name = 'city'+f'{number}'
    sim_name = 'simulation'+f'{number}'

    city = importlib.import_module(city_name, package=None)
    simulation = importlib.import_module(sim_name, package=None)

    fit, stats = numpy.polynomial.polynomial.polyfit(simulation.x,(simulation.mean_steps+simulation.mean_stops)/simulation.mean_steps,([0,1]), full=True)
    
    f = open("../slopedata.txt", "a")
    f.write(f'{city.one_way_fraction} {fit[1]}\n')
    f.close()

for i in range (0,30):
    plot_simulation(i+1)

