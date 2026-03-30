import random

def generate_rnn_data(filename="in.txt", num_samples=1000, bit_length=8):
    """
    生成 rnn-bit 所需的二进制加法训练数据
    :param filename: 输出文件名
    :param num_samples: 生成多少组加法算式
    :param bit_length: 每个数字的位数 (例如 8位, 16位)
    """
    print(f"正在生成 {num_samples} 组 {bit_length}位 二进制加法数据...")
    
    with open(filename, "w", encoding="utf-8") as f:
        for _ in range(num_samples):
            # 1. 随机生成两个整数 (范围 0 到 2^bit_length - 1)
            a = random.randint(0, (1 << bit_length) - 1)
            b = random.randint(0, (1 << bit_length) - 1)
            
            # 2. 计算和
            sum_val = a + b
            
            # 3. 格式化为二进制字符串
            # 这里的格式是: 加数A + 加数B + 结果(多一位以防进位)
            # zfill 确保高位补0，保持固定长度
            bin_a = format(a, f'0{bit_length}b')
            bin_b = format(b, f'0{bit_length}b')
            # 结果通常需要 bit_length + 1 位来容纳进位
            bin_sum = format(sum_val, f'0{bit_length + 1}b')
            
            # 4. 拼接并写入文件
            # 格式: 00101... (输入A) 01010... (输入B) 01000... (输出Sum)
            line = f"{bin_a}{bin_b}{bin_sum}\n"
            f.write(line)
            
    print(f"成功！数据已保存至 {filename}")
    print(f"示例数据 (第一行): 打开文件查看前几个字符...")

# 配置参数
# 生成 10000 组数据，每组是 8位二进制数相加
generate_rnn_data(num_samples=10000, bit_length=8)