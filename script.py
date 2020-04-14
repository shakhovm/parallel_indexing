import os
import sys


def file_reader_time(file_name):
    with open(file_name) as f:
        lst = f.read().split("\n")

    first = []
    second = []
    third = []
    lst.pop()
    for elem in lst[::3]:
        first.append(int(elem.split(':')[1]))
    for elem in lst[1::3]:
        second.append(int(elem.split(':')[1]))
    for elem in lst[2::3]:
        third.append(int(elem.split(':')[1]))
    return first, second, third


def res_reader(file_name):
    with open(file_name) as f:
        lst = f.read().split("\n")
    value = lst[0]
    counter = 1
    for i in range(1, len(lst)):
        if lst[i] == value:
            break
        counter += 1
    return lst, counter


if __name__ == "__main__":
    n = int(sys.argv[1])

    if len(sys.argv) > 2:
        conf_file = sys.argv[2]
    else:
        conf_file = "conf.dat"
    with open("time.txt", 'w') as f:
        f.write("")
        pass # to delete content
    ending = str()
    for i in range(n):
        if os.name == "nt":
            ending = '.exe'
        os.system(f"cmake-build-debug/word_indexing{ending} {conf_file}>> time.txt")
    first, second, third = file_reader_time("time.txt")
    m_1, m_2, m_3 = min(first), min(second), min(third)
    res_lst, length = res_reader("files/res_n.txt")

    set_res = set(res_lst)
    if len(set_res) == length:
        print("Results are the same")
    else:
        print("Results are different")
    print(f"Reading (microseconds) : {m_1}\nIndexing (milliseconds) : {m_2}\nTotal (milliseconds) : {m_3}")