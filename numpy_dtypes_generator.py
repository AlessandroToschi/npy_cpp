import numpy as np 

dtypes = [
    np.bool8,
    np.int8,
    np.int16,
    np.int32,
    np.int64,
    np.uint8,
    np.uint16,
    np.uint32,
    np.uint64,
    np.float16,
    np.float32,
    np.float64,
    np.float128,
    np.complex64,
    np.complex128,
    np.complex256
]

if __name__ == "__main__":
    for dt in dtypes:
        arr = np.zeros(10, dtype=dt)
        print(arr.dtype.str)
        #np.save("./test/{}.npy".format(arr.dtype.name), arr)