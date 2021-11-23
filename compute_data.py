#Logic to compute averages over multiple simulation runs
import json
import argparse
import matplotlib.pyplot as plt
import numpy as np

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

  #This is for single iteration
  data = json_data_list[0]
  for i in range(6):
    througput[i] = througput[i] + float(data["Flow"+str(i+1)]["Throughput"].split()[0])
    loss_rate[i] = loss_rate[i] + float(data["Flow"+str(i+1)]["Loss Rate"])
    delay[i] = delay[i] + float(data["Flow"+str(i+1)]['Mean delay'].split()[0])

  """
  #This is for multiple iterations

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
    print('delay: ', round(delay[i]/5, 3))"""

  for i in range(6):
    print(f'For UE {i+1}')
    print('througput: ', througput[i])
    print('loss_rate: ', loss_rate[i])
    print('delay: ', delay[i])  
  
  #return [i/5 for i in througput], [i/5 for i in loss_rate], [i/5 for i in delay]
  return througput, loss_rate, delay


if __name__ == "__main__":

  parser = argparse.ArgumentParser()
  parser.add_argument("-t", "--task", help="specify T1, T2, T3")
  parser.add_argument("-f", "--file_path", help="Path of file to be used")
  args = parser.parse_args()

  if args.task == 'T1' or args.task == 'T2':
    print('############### Part A ###############')
    generate_data_A(args.file_path)
    print()
    print('############### Part B ###############')
    generate_data_B(args.file_path)

  elif args.task == 'T3':
    generate_data_A(args.file_path)
  else:
    print('Please specify the task to be performed')

