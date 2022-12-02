import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-t", "--target_file", type=str, default="data/test_lbl.txt", help="Path to file of ground truth labels")
parser.add_argument("-r", "--result_file", type=str, default="result.txt", help="Path to file of predicted labels from HMM")
parser.add_argument("-o", "--output_file", type=str, default="accuracy.txt", help="Path to file for recording the accuracy")
parser.add_argument("-a", "--append", type=bool, default=True, help="Append the accuracy to the end of output file")
parser.add_argument("-n", "--name", type=str, default=None, help="Name of the experiment")
args = parser.parse_args()

if __name__ == "__main__":
    with open(args.target_file, "r") as target_file:
        gt_label = [s[:-1] for s in target_file.readlines()]
        with open(args.result_file, "r") as result_file:
            label = [s.split(" ")[0] for s in result_file.readlines()]

            count = 0.0
            for i in range(len(gt_label)):
                count += gt_label[i] == label[i]
            
            with open(args.output_file, "a" if args.append else "w") as output_file:
                name = args.name if args.name is not None else args.result_file
                output_file.write("%s %f\n" % (name, count / len(label)))
            print(count / len(label), end="")