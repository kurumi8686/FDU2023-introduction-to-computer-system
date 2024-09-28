import io
import os
import json
from CPU import *

log_list = []
def run(cpu, i, test_case, file_name, output_directory):
    while cpu.getSTAT() not in ['2', '3', '4']:
        fetch(cpu)
        decode(cpu)
        memory(cpu)
        execute(cpu)
        writeback(cpu)
        updatePC(cpu)
        log = cpu.getCPUlog()
        log_list.append(log.copy())

    # 使用提供的文件名，替代从文件对象的内容中提取文件名的部分
    # base_name = os.path.splitext(os.path.basename(file_name))[0]
    # output_file = os.path.join(output_directory, f"{base_name}")
    # 将执行日志保存到指定的JSON文件中
    # with open(output_file, 'w') as file:
        # json.dump(log_list, file, sort_keys=False, indent=4, separators=(',', ': '))

def main():
    current_path = os.path.abspath(".")
    test_path = current_path + "/test"
    os.chdir(test_path)

    output_directory = os.path.join(current_path, "temp_answer")
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    tests = [
        ("prog10", io.StringIO(open("prog10.yo", "r").read())),
        ("prog1", io.StringIO(open("prog1.yo", "r").read())),
        ("asumr", io.StringIO(open("asumr.yo", "r").read())),
        ("poptest", io.StringIO(open("poptest.yo", "r").read())),
        ("asum", io.StringIO(open("asum.yo", "r").read())),
        ("prog7", io.StringIO(open("prog7.yo", "r").read())),
        ("pushtest", io.StringIO(open("pushtest.yo", "r").read())),
        ("j-cc", io.StringIO(open("j-cc.yo", "r").read())),
        ("pushquestion", io.StringIO(open("pushquestion.yo", "r").read())),
        ("prog6", io.StringIO(open("prog6.yo", "r").read())),
        ("prog9", io.StringIO(open("prog9.yo", "r").read())),
        ("abs-asum-cmov", io.StringIO(open("abs-asum-cmov.yo", "r").read())),
        ("prog5", io.StringIO(open("prog5.yo", "r").read())),
        ("prog4", io.StringIO(open("prog4.yo", "r").read())),
        ("prog8", io.StringIO(open("prog8.yo", "r").read())),
        ("cjr", io.StringIO(open("cjr.yo", "r").read())),
        ("prog3", io.StringIO(open("prog3.yo", "r").read())),
        ("asumi", io.StringIO(open("asumi.yo", "r").read())),
        ("abs-asum-jmp", io.StringIO(open("abs-asum-jmp.yo", "r").read())),
        ("prog2", io.StringIO(open("prog2.yo", "r").read())),
        ("ret-hazard", io.StringIO(open("ret-hazard.yo", "r").read()))
    ]

    i = 0
    for file_name, test_case in tests:
        myCPU = CPU()
        myCPU.init(test_case)
        log_list = []
        run(myCPU, i, test_case, file_name, output_directory)
        i += 1

main()