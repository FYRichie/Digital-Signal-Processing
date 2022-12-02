from matplotlib import pyplot as plt


ACC = "accuracy.txt"

if __name__ == "__main__":
    with open(ACC, "r") as acc_file:
        acc, iter = [], []
        for a in acc_file.readlines():
            _a = a.split(" ")[1:]
            iter.append(int(_a[0]))
            acc.append(float(_a[1][:-1]))
        
        plt.plot(iter, acc)
        plt.show()