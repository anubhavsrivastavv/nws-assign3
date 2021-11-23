#Logic to compute averages over multiple simulation runs
import json

def generate_data(file_path):
  
  with open(file_path) as f:
    compiled_data = f.read()
    data_list = compiled_data.split('==================')   #Splits the data for different runs
    data_list.pop() #removes the last empty element in this list
  
  json_data_list = [json.loads(data) for data in data_list]

  print(json_data_list[0])
  #Compute averages:
  aggregate_throughput = 0 #divide this by 5 when reporting average
  aggregate_packet_loss = 0 #divide this by 5x6 = 30 when reporting average
  aggregate_packet_delay = 0 #divide this by 5x6 = 30 when reporting average
  
  for data in json_data_list:
    for flow, flow_params in data.items():
      if flow == 'Mean flow throughput':
        break
      #print(flow)
      #print(flow_params)
      #print(float(flow_params['Throughput'].split()[0]))
      aggregate_throughput +=  float(flow_params['Throughput'].split()[0])
      aggregate_packet_loss += float(flow_params['Loss Rate'])
      aggregate_packet_delay += float(flow_params['Mean delay'].split()[0])

  print('aggregate_throughput: ', aggregate_throughput)
  print('aggregate_packet_loss: ', aggregate_packet_loss)
  print('aggregate_packet_delay: ', aggregate_packet_delay)

  print('average aggregate_throughput: ', aggregate_throughput/len(json_data_list))
  print('average packet loss: ', aggregate_packet_loss/(len(json_data_list)*6))
  print('average packet delay: ', aggregate_packet_delay/(len(json_data_list)*6))

"""
#Task 2
print('############### Task2 ###############')
#print('Numerlogy 0')
#generate_data('TdmaPFNum0')
print('Numerlogy 1')
generate_data('TdmaPFNum1')
#print('Numerlogy 2')
#generate_data('TdmaPFNum2')
#print('Numerlogy 3')
#generate_data('TdmaPFNum3')
"""
#Task 2 Part B
def generate_data_Task2B(file_path):
  
  with open(file_path) as f:
    compiled_data = f.read()
    data_list = compiled_data.split('==================')   #Splits the data for different runs
    data_list.pop() #removes the last empty element in this list
  
  json_data_list = [json.loads(data) for data in data_list]

  #Lists of size 6 to store average values for each UE over multiple runs
  througput = [0 for i in range(6)]
  delay = [0 for i in range(6)]
  loss_rate = [0 for i in range(6)]

  for data in json_data_list:
    #Flowwise
    for i in range(6):
      througput[i] = througput[i] + float(data["Flow"+str(i+1)]["Throughput"].split()[0])
      loss_rate[i] = loss_rate[i] + float(data["Flow"+str(i+1)]["Loss Rate"])
      delay[i] = delay[i] + float(data["Flow"+str(i+1)]['Mean delay'].split()[0])
  
  for i in range(6):
    print(f'For UE {i+1}')
    print('througput: ', round(througput[i]/5, 3))
    print('loss_rate: ', round(loss_rate[i]/5, 3))
    print('delay: ', round(delay[i]/5, 3))    
  
  return [i/5 for i in througput], [i/5 for i in loss_rate], [i/5 for i in delay]
  
"""
print('Numerlogy 0')
generate_data_Task2B('TdmaPFNum0')
print()
print('Numerlogy 1')
generate_data_Task2B('TdmaPFNum1')
print()
print('Numerlogy 2')
tputNum2, lossNum2, delayNum2 = generate_data_Task2B('TdmaPFNum2')
print(tputNum2)
print(lossNum2)
print(delayNum2)
print()
print('Numerlogy 3')
tputNum3, lossNum3, delayNum3 =generate_data_Task2B('TdmaPFNum3')
print(tputNum3)
print(lossNum3)
print(delayNum3)
print()

print('Average of Num 2 and 3')
for i in range(6):
  tputNum2[i] = round((tputNum2[i] +tputNum3[i])/2,3)
  lossNum2[i] = round((lossNum2[i] +lossNum3[i])/2,3)
  delayNum2[i] = round((delayNum2[i] +delayNum3[i])/2,3)

print(tputNum2)
print(lossNum2)
print(delayNum2)
print()
"""

############ Task 2 ends ##############
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline
import seaborn as sns
import scipy

def _cdf(file_path):
  
  with open(file_path) as f:
    compiled_data = f.read()
    data_list = compiled_data.split('==================')   #Splits the data for different runs
    data_list.pop() #removes the last empty element in this list
  
  json_data_list = [json.loads(data) for data in data_list]

  """
    throughput matrix of the form:
          Seed1   Seed2   Seed3   Seed4   Seed5
      UE1
      UE2
      .
      .
      UE10
  """

  tput_matrix = [[] for j in range(10)]
  tput_list = []
  #Looping over Runs
  for data in json_data_list:
    #Looping over UE in a run
    for i in range(10):
      tput = float(data["Flow"+str(i+1)]["Throughput"].split()[0])
      tput_matrix[i].append(tput)
      tput_list.append(tput)

  print(tput_list)

  """
  for i in range(len(tput_matrix)):
    for j in range(len(tput_matrix[i])):
      print(tput_matrix[i][j],' ',end = '')
    print()
  """

  #Generating for the i_th UE
  #fig, ax = plt.subplots()

  """for i in range(len(tput_matrix)):
    data_sorted = np.sort(tput_matrix[i])

    # calculate the proportional values of samples
    p = 1. * np.arange(len(data_sorted)) / (len(data_sorted) - 1)

    #Smoothning code begins
    xnew = np.linspace(data_sorted.min(), data_sorted.max(), 300) 

    spl = make_interp_spline(data_sorted, p, k=0)  # type: BSpline
    p_smooth = spl(xnew)
    #Smoothning code ends

    #norm_cdf = scipy.stats.norm.cdf(tput_matrix[i])
    sns.lineplot(data_sorted, p, label = 'UE'+str(i+1))
    #ax.plot(xnew, p_smooth, label = 'UE'+str(i+1))"""

  data_sorted = np.sort(tput_list)
  #p = 1. * np.arange(len(data_sorted)) / (len(data_sorted) - 1)
  p = np.arange(len(data_sorted)) / len(data_sorted)


  """sns.lineplot(data_sorted, p)
  #ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.06),
  #        fancybox=True, shadow=True, ncol=2)
  plt.xlabel('Throughput')
  plt.ylabel('CDF')
  plt.show()"""
  return data_sorted, p

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
