import numpy as np 
from itertools import permutations

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
    kinds = ["b", "i", "u", "f", "c"]
    item_sizes = ["1", "2", "4", "8", "16", "32"]
    byte_orders = ["<", ">", "=", "|"]
    invalid_combinations = []
    valid_combinations = []
    valid_combinations += kinds[1:]
    valid_combinations += ["|b1", "|i1", "|u1"]
    valid_combinations += [b + k for k in kinds[1:] for b in byte_orders[:-1]]
    valid_combinations += ["i2", "i4", "i8", "u2", "u4", "u8", "f4", "f8", "f16", "c8", "c16", "c32"]
    valid_combinations += [b + ks for b in byte_orders[:-1] for ks in ["i2", "i4", "i8", "u2", "u4", "u8", "f4", "f8", "f16", "c8", "c16", "c32"]]
    with open("./test_resources/valid_dtype_strings.txt", "w") as valid_file:
        valid_file.write("\n".join(valid_combinations))
    for k in kinds:
        for i in item_sizes:
            for b in byte_orders:
                x = b + k + i
                for j in range(1, len(x)):
                    combinations = ["".join(a) for a in permutations(x, j)]
                    combinations = [c for c in combinations if c not in valid_combinations]
                    invalid_combinations += combinations
    with open("./test_resources/invalid_dtype_strings.txt", "w") as invalid_file:
        invalid_file.write("\n".join(invalid_combinations))
    #dt = np.dtype("<i8")
    #for dt in dtypes:
    #    arr = np.zeros(10, dtype=dt)
    #    print(arr.dtype.str)
    #    #np.save("./test/{}.npy".format(arr.dtype.name), arr)