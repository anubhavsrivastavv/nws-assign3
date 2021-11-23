#Logic to compute averages over multiple simulation runs
import json
import argparse

def generate_data_A(file_path):
  
  with open(file_path) as f:
    compiled_data = f.read()
    data_list = compiled_data.split('==================')   #Splits the data for different runs
    data_list.pop() #removes the last empty element in this list
  
  json_data_list = [json.loads(data) for data in data_list]

  #print(json_data_list[0])
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

  #print('aggregate_throughput: ', aggregate_throughput)
  #print('aggregate_packet_loss: ', aggregate_packet_loss)
  #print('aggregate_packet_delay: ', aggregate_packet_delay)

  print('average aggregate_throughput: ', aggregate_throughput/len(json_data_list))
  print('average packet loss: ', aggregate_packet_loss/(len(json_data_list)*6))
  print('average packet delay: ', aggregate_packet_delay/(len(json_data_list)*6))


def generate_data_B(file_path):
  
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


def Task1():

	print('############### Task1 Part A ###############')
	print('T1-FB-OfdmaMR')
	generate_data_A('t1fullbufferrun/T1-FB-OfdmaMR')
	print()
	print('T1-FB-OfdmaPF')
	generate_data_A('t1fullbufferrun/T1-FB-OfdmaPF')
	print()
	print('T1-FB-OfdmaRR')
	generate_data_A('t1fullbufferrun/T1-FB-OfdmaRR')
	print()
	print('T1-FB-TdmaMR')
	generate_data_A('t1fullbufferrun/T1-FB-TdmaMR')
	print()
	print('T1-FB-TdmaPF')
	generate_data_A('t1fullbufferrun/T1-FB-TdmaPF')
	print()
	print('T1-FB-TdmaRR')
	generate_data_A('t1fullbufferrun/T1-FB-TdmaRR')
	print()
	print()

	
	print('############### Task1 Part B ###############')
	print('T1-FB-OfdmaMR')
	generate_data_B('t1fullbufferrun/T1-FB-OfdmaMR')
	print()
	print('T1-FB-OfdmaPF')
	generate_data_B('t1fullbufferrun/T1-FB-OfdmaPF')
	print()
	print('T1-FB-OfdmaRR')
	generate_data_B('t1fullbufferrun/T1-FB-OfdmaRR')
	print()
	print('T1-FB-TdmaMR')
	generate_data_B('t1fullbufferrun/T1-FB-TdmaMR')
	print()
	print('T1-FB-TdmaPF')
	generate_data_B('t1fullbufferrun/T1-FB-TdmaPF')
	print()
	print('T1-FB-TdmaRR')
	generate_data_B('t1fullbufferrun/T1-FB-TdmaRR')	
	print()


def Task2():
	print('############### Task2 Part A ###############')

	print('Numerlogy 0')
	generate_data_A('t2fullbufferrun/TdmaPFNum0')
	print()
	print('Numerlogy 1')
	generate_data_A('t2fullbufferrun/TdmaPFNum1')
	print()
	print('Numerlogy 2')
	generate_data_A('t2fullbufferrun/TdmaPFNum2')
	print()
	print('Numerlogy 3')
	generate_data_A('t2fullbufferrun/TdmaPFNum3')
	print()
	print()
	print('############### Task2 Part B ###############')
	print('Numerlogy 0')
	generate_data_B('t2fullbufferrun/TdmaPFNum0')
	print()
	print('Numerlogy 1')
	generate_data_B('t2fullbufferrun/TdmaPFNum1')
	print()
	print('Numerlogy 2')
	tputNum2, lossNum2, delayNum2 = generate_data_B('t2fullbufferrun/TdmaPFNum2')
	print()
	print('Numerlogy 3')
	tputNum3, lossNum3, delayNum3 = generate_data_B('t2fullbufferrun/TdmaPFNum3')
	print()
	print('Average of 2 and 3')
	for i in range(6):
		print(f'For UE {i+1}')
		tputNum2[i] = round((tputNum2[i] +tputNum3[i])/2,3)
		lossNum2[i] = round((lossNum2[i] +lossNum3[i])/2,3)
		delayNum2[i] = round((delayNum2[i] +delayNum3[i])/2,3)
		print('througput: ', tputNum2[i])
		print('loss_rate: ', lossNum2[i])
		print('delay: ', delayNum2[i])	  


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
	tput_list_mobRR, p_mobRR = _cdf('old_data/Q8-mobileRR')
	tput_list_mobRR_new, p_mobRR_new = _cdf('uncommented_data/Q8-mobileRR')
	#tput_list_staRR, p_staRR = _cdf('Q8-staticRR')
	fig, ax = plt.subplots()
	ax.plot(tput_list_mobRR, p_mobRR, label = 'Mobile RR')
	ax.plot(tput_list_mobRR_new, p_mobRR_new, label = 'Mobile RR Uncommented')
	#ax.plot(tput_list_staRR, p_staRR, label = 'Static RR')
	"""plt.xlabel('Throughput')
	plt.ylabel('CDF')
	plt.legend()
	plt.show()"""



	#PF
	tput_list_mobPF, p_mobPF = _cdf('old_data/Q8-mobilePF')
	tput_list_mobPF_new, p_mobPF_new = _cdf('uncommented_data/Q8-mobilePF')
	#tput_list_staPF, p_staPF = _cdf('Q8-staticPF')
	#fig, ax = plt.subplots()
	ax.plot(tput_list_mobPF, p_mobPF, label = 'Mobile PF')
	ax.plot(tput_list_mobPF_new, p_mobPF_new, label = 'Mobile PF Uncommented')
	#ax.plot(tput_list_staPF, p_staPF, label = 'Static PF')
	plt.xlabel('Throughput')
	plt.ylabel('CDF')
	plt.legend()
	plt.show()

	exit()

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
	parser.add_argument("-t", "--task", help="specify T1, T2, T3 or Q8, Q9")
	parser.add_argument("-f", "--file_path", help="Path of file to be used")
	args = parser.parse_args()
	print(args)

	if args.task == 'T1':
		Task1()
	elif args.task == 'T2':
		Task2()
	elif args.task == 'Q8':
		Q8()
	elif args.task == 'Q9':
		Q9()
	else:
		print('Please specify the task to be performed')


