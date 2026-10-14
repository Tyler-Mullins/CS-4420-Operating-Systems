CS 4420 Assignment 6
Interaction Diagram
------------------------------------------------------------------------------------------------------------
<img width="1126" height="357" alt="image" src="https://github.com/user-attachments/assets/37fc6bc4-5c6b-4503-b420-c32669219155" />

From my understanding, the way these processes work are via the pipes. Judge process uses the "signal" pipe to
communicate with the voter process that it is time to go. The associated voter generates a random vote that 
is then send to the judge process via the "pipes" pipe. This is demonstrated above.

Compilation instructions
------------------------------------------------------------------------------------------------------------
1. Once the file is accessed, compile using "g++ -Wall assignment6.cc"
2. This will create a file called "a.out".
3. Run the executable using the command "./a.out".

Screenshot of running program
------------------------------------------------------------------------------------------------------------
<img width="544" height="253" alt="image" src="https://github.com/user-attachments/assets/42a03807-73d5-491b-bf1e-c615c9eba22e" />
