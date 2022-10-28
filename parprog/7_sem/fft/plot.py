import matplotlib.pyplot as plt
import csv
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Plot file in format CSV")
    parser.add_argument("File", help="File name", type=str)
    args = parser.parse_args()

    X = []
    Y = []
 
    with open(args.File, 'r') as datafile:
        plotting = csv.reader(datafile, delimiter=' ')
     
        for ROWS in plotting:
            X.append(float(ROWS[0]))
            Y.append(float(ROWS[1]))
 
    plt.plot(X, Y)
    plt.title("file: " + args.File)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.show()
