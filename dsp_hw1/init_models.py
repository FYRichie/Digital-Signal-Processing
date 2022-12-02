import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("-m", "--model_list", type=str, default="modellist.txt", help="Path to training models")
parser.add_argument("-i", "--model_init", type=str, default="model_init.txt", help="Path to model initial parameters")
args = parser.parse_args()

if __name__ == "__main__":
    with open(args.model_list, "r") as model_list_file:
        for mf in model_list_file.readlines():
            os.system("cp %s %s" % (args.model_init, mf[:-1]))