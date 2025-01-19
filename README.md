# Optimum-Scheduling-Under-Resource-Timing-Constraints-ASAP-ALAP-Algorithm-LP-solver.
1. Read the benchmark files (.asapalap) and store it in your own defined data structure 
2. Form the linear equations using the information in the data structure and print in a file(ilpfile) 
3. Use lp solver to solve the inequalities and store the output of lpsolver in file.
   1. lpsover command:
      ```C
      lp_solve "ilpfile"
      ``` 
 5. The two functions(get_token,isdelim) can be used for reading the benchmark files (.asapalap).
    1. Using these two functions in your implementation is optional. 
