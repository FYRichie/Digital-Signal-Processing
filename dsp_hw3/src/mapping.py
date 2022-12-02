import sys
from collections import defaultdict


if __name__ == "__main__":
    input_path, output_path = sys.argv[1:]
    zhu_big_mapping = defaultdict(lambda: "", {})
    with open(input_path, "r", encoding="big5-hkscs") as input_file:
        with open(output_path, "w", encoding="big5-hkscs") as output_file:
            for s in input_file.readlines():
                big5, zhu_in = s.split(" ")[:2]
                zhu_in = zhu_in.replace("\n", "").split("/")

                for zhu in zhu_in:
                    zhu_big_mapping[zhu[0]] += big5
                zhu_big_mapping[big5] = big5

            for key in list(zhu_big_mapping.keys()):
                output_file.write("%s\t%s\n" % (key, " ".join(zhu_big_mapping[key])))
