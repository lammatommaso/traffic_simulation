import sys
import numpy
from math import sqrt
import matplotlib.pyplot as plt
import sim500, sim1000, sim1500, sim2000, sim2500, sim3000, sim3500, sim4000

plt.style.use('my_style')

x = numpy.linspace(0,1,500)

plt.scatter(sim500.x,(sim500.mean_stops+sim500.mean_steps)/sim500.mean_steps, label='500 cars')
fit500 = numpy.polynomial.polynomial.polyfit(sim500.x,(sim500.mean_stops+sim500.mean_steps)/sim500.mean_steps,([0,2]))
plt.plot(x,fit500[0]+fit500[2]*x*x, color='black', linestyle='dashed', label = 'polynomial fit')

plt.scatter(sim1000.x,(sim1000.mean_stops+sim1000.mean_steps)/sim1000.mean_steps, label='1000 cars')
fit1000 = numpy.polynomial.polynomial.polyfit(sim1000.x,(sim1000.mean_stops+sim1000.mean_steps)/sim1000.mean_steps,([0,2]))
plt.plot(x,fit1000[0]+fit1000[2]*x*x, color='black', linestyle='dashed')

plt.scatter(sim1500.x,(sim1500.mean_stops+sim1500.mean_steps)/sim1500.mean_steps, label='1500 cars')
fit1500 = numpy.polynomial.polynomial.polyfit(sim1500.x,(sim1500.mean_stops+sim1500.mean_steps)/sim1500.mean_steps,([0,2]))
plt.plot(x,fit1500[0]+fit1500[2]*x*x, color='black', linestyle='dashed')

plt.scatter(sim2000.x,(sim2000.mean_stops+sim2000.mean_steps)/sim2000.mean_steps, label='2000 cars')
fit2000 = numpy.polynomial.polynomial.polyfit(sim2000.x,(sim2000.mean_stops+sim2000.mean_steps)/sim2000.mean_steps,([0,2]))
plt.plot(x,fit2000[0]+fit2000[2]*x*x, color='black', linestyle='dashed')

plt.scatter(sim2500.x,(sim2500.mean_stops+sim2500.mean_steps)/sim2500.mean_steps, label='2500 cars')
fit2500 = numpy.polynomial.polynomial.polyfit(sim2500.x,(sim2500.mean_stops+sim2500.mean_steps)/sim2500.mean_steps,([0,2]))
plt.plot(x,fit2500[0]+fit2500[2]*x*x, color='black', linestyle='dashed')

plt.scatter(sim3000.x,(sim3000.mean_stops+sim3000.mean_steps)/sim3000.mean_steps, label='3000 cars')
fit3000 = numpy.polynomial.polynomial.polyfit(sim3000.x,(sim3000.mean_stops+sim3000.mean_steps)/sim3000.mean_steps,([0,2]))
plt.plot(x,fit3000[0]+fit3000[2]*x*x, color='black', linestyle='dashed')

plt.scatter(sim3500.x,(sim3500.mean_stops+sim3500.mean_steps)/sim3500.mean_steps, label='3500 cars')
fit3500 = numpy.polynomial.polynomial.polyfit(sim3500.x,(sim3500.mean_stops+sim3500.mean_steps)/sim3500.mean_steps,([0,2]))
plt.plot(x,fit3500[0]+fit3500[2]*x*x, color='black', linestyle='dashed')

plt.scatter(sim4000.x,(sim4000.mean_stops+sim4000.mean_steps)/sim4000.mean_steps, label='4000 cars')
fit4000 = numpy.polynomial.polynomial.polyfit(sim4000.x,(sim4000.mean_stops+sim4000.mean_steps)/sim4000.mean_steps,([0,2]))
plt.plot(x,fit4000[0]+fit4000[2]*x*x, color='black', linestyle='dashed')

plt.title('Oneway Increment Simulation')
plt.xlabel('Oneway Fraction')
plt.ylabel('Traffic Index')

# c = numpy.linspace(0,4500,10000)
# car_number = ([500,1000,1500,2000,2500,3000,3500,4000])
# ex_time = ([sim500.execution_time, sim1000.execution_time, sim1500.execution_time, sim2000.execution_time,sim2500.execution_time,sim3000.execution_time, sim3500.execution_time,sim4000.execution_time])
# plt.scatter(car_number,ex_time, label = 'execution time')
# fit_time, stats= numpy.polynomial.polynomial.polyfit(car_number, ex_time,([1,2,3,4]), full = True)
# plt.plot(c,fit_time[1]*c+fit_time[2]*c**2+fit_time[3]*c**3+fit_time[4]*c**4, color='black', linestyle='dashed', label = 'polynomial fit')
# errors = []
# for i in range(0,8):
#     eval_i = fit_time[1]*car_number[i]+fit_time[2]*car_number[i]**2+fit_time[3]*car_number[i]**3+fit_time[4]*car_number[i]**4
#     errors.append(eval_i - ex_time[i])
# print(f'polynomial fit = {fit_time[1]}x + {fit_time[2]}x^2+{fit_time[3]}x^3+{fit_time[4]}x^4')
# print(f'statistics = {stats}')
# print(f'errors = {errors}')
# plt.title('Execution Statistics')
# plt.xlabel('Car Number')
# plt.ylabel('Execution Time(s)')

plt.legend()
plt.savefig("oneway_increment.png")
