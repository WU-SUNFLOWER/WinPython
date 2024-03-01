#ifndef ByteCode_Hpp
#define ByteCode_Hpp

enum ByteCode : uint8_t {
    Binary_Add = 0x17,
    Print_Item = 0x47,
    Print_NewLine = 0x48,
    Return_Value = 0x53,
    // 判断指令是否含参的边界值
    // op < Have_Argument 表示指令不含参数
    // op >= Have_Argument 表示指令含参数
    Have_Argument = 0x5a,
    Store_Name = 0x5a,  // 弹出栈顶元素出栈，并赋值给参数指定的变量
    Load_Const = 0x64,  // 将常量列表中的操作数加载到栈中
    Load_Name = 0x65,  // 将参数代表的变量压栈
    Compare_Op = 0x6b,
    Pop_Jump_If_False = 0x72,
    Jump_Forward = 0x6e,

};

enum CompareCondition : uint8_t {
    Less = 0x00,
    Less_Equal = 0x01,
    Equal = 0x02,
    Not_Equal = 0x03,
    Greater = 0x04,
    Greater_Equal = 0x05,
    In = 0x05,
    Not_In = 0x06,
    Is = 0x07,
    Is_Not = 0x08,
    Exc_Match = 0x09,
};

#endif