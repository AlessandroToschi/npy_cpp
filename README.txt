3 use cases:

1) the user wants to open a numpy array file stored on the disk and then it provides the path to it. 
Given that, the npy_array loads the array and the user can read/write the data.
The memory is freed when the object is destructed.
If the user makes a modification and want to make it permentantly then it has to save the file.
The file can be saved either by storing the previous path or by providing a new one.

2) the user creates a new numpy array given a shape vector.
Then the class allocates a vector with that shape and ready to be written and read.

3) the user creates a new numpy array given a shape vector and a data vector.
They must be of the same size.