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

    x = numpy.linspace(500,4500,10000)
    plt.scatter(simulation.x,(simulation.mean_steps+simulation.mean_stops)/simulation.mean_steps, label = f'oneway fraction {city.one_way_fraction}')
    fit, stats = numpy.polynomial.polynomial.polyfit(simulation.x,(simulation.mean_steps+simulation.mean_stops)/simulation.mean_steps,([0,1]), full=True)
    plt.plot(x,fit[0]+fit[1]*x,color = 'black',linestyle = 'dashed', label = f'{fit[0]}$+${fit[1]}$x$')

    plt.title('Car Increment Simulation')
    plt.xlabel('Car Number')
    plt.ylabel('Traffic Index')

    plt.legend(loc = 'upper left')
    plt.savefig('simulation'+f'{number}'+'.png')
    plt.clf()

    f = open("data.txt", "a")
    f.write(f'{fit[0]} {fit[1]}\n')
    f.close()

for i in range (0,30):
    plot_simulation(i+1)
