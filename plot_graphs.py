import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline
import seaborn as sns
import scipy
import argparse
import json

def _cdf(file_path):
  
  with open(file_path) as f:
    compiled_data = f.read()
    data_list = compiled_data.split('==================')   #Splits the data for different runs
    data_list.pop() #removes the last empty element in this list
  
  json_data_list = [json.loads(data) for data in data_list]

  tput_list = []
  #Looping over Runs
  for data in json_data_list:
    #Looping over UE in a run
    for i in range(10):
      tput = float(data["Flow"+str(i+1)]["Throughput"].split()[0])
      tput_list.append(tput)

  print(tput_list)

  data_sorted = np.sort(tput_list)
  #p = 1. * np.arange(len(data_sorted)) / (len(data_sorted) - 1)
  p = np.arange(len(data_sorted)) / len(data_sorted)

  return data_sorted, p

def Q8(): 

  #RR
  tput_list_mobRR, p_mobRR = _cdf('Q8-mobileRR')
  tput_list_staRR, p_staRR = _cdf('Q8-staticRR')
  fig, ax = plt.subplots()
  ax.plot(tput_list_mobRR, p_mobRR, label = 'Mobile RR')
  ax.plot(tput_list_staRR, p_staRR, label = 'Static RR')
  """plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.legend()
  plt.show()"""

  #PF
  tput_list_mobPF, p_mobPF = _cdf('Q8-mobilePF')
  tput_list_staPF, p_staPF = _cdf('Q8-staticPF')
  #fig, ax = plt.subplots()
  ax.plot(tput_list_mobPF, p_mobPF, label = 'Mobile PF')
  ax.plot(tput_list_staPF, p_staPF, label = 'Static PF')
  """plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.legend()
  plt.show()"""

  #MR
  tput_list_mobMR, p_mobMR = _cdf('Q8-mobileMR')
  tput_list_staMR, p_staMR = _cdf('Q8-staticMR')
  #fig, ax = plt.subplots()
  ax.plot(tput_list_mobMR, p_mobMR, label = 'Mobile MR')
  ax.plot(tput_list_staMR, p_staMR, label = 'Static MR')
  plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.legend()
  plt.show()

def Q9():
  #Q9 Plots
  #RR
  tput_list_mobRR, p_mobRR = _cdf('Q9-mobileRR')
  tput_list_staRR, p_staRR = _cdf('Q9-staticRR')
  fig, ax = plt.subplots()
  ax.plot(tput_list_mobRR, p_mobRR, label = 'Mobile RR')
  ax.plot(tput_list_staRR, p_staRR, label = 'Static RR')
  """plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.legend()
  plt.show()"""

  #PF
  tput_list_mobPF, p_mobPF = _cdf('Q9-mobilePF')
  tput_list_staPF, p_staPF = _cdf('Q9-staticPF')
  #fig, ax = plt.subplots()
  ax.plot(tput_list_mobPF, p_mobPF, label = 'Mobile PF')
  ax.plot(tput_list_staPF, p_staPF, label = 'Static PF')
  """plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.legend()
  plt.show()"""

  #MR
  tput_list_mobMR, p_mobMR = _cdf('Q9-mobileMR')
  tput_list_staMR, p_staMR = _cdf('Q9-staticMR')
  #fig, ax = plt.subplots()
  ax.plot(tput_list_mobMR, p_mobMR, label = 'Mobile MR')
  ax.plot(tput_list_staMR, p_staMR, label = 'Static MR')
  plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.legend()
  plt.show()



if __name__ == "__main__":

  parser = argparse.ArgumentParser()
  parser.add_argument("-t", "--task", help="specify Q8, Q9")
  args = parser.parse_args()
  print(args)

  if args.task == 'Q8':
    Q8()
  elif args.task == 'Q9':
    Q9()
  else:
    print('Please specify the task to be performed')





