# 整型和浮点型混合运算
def mixed_arithmetic_operations():
    int_num = 10
    float_num = 3.14
    
    # 加法
    addition = int_num + float_num
    # 减法
    subtraction = int_num - float_num
    # 乘法
    multiplication = int_num * float_num
    # 除法
    division = int_num / float_num
    # 取整除法
    floor_division = int_num // float_num
    
    print([addition, subtraction, multiplication, division, floor_division])
    
    # 加法
    addition = float_num + int_num
    # 减法
    subtraction = float_num - int_num
    # 乘法
    multiplication = float_num * int_num
    # 除法
    division = float_num / int_num
    # 取整除法
    floor_division = float_num // int_num
    
    print([addition, subtraction, multiplication, division, floor_division])
    
    # 加法
    addition = int_num + int_num
    # 减法
    subtraction = int_num - int_num
    # 乘法
    multiplication = int_num * int_num
    # 除法
    division = int_num / int_num
    # 取整除法
    floor_division = int_num // int_num
    
    print([addition, subtraction, multiplication, division, floor_division])

    # 加法
    addition = float_num + float_num
    # 减法
    subtraction = float_num - float_num
    # 乘法
    multiplication = float_num * float_num
    # 除法
    division = float_num / float_num
    # 取整除法
    floor_division = float_num // float_num
    
    print([addition, subtraction, multiplication, division, floor_division])

if __name__ == "__main__":
    print("Mixed Arithmetic Operations:")
    mixed_arithmetic_operations()