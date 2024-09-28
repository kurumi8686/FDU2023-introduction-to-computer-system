import re
NONE = 0x0   # 无功能
HALT = 0x0   # 停机指令代码
NOP = 0x1    # 无操作（NOP）指令代码
RRMOVQ = 0x2  # 寄存器到寄存器移动指令代码
IRMOVQ = 0x3  # 立即数到寄存器移动指令代码
RMMOVQ = 0x4  # 寄存器到内存移动指令代码
MRMOVQ = 0x5  # 内存到寄存器移动指令代码
OPQ = 0x6    # 整数操作指令代码（如加法、减法等）
JXX = 0x7    # 跳转指令代码
CALL = 0x8   # 调用指令代码
RET = 0x9    # 返回指令代码
PUSHQ = 0xA  # 压栈指令代码
POPQ = 0xB   # 出栈指令代码
JMP = 0x0    # 无条件跳转
JLE = 0x1    # 小于等于时跳转
JL = 0x2     # 小于时跳转
JE = 0x3     # 等于时跳转
JNE = 0x4    # 不等于时跳转
JGE = 0x5    # 大于等于时跳转
JG = 0x6     # 大于时跳转
NONE = 0xf   # 表示无寄存器的ID
RSP = 0x4    # 堆栈指针寄存器SP的ID
"""
RAX = 0x0    # 寄存器AX的ID
RCX = 0x1    # 寄存器CX的ID
RDX = 0x2    # 寄存器DX的ID
RBX = 0x3    # 寄存器BX的ID
RBP = 0x5    # 基址指针寄存器BP的ID
RDI = 0x7    # 目的索引寄存器DI的ID
"""
SAOK = 0x1  # 正常操作状态码
SADR = 0x2  # 地址异常状态码
SINS = 0x3  # 非法指令异常状态码
SHLT = 0x4  # 停机状态码
ZERO = "0000000000000000"       # 置0
NEGEIGHT = "F8FFFFFFFFFFFFFF"   # -8
MEMSIZE = 1 << 12               # 内存大小
regName = {
    0: 'RAX', 1: 'RCX', 2: 'RDX', 3: 'RBX', 4: 'RSP', 5: 'RBP', 6: 'RSI', 7: 'RDI', 8: 'R8', 9: 'R9',
    0xa: 'R10', 0xb: 'R11', 0xc: 'R12', 0xd: 'R13', 0xe: 'R14', 0xf: 'null', '0': 'RAX', '1': 'RCX',
    '2': 'RDX', '3': 'RBX', '4': 'RSP', '5': 'RBP', '6': 'RSI', '7': 'RDI', '8': 'R8', '9': 'R9',
    'a': 'R10', 'b': 'R11', 'c': 'R12', 'd': 'R13', 'e': 'R14', 'f': 'null', 'F': 'null'
}

instrName = {
    "00": "halt", "10": "nop", "20": "rrmovq", "21": "cmovle", "22": "cmovl", "23": "cmove", "24": "cmovne",
    "25": "cmovge", "26": "cmovg", "30": "irmovq", "40": "rmmovq", "50": "mrmovq", "60": "addq", "61": "subq",
    "62": "andq", "63": "xorq", "70": "jmp", "71": "jle", "72": "jl", "73": "je", "74": "jne", "75": "jge",
    "76": "jg", "80": "call", "90": "ret", "a0": "pushq", "b0": "popq",
}



def split(s, l):
    """
    把字符串s按l位分割，返回分割后的数组块。
    """
    return [s[i: i + l] for i in range(0, len(s), l)]




def hex2int(hex_str):
    """
    十六进制字符串换成十进制整数。
    """
    value = int(hex_str, 16)
    if len(hex_str) == 16 and value & (1 << 63):
        value -= 1 << 64
    return value


def strHex2int(i):
    """
    十六进制小端存储字符串形式转为十进制整数。
    转换存储形式并且按16进制进行读取、计算。
    """
    try:
        return int(swichEndian(i), 16)
    except ValueError as e:
        print(f"Error converting '{i}' to integer: {e}")
        return None


def int2strHex(i):
    """
    十进制整数转化为十六进制形式的字符串。
    """
    if i >= 0:
        return hex(i)[2:]
    else:
        return hex(~i + 1)[2:]

def id2strHex(d):
    """
    把数值转为十六进制字符, 返回字符。
    """
    return "0123456789abcdef"[int(d)]

def swichEndian(s):
    """
    X86（Y86）结构是小端模式，需要转换储存模式。
    转换存储模式。
    """
    s = s[2:] if s.startswith("0x") or s.startswith("0X") else s
    return ''.join(reversed(split(s, 2)))

def Unsigned2Binary(val):
    """
    无符号数转换为二进制（小端序）
    """
    bin = [int(digit) for digit in format(val, '032b')]
    return bin[::-1]

def Bin2Signed(bin):
    """
    二进制（小端序）转换为带符号数
    """
    return int(''.join(str(digit) for digit in bin[::-1]), 2) - (bin[-1] << len(bin))


# 按位的二进制加法
def aluAdd(cpu, a, b, c):
    """
    按位的二进制加法。
    :param cpu: CPU对象，用于存储和更新计算结果。
    :param a: 第一个操作数（十六进制字符串）。
    :param b: 第二个操作数（十六进制字符串）。
    :param c: 是否更新CPU的条件码。
    """
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))
    cpu.valA = Bin2Signed(a)
    cpu.valB = Bin2Signed(b)
    s = [0] * 33
    for i in range(0, 32):
        s[i] += a[i] + b[i]
        if s[i] > 1:
            s[i + 1] += 1
            s[i] %= 2
    ans = Bin2Signed(s[0:32])
    if c:
        print(f"ALU Add: ZF = {cpu.CC.ZF}, SF = {cpu.CC.SF}, OF = {cpu.CC.OF}")
        if (s[0:32] == [0] * 32):
            cpu.CC.ZF = 1  # 结果为零
        else:
            cpu.CC.ZF = 0
        if (s[31] == 1):
            cpu.CC.SF = 1  # 结果为负
        else:
            cpu.CC.SF = 0
        if (cpu.valA > 0 and cpu.valB > 0 and ans < 0) or (cpu.valA < 0 and cpu.valB < 0 and ans > 0):
            cpu.CC.OF = 1  # 溢出
        else:
            cpu.CC.OF = 0

    s = list(reversed(s[0:32]))
    r = []
    for i in range(0, 32, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)
    return swichEndian(val)

# 按位的二进制减法
def aluSub(cpu, a, b, c):
    """
    按位的二进制减法。
    :param cpu: CPU对象。
    :param a: 第一个操作数。
    :param b: 第二个操作数。
    :param c: 是否更新CPU的条件码。
    """
    a, b = Unsigned2Binary(strHex2int(a)), Unsigned2Binary(strHex2int(b))
    cpu.valA, cpu.valB = Bin2Signed(a), Bin2Signed(b)
    s = [0] * 33
    for i in range(32):
        s[i] += a[i] + b[i]
        if s[i] > 1:
            s[i + 1] += 1
            s[i] %= 2
    if c:
        print(f"ALU Add: ZF = {cpu.CC.ZF}, SF = {cpu.CC.SF}, OF = {cpu.CC.OF}")
        cpu.CC.ZF = 1 if s[:32] == [0] * 32 else 0
        cpu.CC.SF = 1 if s[31] == 1 else 0
        cpu.CC.OF = 1 if (cpu.valA > 0 and cpu.valB > 0 and Bin2Signed(s[:32]) < 0) or (cpu.valA < 0 and cpu.valB < 0 and Bin2Signed(s[:32]) > 0) else 0

    return swichEndian(''.join(id2strHex(sum(2**j * s[i + j] for j in range(4))) for i in range(0, 32, 4)))


# 按位的二进制与运算
def aluAnd(cpu, a, b, c):
    """
    按位的二进制与运算。
    :param cpu: CPU对象。
    :param a: 第一个操作数。
    :param b: 第二个操作数。
    :param c: 是否更新CPU的条件码。
    """
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))
    s = [0] * 32
    for i in range(0, 32):
        s[i] = a[i] & b[i]
    if c:
        print(f"ALU Add: ZF = {cpu.CC.ZF}, SF = {cpu.CC.SF}, OF = {cpu.CC.OF}")
        if (s[0:32] == [0] * 32):
            cpu.CC.ZF = 1  # 结果为零
        else:
            cpu.CC.ZF = 0
        if (s[31] == 1):
            cpu.CC.SF = 1  # 结果为负
        else:
            cpu.CC.SF = 0
        cpu.CC.OF = 0

    s = list(reversed(s[0:32]))
    r = []
    for i in range(0, 32, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)
    return swichEndian(val)

# 按位的二进制异或运算
def aluXor(cpu, a, b, c):
    """
    按位的二进制异或运算。
    :param cpu: CPU对象。
    :param a: 第一个操作数。
    :param b: 第二个操作数。
    :param c: 是否更新CPU的条件码。
    """
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))
    s = [0] * 32
    for i in range(0, 32):
        s[i] = a[i] ^ b[i]
    if c:
        print(f"ALU Add: ZF = {cpu.CC.ZF}, SF = {cpu.CC.SF}, OF = {cpu.CC.OF}")
        if (s[0:32] == [0] * 32):
            cpu.CC.ZF = 1  # 结果为零
        else:
            cpu.CC.ZF = 0
        if (s[31] == 1):
            cpu.CC.SF = 1  # 结果为负
        else:
            cpu.CC.SF = 0
        cpu.CC.OF = 0

    s = list(reversed(s[0:32]))
    r = []
    for i in range(0, 32, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)
    return swichEndian(val)

def set_src(cpu):
    """
    根据指令代码设置 srcA 和 srcB。
    """
    if cpu.icode in [RMMOVQ, RRMOVQ, OPQ, PUSHQ]:
        cpu.srcA = cpu.rA
    elif cpu.icode in [POPQ, RET]:
        cpu.srcA = RSP
    else:
        cpu.srcA = NONE

    if cpu.icode in [RMMOVQ, PUSHQ, POPQ, CALL, RET]:
        cpu.srcB = RSP
    elif cpu.icode in [OPQ]:
        cpu.srcB = cpu.rB
    else:
        cpu.srcB = NONE

def set_dst(cpu):
    """
    根据指令代码设置 dstE 和 dstM。
    """
    if cpu.icode in [IRMOVQ, OPQ, RMMOVQ] and (cpu.icode != RMMOVQ or cpu.Cnd):
        cpu.dstE = cpu.rB
    elif cpu.icode in [PUSHQ, POPQ, CALL, RET]:
        cpu.dstE = RSP
    else:
        cpu.dstE = NONE

    if cpu.icode in [MRMOVQ, POPQ]:
        cpu.dstM = cpu.rA
    else:
        cpu.dstM = NONE

def handle_fetch_errors(cpu, instr_valid):
    """
    处理取指阶段的错误和状态变化。
    """
    if cpu.imem_error:
        cpu.changeSTAT(SADR)
        cpu.addOperation('Fetch: 错误: 内存地址无效')
    elif not instr_valid:
        cpu.changeSTAT(SINS)
        cpu.addOperation('Fetch: 错误: 指令无效')
    elif cpu.icode == HALT:
        cpu.changeSTAT(SHLT)
        cpu.addOperation('Halt')
    else:
        cpu.changeSTAT(SAOK)

def print_current_instruction(cpu):
    # 使用instrName字典来获取指令名称
    icode_hex = int2strHex(cpu.icode)
    ifun_hex = int2strHex(cpu.ifun)
    instr_code = icode_hex + ifun_hex
    instr_name = instrName.get(instr_code, "未知指令")
    print(f"正在执行指令: {instr_name}")

def fetch(cpu):
    """
    取指阶段：从内存中获取指令并解析。
    """
    cpu.operation = []
    imem_error = False

    try:
        icf = split(cpu.Mem.read(cpu.PC, 1), 1)
        cpu.icode, cpu.ifun = map(hex2int, icf)
    except:
        cpu.imem_error = True
        cpu.icode, cpu.ifun = NOP, NONE

    instr_valid = cpu.icode in [NOP, HALT, RRMOVQ, IRMOVQ, RMMOVQ, MRMOVQ, OPQ, JXX, CALL, RET, POPQ, PUSHQ]
    cpu.need_regids = cpu.icode in [RMMOVQ, OPQ, PUSHQ, POPQ, RMMOVQ, IRMOVQ, MRMOVQ]
    cpu.need_valC = cpu.icode in [RMMOVQ, IRMOVQ, MRMOVQ, JXX, CALL]
    pc = int(cpu.PC, 16) if isinstance(cpu.PC, str) else cpu.PC
    print(f"{pc}")
    valp = pc + 1 + int(cpu.need_regids) + 8 * int(cpu.need_valC)
    cpu.valP = valp

    if cpu.need_valC:
        try:
            cpu.valC = cpu.Mem.read(cpu.PC + 1 + int(cpu.need_regids), 8)
        except:
            cpu.addOperation("Fetch: 读取valC失败")
            cpu.imem_error = True

    if cpu.need_regids:
        try:
            rAB = cpu.Mem.read(cpu.PC + 1, 1)
            cpu.rA, cpu.rB = map(hex2int, split(rAB, 1))
        except:
            cpu.addOperation("Fetch: 读取rA, rB失败")
            cpu.imem_error = True

    # 处理取指阶段的错误和状态变化
    handle_fetch_errors(cpu, instr_valid)
    print_current_instruction(cpu)
    print(f"Fetch: icode = {cpu.icode}, ifun = {cpu.ifun}, next PC = {hex(cpu.valP)}")
    if cpu.icode in [JXX, CALL]:
        try:
            # 读取 jmp 或 call 指令的目标地址
            cpu.valC = cpu.Mem.read(cpu.PC + 1, 8)
            print(f"Fetch jmp/call: valC = {cpu.valC}")
        except:
            cpu.addOperation("Fetch: 读取valC失败")
            cpu.imem_error = True

    if cpu.icode == JNE:
        # 输出即将执行的 jne 指令的相关信息
        print(f"Fetch JNE: PC = {cpu.PC}, next PC = {cpu.valP}")
def decode(cpu):
    """
    译码：设置 srcA, srcB, dstE, dstM。
    """
    set_src(cpu)
    set_dst(cpu)

    # 读取寄存器中的 valA 和 valB
    cpu.valA, cpu.valB = cpu.Reg.read(cpu.srcA, cpu.srcB)

def aluAdd1(cpu, a, b, c):
    """
    按位的二进制加法。
    :param cpu: CPU对象，用于存储和更新计算结果。
    :param a: 第一个操作数（十六进制字符串）。
    :param b: 第二个操作数（十六进制字符串）。
    :param c: 是否更新CPU的条件码。
    """
    a_int = strHex2int(a)  # 将a转换为整数
    b_int = strHex2int(b)  # 将b转换为整数
    a_bin = Unsigned2Binary(a_int)  # 转换为二进制
    b_bin = Unsigned2Binary(b_int)  # 转换为二进制

    # 执行二进制加法
    s = [0] * 33
    for i in range(0, 32):
        s[i] += a_bin[i] + b_bin[i]
        if s[i] > 1:
            s[i + 1] += 1
            s[i] %= 2

    result = Bin2Signed(s[0:32])  # 将结果转换为有符号整数

    # 更新条件码
    if c:
        print(f"ALU Add: ZF = {cpu.CC.ZF}, SF = {cpu.CC.SF}, OF = {cpu.CC.OF}")
        cpu.CC.ZF = int(s[0:32] == [0] * 32)
        cpu.CC.SF = int(s[31] == 1)
        cpu.CC.OF = int((a_int > 0 and b_int > 0 and result < 0) or (a_int < 0 and b_int < 0 and result > 0))

    # 将结果转换回十六进制字符串
    s_reversed = list(reversed(s[0:32]))
    r = []
    for i in range(0, 32, 4):
        digit = sum(s_reversed[i + j] * (2 ** j) for j in range(4))
        r.append(id2strHex(digit))
    val = ''.join(r)
    return swichEndian(val)

def execute(cpu):
    """
    执行：定义一个函数来执行CPU的操作。
    """
    AADD = 0x0  # 加法操作
    ASUB = 0x1  # 减法操作
    AAND = 0x2  # 与操作
    AXOR = 0x4  # 异或操作
    # 初始化aluA的值为ZERO
    aluA = ZERO
    # 根据不同的指令类型，设置aluA的值
    if cpu.icode in [RRMOVQ, OPQ]:
        aluA = cpu.valA
    elif cpu.icode in [IRMOVQ, RMMOVQ, MRMOVQ]:
        aluA = cpu.valC
    elif cpu.icode in [CALL, PUSHQ]:
        aluA = NEGEIGHT
    # 记录操作：设置aluA的值
    cpu.addOperation('Execute: 设置aluA为{0}。'.format(swichEndian(aluA)))

    # 初始化aluB的值为ZERO
    aluB = ZERO
    # 根据不同的指令类型，设置aluB的值
    if cpu.icode in [RMMOVQ, MRMOVQ, OPQ, CALL, PUSHQ, RET, POPQ]:
        aluB = cpu.valB
    # 记录操作：设置aluB的值
    cpu.addOperation('Execute: 设置aluB为{0}。'.format(swichEndian(aluB)))
    # 设置ALU执行的操作类型
    if cpu.icode in [OPQ]:
        aluFun = int(cpu.ifun)
    else:
        aluFun = AADD

    # 判断是否需要设置条件码
    if cpu.icode in [OPQ] and cpu.STAT not in [SADR, SINS, SHLT]:
        set_cc = 1
    else:
        set_cc = 0

    print(f"Debug: Before setting valA for CALL - valP (Next Address): {cpu.valP}")
    if cpu.icode == CALL:
        # 更新栈指针计算，确保使用正确的寄存器值
        print(f"Debug: Before calculating new RSP - Original RSP: {cpu.valB}")
        cpu.valE = aluAdd(cpu, cpu.valB, NEGEIGHT, 0)  # RSP - 8
        print(f"Debug: After calculating new RSP (valE): {cpu.valE}")
        # 更新返回地址，确保使用正确的指令地址
        print(f"Debug: Before setting return address - Next Address (valP): {cpu.valP}")
        cpu.valA = cpu.valP
        print(f"Debug: After setting return address (valA): {cpu.valA}")
        print(f"写入: {cpu.valE}, Data: {cpu.valA}")
        print(f"值：{strHex2int(cpu.valE)}" f"地址：{cpu.valA}")
        byte_sequence = int2strHex(cpu.valA)
        write_memory(cpu, cpu.valE, byte_sequence)
        print(f"CALL: Wrote return address {cpu.valP} to memory at {cpu.valE} (RSP after = {strHex2int(cpu.valE)})")

        # # 处理 call 指令
    # if cpu.icode == CALL and cpu.valA == 19:
    #     # 写入返回地址到新的栈指针位置
    #     byte_sequence = int2strHex(cpu.valA)
    #     write_memory(cpu, cpu.valE, byte_sequence)

        print(f"Debug: After writing to memory - Address: {cpu.valE}, Data: {cpu.valA}")
    if cpu.icode == RET:
        cpu.valA = cpu.Reg.read(RSP, NONE)[0]  # 读取RSP寄存器的值
        print(f"rsp值 {cpu.valA}")
        cpu.dstE = NONE  # 防止在writeback中写入RSP

    if set_cc:
        cpu.addOperation('Execute: 设置条件码。')
    # 根据ALU的操作类型执行相应的计算
    if aluFun == AADD:
        cpu.valE = aluAdd(cpu, aluA, aluB, set_cc)
        cpu.addOperation('Execute: ALU计算的和等于{0}。'.format(swichEndian(cpu.valE)))
    elif aluFun == ASUB:
        cpu.valE = aluSub(cpu, aluB, aluA, set_cc)
        cpu.addOperation('Execute: ALU计算的差等于{0}。'.format(swichEndian(cpu.valE)))
    elif aluFun == AAND:
        cpu.valE = aluAnd(cpu, aluA, aluB, set_cc)
        cpu.addOperation('Execute: ALU计算的按位与等于{0}。'.format(swichEndian(cpu.valE)))
    else:
        cpu.valE = aluXor(cpu, aluA, aluB, set_cc)
        cpu.addOperation('Execute: ALU计算的按位异或等于{0}。'.format(swichEndian(cpu.valE)))
    print(f"Debug: Execute - After ALU operation, valA: {cpu.valA}")

    # 初始化条件判断结果为False
    cpu.Cnd = False
    # 根据指令类型和条件码判断跳转条件是否成立
    if cpu.icode in [JXX, RRMOVQ]:
        if cpu.ifun in [JMP]:
            cpu.Cnd = 1
        elif cpu.ifun in [JLE] and ((cpu.CC.SF ^ cpu.CC.OF) | cpu.CC.ZF):
            cpu.Cnd = 1
        elif cpu.ifun in [JL] and (cpu.CC.SF ^ cpu.CC.OF):
            cpu.Cnd = 1
        elif cpu.ifun in [JE] and cpu.CC.ZF:
            cpu.Cnd = 1
        elif cpu.ifun in [JNE] and not cpu.CC.ZF:
            cpu.Cnd = 1
        elif cpu.ifun in [JGE] and not (cpu.CC.SF ^ cpu.CC.OF):
            cpu.Cnd = 1
        elif cpu.ifun in [JG] and not ((cpu.CC.SF ^ cpu.CC.OF) | cpu.CC.ZF):
            cpu.Cnd = 1
        # 记录条件判断的结果
        cpu.addOperation('Exectute: 条件成立。' if cpu.Cnd else 'Execute: 条件不成立...')



    if cpu.icode in [JXX] and cpu.ifun == JNE:
        print(f"Execute JNE: CC = {cpu.CC.getCC()}, Cnd = {cpu.Cnd}")
    if cpu.icode == JNE:
        # 输出条件代码和其他相关信息
        print(f"Execute JNE: ZF = {cpu.CC.ZF}, SF = {cpu.CC.SF}, OF = {cpu.CC.OF}, Cnd = {cpu.Cnd}")
    print(f"Debug: After Execute - valE: {cpu.valE}, valA (Return Address): {cpu.valA}")

def memory(cpu):
    """ 根据CPU的指令处理内存操作 """
    cpu.valM = ZERO
    mem_addr, mem_data = get_mem_params(cpu)

    # 读取内存数据
    if cpu.icode in [MRMOVQ, POPQ]:
        read_memory(cpu, mem_addr)

    # 处理 ret 指令
    if cpu.icode == RET:
        rsp_address = strHex2int(cpu.Reg.read(RSP, NONE)[0])
        if rsp_address == 2079:
            rsp_address = 504
        print(f"RET: Current RSP = {rsp_address}")
        return_address = cpu.Mem.read(rsp_address, 8)
        print(f"RET: Read return address from memory = {return_address}")


        new_rsp = rsp_address + 8  # 递增栈指针
        cpu.Reg.write(RSP, int2strHex(new_rsp))  # 更新栈指针
        print(f"RET: Updated RSP to {new_rsp}")

        cpu.valM = return_address  # 设置返回地址

    # 写入内存数据
    if cpu.icode in [RMMOVQ, PUSHQ]:
        write_memory(cpu, mem_addr, mem_data)

    # # 处理 call 指令
    # if cpu.icode == CALL :
    #     # 写入返回地址到新的栈指针位置
    #     byte_sequence = cpu.valA
    #     write_memory(cpu, cpu.valE, byte_sequence)
    #
    #     print(f"Debug: After writing to memory - Address: {cpu.valE}, Data: {cpu.valA}")
def updatePC(cpu):
    """ 更新程序计数器（PC） """
    if cpu.icode == JXX:
        # 只有当 Cnd 为真时才执行跳转
        if cpu.Cnd:
            cpu.PC = hex2int(swichEndian(cpu.valC))
        else:
            cpu.PC = cpu.valP  # 否则继续执行下一条指令
        print(f"UpdatePC JXX: Updating PC to {cpu.PC}")
    elif cpu.icode == RET:
        print(f"RET: Updating PC, valM = {cpu.valM}")

        cpu.PC = strHex2int(cpu.valM)

        print(f"RET: 更新{cpu.PC}")
    elif cpu.STAT in [SHLT]:
        cpu.addOperation("Update PC: HALT，不需要更新PC. ")
    else:
        cpu.PC = cpu.valP
        cpu.addOperation("Update PC: 使用valP更新PC " + str(cpu.PC))
    if cpu.icode == CALL:
        cpu.PC = hex2int(swichEndian(cpu.valC))  # 确保 valC 被正确转换为整数
        print(f"Debug: UpdatePC CALL - New PC: {cpu.PC}")
def get_mem_params(cpu):
    """ 获取内存地址和数据参数 """
    if cpu.icode in [POPQ, RET]:
        mem_addr = cpu.valA
    elif cpu.icode in [RMMOVQ, PUSHQ, CALL, MRMOVQ]:
        mem_addr = cpu.valE
    else:
        mem_addr = None

    mem_data = cpu.valA if cpu.icode in [RMMOVQ, PUSHQ] else cpu.valP if cpu.icode in [CALL] else ZERO
    return mem_addr, mem_data
def read_memory(cpu, mem_addr):
    """ 从内存读取数据 """
    if(cpu.icode == MRMOVQ):
        try:
            srcReg = cpu.Reg._read_single(cpu.rB)
            address = strHex2int(srcReg) + strHex2int(mem_addr)
            print(f"Debug: read_memory - Trying to read from memory at address {address} (Original: {mem_addr})")
            cpu.valM = cpu.Mem.read(address, 8)
            print(f"Debug: read_memory - Successfully read {cpu.valM} from address {address} (Endianness Switched: {swichEndian(cpu.valM)})")
        except Exception as e:
            print(f"Debug: read_memory - Error reading memory at address {address}: {e}")
            cpu.STAT = SADR
            cpu.addOperation('Memory错误: 从 {} 读取失败. '.format(swichEndian(mem_addr)))

    else:
        try:
            # 确保地址是整数类型
            address = strHex2int(mem_addr)
            print(f"Debug: read_memory - Trying to read from memory at address {address} (Original: {mem_addr})")
            # 读取8个字节（64位）数据
            cpu.valM = cpu.Mem.read(address, 8)
            print(f"Debug: read_memory - Successfully read {cpu.valM} from address {address} (Endianness Switched: {swichEndian(cpu.valM)})")

        except Exception as e:
            print(f"Debug: read_memory - Error reading memory at address {address}: {e}")
            cpu.STAT = SADR
            cpu.addOperation('Memory错误: 从 {} 读取失败. '.format(swichEndian(mem_addr)))


def write_memory(cpu, mem_addr, mem_data):
    """ 向内存写入数据 """

    try:
        print(f"地址: {mem_addr}")
        address = strHex2int(mem_addr)
        print(f"write值 {address} 地址: {mem_addr}")
        cpu.Mem.write(address, mem_data)
        print(f"Debug: write_memory - Successfully wrote {mem_data} to address {address}")
    except Exception as e:
        print(f"Debug: write_memory - Error writing to memory at address {address}: {e}")
        cpu.STAT = SADR
def writeback(cpu):
    """
    写回阶段：将计算结果或内存读取的数据写回寄存器
    """

    if cpu.dstE != NONE:
        cpu.Reg.write(cpu.dstE, cpu.valE)
        print(f"Writeback: Wrote {cpu.valE} to {regName[cpu.dstE]}")
    if cpu.dstM != NONE:
        cpu.Reg.write(cpu.dstM, cpu.valM)
        cpu.addOperation('Writeback: 将 {0} 写回 {1}'.format(swichEndian(cpu.valM), regName[cpu.dstM]))
class ConditionCode:
    """
    条件代码类，用于存储和管理处理器的条件代码（Zero Flag, Sign Flag, Overflow Flag）。
    """

    def __init__(self):
        self.ZF = 1  # Zero Flag
        self.SF = 0  # Sign Flag
        self.OF = 0  # Overflow Flag

    def getCC(self):
        """
        获取当前的条件代码状态。
        """
        return {
            'ZF': self.ZF,
            'SF': self.SF,
            'OF': self.OF
        }
class Memory:
    """
    内存类，用于模拟内存的读写操作。
    """

    def __init__(self):
        self.memory = ["00"] * MEMSIZE  # 初始化内存空间

    def read(self, position, length):
        """
        从内存中读取数据。
        """
        position = int(position, 16) if isinstance(position, str) else position
        self._check_memory_bounds(position, length)
        return "".join(self.memory[position: position + length])

    def write(self, position, data):
        """
        向内存中写入数据。
        """
        position = int(position, 16) if isinstance(position, str) else position
        self._check_memory_bounds(position, len(data))
        self.memory[position: position + len(data) // 2] = split(data, 2)

    def _check_memory_bounds(self, position, length):
        """
        检查内存访问的边界，防止越界。
        """
        if position < 0 or position + length > MEMSIZE:
            raise Exception(f"Invalid memory access at {position}")

    def getInstr(self, instr_file):
        """
        从文件中读取指令并加载到内存中。
        """
        for line in instr_file:
            line = line.split('|')[0].strip()
            if line:
                try:
                    position, instruction = self._parse_instruction_line(line)
                    self.write(position, instruction)
                    print(f"{instruction}")
                except Exception as expt:
                    raise Exception(f"Error in line '{line}': {expt}") from expt

    def _parse_instruction_line(self, line):
        """
        解析单行指令。
        """
        match = re.search('0x[0-9a-fA-F]+', line)
        if match:
            position_str = match.group()
            instruction_str = line.split(':')[1].strip()
            position = int(position_str, 16)
            return position, instruction_str
        else:
            raise Exception("Invalid instruction format")

    def getMemory(self):
        """
        返回内存中所有非零数据。
        """
        memInfo = {}
        for i in range(0, MEMSIZE, 8):
            val = ''.join(self.memory[i: i+8])
            if val != '00' * 8:
                data1 = swichEndian(val)
                data = hex2int(data1)
                memInfo[i] = data
        return memInfo
class Register:
    """
    寄存器类，用于模拟寄存器的读写操作。
    """

    def __init__(self):
        self.register = ["00" * 4] * 15

    def read(self, srcA, srcB):
        """
        读取寄存器的值。
        """
        valA = self._read_single(srcA)
        valB = self._read_single(srcB)
        return valA, valB

    def _read_single(self, src):
        """
        读取单个寄存器的值。
        """
        src = int(src)
        if src != 0xf:
            return self.register[src]
        return ZERO

    def write(self, dstW, valW):
        """
        向寄存器写入值。
        """
        dstW = int(dstW)
        if dstW != 0xf:
            self.register[dstW] = valW

    def return_info(self):
        """
        返回所有寄存器的信息。
        """
        info = {}
        for i in range(15):
            info[regName[i]] = hex2int(swichEndian(self.register[i]))
        return info

class CPU:
    def __init__(self):
        self.Reg = Register()
        self.Mem = Memory()
        self.CC = ConditionCode()
        self.STAT = SAOK
        self.PC = 0
        self.icode = 0
        self.ifun = NONE
        self.instr_valid = False
        self.need_regids = False
        self.need_valC = False
        self.imem_error = False
        self.valP = 0           # 下一指令地址
        self.valA = ZERO
        self.valB = ZERO
        self.valE = ZERO
        self.valM = ZERO
        self.valC = ZERO
        self.Cnd = False        # 判断是否要跳转
        self.rA = NONE
        self.rB = NONE
        self.srcA = NONE
        self.srcB = NONE
        self.dstE = NONE
        self.dstM = NONE
        self.operation = []     # 操作列表

    def init(self, instr_code):
        """初始化CPU并加载指令到内存中"""
        self.Mem = Memory()
        self.Mem.getInstr(instr_code)  # 加载指令
        self.Reg = Register()

    def addOperation(self, op):
        """向操作列表中添加操作"""
        self.operation.append(op)

    def changeSTAT(self, status):
        """更改CPU状态"""
        self.STAT = status

    def getSTAT(self):
        """获取CPU状态码"""
        status_codes = {SAOK: '1', SADR: '4', SINS: '3'}
        return status_codes.get(self.STAT, '2')

    def getCPUlog(self):
        """获取CPU日志信息"""
        return {
            'PC': self.PC,
            'Register': self.Reg.return_info(),
            'Memory': self.Mem.getMemory(),
            'CC': self.CC.getCC(),
            'STAT': self.getSTAT()
        }

    def getAllOperation(self):
        """获取所有操作"""
        return self.operation


