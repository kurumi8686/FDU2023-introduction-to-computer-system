import re
FNONE = 0x0   # 无功能
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
MOVLE = 0x1
MOVL = 0x2
MOVE = 0x3
MOVNE = 0x4
MOVGE = 0x5
MOVG = 0x6
RNONE = 0xf   # 表示无寄存器的ID
VNONE = '0000000000000000'
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
AADD = 0x0
ASUB = 0x1
AAND = 0x2
AXOR = 0x4
ZERO = "0000000000000000"       # 置0
EIGHT = "0800000000000000"
NEGEIGHT = "F8FFFFFFFFFFFFFF"   # -8
MEMSIZE = 1 << 12               # 内存大小
regName = {
    0: 'rax', 1: 'rcx', 2: 'rdx', 3: 'rbx', 4: 'rsp', 5: 'rbp', 6: 'rsi', 7: 'rdi', 8: 'r8', 9: 'r9',
    0xa: 'r10', 0xb: 'r11', 0xc: 'r12', 0xd: 'r13', 0xe: 'r14', 0xf: 'null',
    '0': 'RAX', '1': 'RCX', '2': 'RDX', '3': 'RBX', '4': 'RSP', '5': 'RBP', '6': 'RSI', '7': 'RDI', '8': 'R8', '9': 'R9',
    'a': 'R10', 'b': 'R11', 'c': 'R12', 'd': 'R13', 'e': 'R14', 'f': 'null', 'F': 'null'
}
instrName = {
    "00": "halt", "10": "nop", "20": "rrmovq", "21": "MOVLE", "22": "MOVL", "23": "MOVE", "24": "MOVNE",
    "25": "MOVGE", "26": "MOVG", "30": "irmovq", "40": "rmmovq", "50": "mrmovq", "60": "addq", "61": "subq",
    "62": "andq", "63": "xorq", "70": "jmp", "71": "jle", "72": "jl", "73": "je", "74": "jne", "75": "jge",
    "76": "jg", "80": "call", "90": "ret", "a0": "pushq", "b0": "popq"
}


# 把const中0-f的数值转为十六进制字符, 返回字符
def id2strHex(d):
    return "0123456789abcdef"[int(d)]


# 十六进制小端存储字符串形式转为十进制整数。转换存储形式并且按16进制进行读取、计算。
def split(s, l):
    res = [s[i: i + l] for i in range(0, len(s), l)]
    return res


# X86（Y86）结构是小端模式，需要转换储存模式
# 转换存储模式
def swichEndian(s):
    if s.startswith("0x") or s.startswith("0X"):
        s = s[2:]  # 去除开头的'0x'字段
    return ''.join(reversed(split(s, 2)))  # s按两位两位分隔, 进行倒序


# 十六进制字符串换成十进制整数
def hex2int(i):
    res = int(i, 16)    # 读取16进制字符串
    max_int_str = "7fffffffffffffff"
    MAXINT = int(max_int_str, 16)   # 计算32可表示的最大正数
    # print("MAXINT:")
    # print(MAXINT)
    if res > MAXINT:        # 针对溢出情况的处理: 用32位二进制数能表示的最大正数减去得出的结果并+1
        j = i[0:1]
        i_firbit = int(j, 16)
        i_firbit -= 8       # 十六进制最高位 -8
        i_f = id2strHex(i_firbit)
        j = i_f + i[1:]
        res = int(j, 16)
        ret_val = '-' + str(MAXINT - res + 1)   # 加上负号
        return int(ret_val)     # 返回int类型
    else:
        return res      # 没有发生溢出, 直接返回


# 十六进制小端存储字符串形式转为十进制整数: 转换存储形式并且按16进制进行读取、计算
def strHex2int(i):
    return int(swichEndian(i), 16)


# 十进制整数转化为十六进制形式的字符串
def int2strHex(i):
    if i >= 0:
        s = hex(i)
        s = s[2:]       # hex() 返回'0xXXXXXXXX'模式串, 将开头的'0x'字段去除
        # if len(s) > 8:
        #     s = s[-8:]
        # s = "0" * (8 - len(s)) + s
    else:
        i = ~i + 1
        s = hex(i)
    return s


def Unsigned2Binary(val):
    bin = []
    tmp = val
    while tmp != 0:
        bin.append(tmp % 2)
        tmp //= 2
    if len(bin) < 64:
        bin += [0] * (64 - len(bin))
    return bin[0:64]


# 把二进制转换成带符号数, 为小端序
def Bin2Signed(bin):
    val = 0
    bas = 1
    for i in range(0, len(bin) - 1):
        val = val + bas * int(bin[i])
        bas *= 2
    if bin[-1] == 1:
        val -= bas
    return val


# 按位的二进制加法
def aluAdd(cpu, a, b, c):
    # 将十六进制字符串转换为无符号二进制列表
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))

    # 设置ALU的操作数A和B
    cpu.aluA = Bin2Signed(a)
    cpu.aluB = Bin2Signed(b)

    # 初始化结果列表s
    s = [0] * 65

    # 进行按位加法运算
    for i in range(0, 64):
        s[i] += a[i] + b[i]
        if s[i] > 1:
            s[i + 1] += 1
            s[i] %= 2

    # 将结果转换为有符号二进制表示
    ans = Bin2Signed(s)

    # 根据条件更新CPU的条件码
    if c:
        if (s[0:64] == [0] * 64 and cpu.aluA != 0 and cpu.aluB != 0):
            cpu.CC.ZF = True
        else:
            cpu.CC.ZF = False
        if (s[63] == 1):
            cpu.CC.SF = True
        else:
            cpu.CC.SF = False
        if (cpu.aluA > 0 and cpu.aluB > 0 and ans < 0) or (cpu.aluA < 0 and cpu.aluB < 0 and ans > 0):
            cpu.CC.OF = True
        else:
            cpu.CC.OF = False

    # 将结果列表s反转并转换为十六进制字符串表示
    s = list(reversed(s[0:64]))
    r = []
    for i in range(0, 64, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)

    # 转换字节顺序并返回结果
    return swichEndian(val)


# 按位的二进制减法
def aluSub(cpu, a, b, c):
    # 将十六进制字符串转换为无符号二进制列表
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))

    # 设置ALU的操作数A和B
    cpu.aluA = Bin2Signed(a)
    cpu.aluB = Bin2Signed(b)

    # 取反并加1，实现减法运算
    b = list(map(lambda x: 0 if x == 1 else 1, b))
    b[0] += 1

    # 初始化结果列表s
    s = [0] * 65

    # 进行按位加法运算
    for i in range(0, 64):
        s[i] += a[i] + b[i]
        if s[i] > 1:
            s[i + 1] = 1
            s[i] %= 2

    # 将结果转换为有符号二进制表示
    ans = Bin2Signed(s[0:64])

    # 根据条件更新CPU的条件码
    if c:
        if (s[0:64] == [0] * 64 and cpu.aluA != 0 and cpu.aluB != 0):
            cpu.CC.ZF = True
        else:
            cpu.CC.ZF = False
        if (s[63] == 1):
            cpu.CC.SF = True
        else:
            cpu.CC.SF = False
        if (cpu.aluA > 0 and cpu.aluB > 0 and ans < 0) or (cpu.aluA < 0 and cpu.aluB < 0 and ans > 0):
            cpu.CC.OF = True
        else:
            cpu.CC.OF = False

    # 将结果列表s反转并转换为十六进制字符串表示
    s = list(reversed(s[0:64]))
    r = []
    for i in range(0, 64, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)

    # 转换字节顺序并返回结果
    return swichEndian(val)


# 按位的二进制与运算
def aluAnd(cpu, a, b, c):
    # 将十六进制字符串转换为无符号二进制列表
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))

    # 初始化结果列表s
    s = [0] * 65

    # 进行按位与运算
    for i in range(0, 64):
        s[i] = a[i] & b[i]

    # 根据条件更新CPU的条件码
    if c:
        if (s[0:64] == [0] * 64):
            cpu.CC.ZF = True
        else:
            cpu.CC.ZF = False
        if (s[63] == 1):
            cpu.CC.SF = True
        else:
            cpu.CC.SF = False
        cpu.CC.OF = False

    # 将结果列表s反转并转换为十六进制字符串表示
    s = list(reversed(s[0:64]))
    r = []
    for i in range(0, 64, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)

    # 转换字节顺序并返回结果
    return swichEndian(val)


# 按位的二进制异或运算
def aluXor(cpu, a, b, c):
    # 将十六进制字符串转换为无符号二进制列表
    a = Unsigned2Binary(strHex2int(a))
    b = Unsigned2Binary(strHex2int(b))

    # 初始化结果列表s
    s = [0] * 65

    # 进行按位异或运算
    for i in range(0, 64):
        s[i] = a[i] ^ b[i]

    # 根据条件更新CPU的条件码
    if c:
        if (s[0:64] == [0] * 64):
            cpu.CC.ZF = True
        else:
            cpu.CC.ZF = False
        if (s[31] == 1):
            cpu.CC.SF = True
        else:
            cpu.CC.SF = False
        cpu.CC.OF = False

    # 将结果列表s反转并转换为十六进制字符串表示
    s = list(reversed(s))
    r = []
    for i in range(0, 64, 4):
        t = 0
        for j in range(0, 4):
            t = t * 2 + s[i + j]
        r.append(id2strHex(t))
    val = ''.join(r)

    # 转换字节顺序并返回结果
    return swichEndian(val)


def fetch(cpu):
    cpu.operation = []
    try:    # 取icode和ifun
        icf = split(cpu.Mem.read(cpu.PC, 1), 1)
        cpu.icode = hex2int(icf[0])
        cpu.ifun = hex2int(icf[1])
    except:
        cpu.imem_error = True
        cpu.icode = NOP
        cpu.ifun = FNONE

    instr_valid = cpu.icode not in [NOP, HALT, RRMOVQ, IRMOVQ, RMMOVQ, MRMOVQ, OPQ, JXX, CALL, RET, POPQ, PUSHQ]
    cpu.need_regids = cpu.icode in [RMMOVQ, OPQ, PUSHQ, POPQ, RRMOVQ, IRMOVQ, MRMOVQ]
    cpu.need_valC = cpu.icode in [RMMOVQ, IRMOVQ, MRMOVQ, JXX, CALL]
    valp = cpu.PC + 1 + int(cpu.need_regids) + 8 * int(cpu.need_valC)
    # print(valp)
    # valp = str(valp)
    cpu.valP = valp
    # print(cpu.valP)

    cpu.valC = ZERO
    if cpu.need_valC:
        try:
            cpu.valC = cpu.Mem.read(cpu.PC + 1 + int(cpu.need_regids), 8)
        except:
            cpu.imem_error = True

    if cpu.need_regids:
        try:
            cpu.rA, cpu.rB = split(cpu.Mem.read(cpu.PC + 1, 1), 1)
            cpu.rA = hex2int(cpu.rA)
            cpu.rB = hex2int(cpu.rB)
        except:
            cpu.imem_error = True

    if cpu.imem_error:
        cpu.changeSTAT(SADR)
    elif instr_valid:
        cpu.changeSTAT(SINS)
    elif cpu.icode == HALT:
        cpu.changeSTAT(SHLT)
    else:
        cpu.changeSTAT(SAOK)
def decode(cpu):
    # 设置 srcA
    if cpu.icode in [RMMOVQ, RRMOVQ, OPQ, PUSHQ]:
        cpu.srcA = cpu.rA
    elif cpu.icode in [POPQ, RET]:
        cpu.srcA = RSP
    else:
        cpu.srcA = RNONE

    # 设置 srcB
    if cpu.icode in [OPQ, RMMOVQ, MRMOVQ]:
        cpu.srcB = cpu.rB
    elif cpu.icode in [PUSHQ, POPQ, CALL, RET]:
        cpu.srcB = RSP
    else:
        cpu.srcB = RNONE

    # 设置 dstM
    if cpu.icode in [MRMOVQ, POPQ]:
        cpu.dstM = cpu.rA
    else:
        cpu.dstM = RNONE

    # 读取 valA 和 valB
    cpu.valA, cpu.valB = cpu.Reg.read(cpu.srcA, cpu.srcB)
def execute(cpu):
    aluA = ZERO
    if cpu.icode in [RRMOVQ, OPQ]:
        aluA = cpu.valA
    elif cpu.icode in [IRMOVQ, RMMOVQ, MRMOVQ]:
        aluA = cpu.valC
    elif cpu.icode in [CALL, PUSHQ]:
        aluA = NEGEIGHT
    elif cpu.icode in [RET, POPQ]:
        aluA = EIGHT

    if cpu.icode in [RMMOVQ, MRMOVQ, OPQ, CALL, PUSHQ, RET, POPQ]:
        aluB = cpu.valB
    else:
        aluB = ZERO

    # 设置 ALU 应执行的操作
    if cpu.icode in [OPQ]:
        aluFun = int(cpu.ifun)
    else:
        aluFun = AADD

    if cpu.icode in [OPQ] and cpu.STAT not in [SADR, SINS, SHLT]:
        set_cc = True
    else:
        set_cc = False

    if aluFun == AADD:
        cpu.valE = aluAdd(cpu, aluA, aluB, set_cc)
    elif aluFun == ASUB:
        cpu.valE = aluSub(cpu, aluB, aluA, set_cc)
    elif aluFun == AAND:
        cpu.valE = aluAnd(cpu, aluA, aluB, set_cc)
    else:
        cpu.valE = aluXor(cpu, aluA, aluB, set_cc)

    cpu.Cnd = False
    if cpu.icode in [JXX, RRMOVQ]:
        if cpu.ifun in [JMP]:  # 0
            cpu.Cnd = True
        elif cpu.ifun in [MOVLE] and ((cpu.CC.SF ^ cpu.CC.OF) | cpu.CC.ZF):    # 1
            cpu.Cnd = True
        elif cpu.ifun in [MOVL] and (cpu.CC.SF ^ cpu.CC.OF):       # 2
            cpu.Cnd = True
        elif cpu.ifun in [MOVE] and cpu.CC.ZF:     # 3
            cpu.Cnd = True
        elif cpu.ifun in [MOVNE] and not cpu.CC.ZF:        # 4
            flag = not cpu.CC.ZF
            flag1 = cpu.CC.ZF
            cpu.Cnd = True
        elif cpu.ifun in [MOVGE] and not (cpu.CC.SF ^ cpu.CC.OF):      # 5
            cpu.Cnd = True
        elif cpu.ifun in [MOVG] and not ((cpu.CC.SF ^ cpu.CC.OF) | cpu.CC.ZF):     #6
            cpu.Cnd = True

    # 设置 dstE
    if cpu.icode in [IRMOVQ, OPQ]:
        cpu.dstE = cpu.rB
    elif cpu.icode in [RRMOVQ] and cpu.Cnd:
        cpu.dstE = cpu.rB
    elif cpu.icode in [PUSHQ, POPQ, CALL, RET]:
        cpu.dstE = RSP
    else:
        cpu.dstE = RNONE
def memory(cpu):
    cpu.valM = VNONE

    if cpu.icode in [POPQ, RET]:
        mem_addr = cpu.valB
    elif cpu.icode in [RMMOVQ, PUSHQ, MRMOVQ, CALL]:
        mem_addr = cpu.valE
    else:
        mem_addr = None

    if cpu.icode in [RMMOVQ, PUSHQ]:
        mem_data = cpu.valA
    elif cpu.icode in [CALL]:
        mem_data = int2strHex(cpu.valP)
    else:
        mem_data = VNONE

    if cpu.icode in [MRMOVQ, POPQ, RET]:
        read_flag = True
    else:
        read_flag = False
    if cpu.icode in [RMMOVQ, PUSHQ, CALL]:
        write_flag = True
    else:
        write_flag = False

    if read_flag:
        try:
            if isinstance(mem_addr, str):
                cpu.valM = cpu.Mem.read(strHex2int(mem_addr), 8)
            elif isinstance(mem_addr, int):
                cpu.valM = cpu.Mem.read(mem_addr, 8)
        except:
            cpu.STAT = SADR

    if write_flag:
        try:
            cpu.Mem.write(strHex2int(mem_addr), mem_data)
        except:
            cpu.STAT = SADR
def writeback(cpu):
    if cpu.dstE != RNONE:
        cpu.Reg.write(cpu.dstE, cpu.valE)
    if cpu.dstM != RNONE:
        cpu.Reg.write(cpu.dstM, cpu.valM)
def updatePC(cpu):
    # 如果当前指令是调用指令（CALL），更新程序计数器（PC）为valC的十六进制值对应的整数
    if cpu.icode in [CALL]:
        cpu.PC = strHex2int(cpu.valC)
    # 如果当前指令是条件跳转指令（JXX）且条件满足（Cnd为真），更新程序计数器（PC）为valC的十六进制值对应的整数
    elif cpu.icode in [JXX] and cpu.Cnd:
        cpu.PC = strHex2int(cpu.valC)
    # 如果当前指令是返回指令（RET），更新程序计数器（PC）为valM的字节序反转后的整数值
    elif cpu.icode in [RET]:
        cpu.PC = hex2int(swichEndian(cpu.valM))
    # 如果当前状态为停机状态（SHLT），程序计数器（PC）保持不变
    elif cpu.STAT in [SHLT]:
        cpu.PC = cpu.PC

    elif cpu.icode in [PUSHQ] and cpu.STAT in [SADR]:
        cpu.PC = cpu.PC
    # 对于其他情况，更新程序计数器（PC）为valP的整数值
    else:
        cpu.PC = cpu.valP


# 定义条件代码类
class ConditionCode:
    def __init__(self):
        self.ZF = True
        self.SF = False
        self.OF = False

    def getCC(self):
        cc = {
            'ZF': int(self.ZF),
            'SF': int(self.SF),
            'OF': int(self.OF)
        }
        return cc

# 定义寄存器类
class Register:
    def __init__(self):
        self.register = ["00" * 8] * 15

    def read(self, srcA, srcB):
        srcA = int(srcA)
        srcB = int(srcB)
        valA = VNONE
        valB = VNONE
        if srcA != 0xf:
            valA = self.register[srcA]
        if srcB != 0xf:
            valB = self.register[srcB]
        return valA, valB

    def write(self, dstW, valW):
        dstW = int(dstW)
        if dstW != 0xf:
            self.register[dstW] = valW

    def return_info(self):
        info = {}
        for i in range(0, 15):
            info[regName[i]] = hex2int(swichEndian(self.register[i]))
        return info

# 定义内存类
class Memory:
    def __init__(self):
        self.memory = ["00"] * MEMSIZE

    def read(self, position, length):
        if position < 0 or position >= MEMSIZE:
            raise Exception(("无效的内存访问：尝试从 %d 读取数据到 %d" % (position, position + length)))

        return "".join(self.memory[position: position + length])

    def write(self, position, data):
        if position < 0 or position >= MEMSIZE:
            raise Exception(("无效的内存访问：尝试从 %d 写入数据到 %d" % (position, position + len(data))))
        self.memory[position: position + len(data) // 2] = split(data, 2)

    def getInstr(self, instr_file):
        for line in instr_file:
            if line.find('|') != -1:
                line = line[:line.find('|')]
            if len(line) == 0 or line.isspace():
                continue
            position = None
            instruction = None
            try:
                position_str = re.search('0x[0-9a-fA-F]+', line).group()
                instruction_pos = line.find(':')
                instruction_str = line[instruction_pos + 1:]
                instruction = instruction_str.strip(' ')
                if len(position_str) != 0:
                    position = int(position_str, 16)
                    if len(instruction) != 0:
                        self.write(position, instruction)
            except Exception as expt:
                raise Exception("获取内存中的指令时出错：%s" % line) from expt

    def getMemory(self):
        memInfo = {}
        for i in range(0, MEMSIZE, 8):
            val = ''.join(self.memory[i: i + 8])
            if val != '00' * 8:
                data1 = swichEndian(val)
                data = hex2int(data1)
                memInfo[i] = data
        return memInfo

# 定义CPU类
class CPU:
    def __init__(self):
        self.Reg = Register()
        self.Mem = Memory()
        self.CC = ConditionCode()
        self.STAT = SAOK
        self.PC = 0
        self.icode = 0
        self.ifun = FNONE
        self.instr_valid = False
        self.need_regids = False
        self.need_valC = False
        self.imem_error = False
        self.valP = 0
        self.valA = ZERO
        self.valB = ZERO
        self.valE = ZERO
        self.valM = ZERO
        self.valC = ZERO
        self.Cnd = False
        self.rA = RNONE
        self.rB = RNONE
        self.srcA = RNONE
        self.srcB = RNONE
        self.dstE = RNONE
        self.dstM = RNONE
        self.aluA = 0
        self.aluB = 0

    def init(self, instr_code):
        self.Mem = Memory()
        self.Mem.getInstr(instr_code)
        self.Reg = Register()

    def changeSTAT(self, status):
        self.STAT = status

    def getSTAT(self):
        if self.STAT == SAOK:
            return '1'
        elif self.STAT == SADR:
            return '3'
        elif self.STAT == SINS:
            return '4'
        else:
            return '2'

    def getCPUlog(self):
        CPU_log = {
            'PC': self.PC,
            'REG': self.Reg.return_info(),
            'MEM': self.Mem.getMemory(),
            'CC': self.CC.getCC(),
            'STAT': int(self.getSTAT())
        }
        return CPU_log
