Out main code file - "asg3.cc" is present in ns-3-dev/scratch folder. 

                                             ns-3-dev
                                                |
                                                |
                    ------------------------------------------------------
                    |                           |                        |                 
                    |                           |                        |                 
                    |                           |                        |                 
                  outputFiles                scripts                   plots              
                    |                           |
                    |                           |
     ------------------------------        -------------
     |    |    |    |    |    |   |        |           |
     |    |    |    |    |    |   |        |           |       
     |    |    |    |    |    |   |        |           |
    T1   T2   T3   Q7   Q8   Q9  Q10    exec_scr   dataGen_scr



outputFiles - This folder contains all our execution traces seggregated into task specific folders.  
scripts - This folder contains
            1. Execution scripts in exec_scr folder. 
            2. Data generation and plotting scripts in dataGen_scr. 

plots - This folder contains the various plots that we have generated for different tasks

For all execution, copy the task specific execution script from /ns-3-dev/scripts/exec_scr to /ns-3-dev/  and then execute the script from ns-3-dev folder.

TASK 1
======
PART - A 
---------
FullBuffer Case - Execute fullbuffer case using script scripts/exec_scr/T1-FB from the ns-3-dev folder as shown below:
#. T1-FB
On running the script, output files will be generated in the folder ns-3-dev/outputFiles/T1 folder with the names T1-FB-<schedulername> (eg : T1-FB-TdmaRR, T1-FB-OfdmaPF etc).


NonFullBuffer Case - Execute non-fullbuffer case using script scripts/exec_scr/T1-NFB from ns-3-dev folder as shown below:
#. T1-NFB
On running the script, output files will be generated in the folder ns-3-dev/outputFiles/T1 folder with the names T1-NFB-<schedulername> (eg : T1-NFB-TdmaRR, T1-NFB-OfdmaPF etc).

PART - B
--------
Execute Part B using script scripts/exec_scr/T1-PartB from ns-3-dev folder as shown below:
#. T1-PartB
On running the script, Output files will be generated in the folder ns-3-dev/outputFiles/T1 with the name T1-PartB-<schedulername> (eg :T1-PartB-TdmaRR, T1-PartB-TdmaPF etc)


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
Execute Task3 using script scripts/exec_scr/T3 from ns-3-dev folder as shown below:
#. T3
On running the script, output files will be generated in the folder ns-3-dev/outputFiles/T3 with the name T3-<speed>-<schedulername> (eg : T3-10-RR, T3-50-PF etc)


Question 7
==========
Execute Q7 using the script scripts/exec_scr/Q7 from ns-3-dev folder as shown below.
#. Q7
This will generate the REM plots (three plots) in the folder outputFiles/Q7/

Question 8
==========
Execute Q8 using script scripts/exec_scr/Q8 from the ns-3-dev folder as shown below:
#. Q8
On running the script, Output files will be generated in the folder ns-3-dev/outputFiles/Q8 folder with name Q8-staticRR, Q8-staticPF, Q8-staticMR, Q8-mobileRR, Q8-mobilePF, Q8-mobileMR.

Question 9
==========
Execute Q9 using script scripts/exec_scr/Q9 from the ns-3-dev folder as shown below:
#. Q9
On running the script, Output files will be generated in the folder ns-3-dev/outputFiles/Q9 folder with names Q9-staticRR, Q9-staticPF, Q9-staticMR, Q9-mobileRR, Q9-mobilePF, Q9-mobileMR

Question 10
===========
Execute Q10 using the script scripts/exec_scr/Q10 from the ns-3-dev folder as show below:
. Q10
This generates the SNR values and the instantaneous throughputs for three schedulers in the files SNR, Q10-throughPutsTdmaRR, Q10-throughPutsTdmaPR and Q10-throughPutsTdmaMR respectively.


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
