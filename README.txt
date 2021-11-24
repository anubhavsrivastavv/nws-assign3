copy the file asg.cc to ns-3-dev/scratch/
All scripts are present in folder /ns-3-dev/scripts


TASK 1
======
PART - A 
---------

Execute fullbuffer case  using script T1-FB from ns-3-dev folder as shown below:
#. T1-FB
Output files will be generated in the folder ns-3-dev/outputFiles/ with the name T1-FB-<schedulername> (eg : T1-FB-TdmaRR, T1-FB-OfdmaPF etc)
Our execution traces are present in folder ns-3-dev/outputFiles/Task1-FullBuffer.

Execute non-fullbuffer case  using script T1-NFB from ns-3-dev folder as shown below:
#. T1-NFB
Output files will be generated in the folder ns-3-dev/outputFiles with the name T1-NFB-<schedulername> (eg : T1-NFB-TdmaRR, T1-NFB-OfdmaPF etc)
Our execution traces are present in folder ns-3-dev/outputFiles/Task1-NonFullBuffer.

PART - B
--------
Execute Part B using script T1-PartB from ns-3-dev folder as shown below:
#. T1-PartB
Output files will be generated in the folder ns-3-dev/outputFiles with the name T1-PartB-<schedulername> (eg :T1-PartB-TdmaRR, T1-PartB-TdmaPF etc)
Our execution traces are present in folder ns-3-dev/outputFiles/Task1-PartB/.


TASK 2
======
PART - A 
---------

Execute fullbuffer case  using script T2 from ns-3-dev folder as shown below:
#. T2
Output files will be generated in the folder ns-3-dev/outputFiles/ with the name T2-Numerology<Numerology> (eg : T2-Numerology0, T2-Numerology1 etc)
Our execution traces are present in folder ns-3-dev/outputFiles/Task2


TASK3
=====
Execute Task3 using script T3 from ns-3-dev folder as shown below:
#. T3
Output files will be generated in the folder ns-3-dev/outputFiles/ with the name T3-<speed>-<schedulername> (eg : T3-10-RR, T3-50-PF etc)
Our execution traces are present in folder ns-3-dev/outputFiles/Task3.


Question 7
==========
Execution : ./waf --run "asg3 --REMP=true"
Plotting : gnuplot -p nr-rem--gnbs.txt nr-rem--ues.txt nr-rem--buildings.txt nr-rem--plot-rem.gnuplot

Question 8
==========
Execute Q8 using file Q8 from ns-3-dev folder as shown below:
#. Q8

Output files will be generated in outputFiles folder with name Q8-staticRR, Q8-staticPF, Q8-staticMR, Q8-mobileRR, Q8-mobilePF, Q8-mobileMR.
Our execution traces are present in folder ns-3-dev/outputFiles/Q8.

Question 9
==========
Execute Q9 using file Q9 from ns-3-dev folder as shown below:
#. Q9
Output files will be generated in outputFiles folder with name Q9-staticRR, Q9-staticPF, Q9-staticMR, Q9-mobileRR, Q9-mobilePF, Q9-mobileMR
Our execution traces are present in folder ns-3-dev/outputFiles/Q9.


---------------------------------------------
Data Generation and Graph creation
---------------------------------------------

-> Getting the usable data from the output files:

	We've created a python script compute_data.py which takes two command line arguments
		-t : Task to be performed (T1, T2 OR T3 corresponding to Task1, Task2 and Task3)

		-f: The path of output file from which the data has to be obtained
		
		Outputs:
		Part A:
		average aggregate_throughput
		average packet loss
		average packet delay
		
		Part B:
		Per UE througput, loss_rate, delay

		Example:
		$ python compute_data.py -t T1 -f 'outputFiles/T1-FB-OfdmaMR'
	
-> Plotting the graphs:

	We've created a python script plot_graphs.py which makes use of matplotlib for graph creation. This script takes in one command line 
	argument:
		-t : Task to be performed (Q8, Q9 or Q10)
		
		Example:
		$ python compute_data.py -t Q10
	
		Outputs:
		CDF-Throughput plots for Q8 and Q9		
		SNR plots for Q10
		
	Note*: plot_graphs.py does not accept file_path as command line args so we need to directly change the file path in code and then run