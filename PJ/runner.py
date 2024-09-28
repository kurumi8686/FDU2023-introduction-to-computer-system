import io
import os
import json
from CPU import *

def run(cpu, i, test_case, file_name, output_directory):
    global log_list
    log_list = []
    while cpu.getSTAT() not in ['2', '4', '3']:
        fetch(cpu)
        decode(cpu)
        execute(cpu)
        memory(cpu)
        writeback(cpu)
        updatePC(cpu)
        log = cpu.getCPUlog()
        log_list.append(log)



    base_name = os.path.splitext(os.path.basename(file_name))[0]
    output_file = os.path.join(output_directory, f"{base_name}")
    # 将执行日志保存到指定的JSON文件中
    with open(output_file, 'w') as file:
        json.dump(log_list, file, sort_keys=False, indent=4, separators=(',', ': '))


# 生成答案输出文件


if __name__ == "__main__":
    current_path = os.path.abspath(".")
    test_path = current_path + "/test"
    os.chdir(test_path)

    output_directory = os.path.join(current_path, "temp_answer")
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    tests = [
        ("asum", io.StringIO(open("asum.yo", "r").read())),
        ("prog6", io.StringIO(open("prog6.yo", "r").read())),
        ("prog1", io.StringIO(open("prog1.yo", "r").read())),
        ("prog2", io.StringIO(open("prog2.yo", "r").read())),
        ("prog3", io.StringIO(open("prog3.yo", "r").read())),
        ("prog4", io.StringIO(open("prog4.yo", "r").read())),
        ("prog5", io.StringIO(open("prog5.yo", "r").read())),
        ("prog7", io.StringIO(open("prog7.yo", "r").read())),
        ("prog8", io.StringIO(open("prog8.yo", "r").read())),
        ("prog9", io.StringIO(open("prog9.yo", "r").read())),
        ("prog10", io.StringIO(open("prog10.yo", "r").read())),
        ("pushtest", io.StringIO(open("pushtest.yo", "r").read())),
        ("ret-hazard", io.StringIO(open("ret-hazard.yo", "r").read())),
        ("cjr", io.StringIO(open("cjr.yo", "r").read())),
        ("pushquestion", io.StringIO(open("pushquestion.yo", "r").read())),
        ("poptest", io.StringIO(open("poptest.yo", "r").read())),
        ("asumr", io.StringIO(open("asumr.yo", "r").read())),
        ("asumi", io.StringIO(open("asumi.yo", "r").read())),
        ("j-cc", io.StringIO(open("j-cc.yo", "r").read())),
        ("abs-asum-cmov", io.StringIO(open("abs-asum-cmov.yo", "r").read())),
        ("abs-asum-jmp", io.StringIO(open("abs-asum-jmp.yo", "r").read()))
    ]

    i = 0
    for file_name, test_case in tests:
        myCPU = CPU()
        myCPU.init(test_case)
        run(myCPU, i, test_case, file_name, output_directory)
        i = i + 1
