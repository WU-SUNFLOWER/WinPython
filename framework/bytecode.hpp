#ifndef ByteCode_Hpp
#define ByteCode_Hpp

enum ByteCode : uint8_t {
    Pop_Top = 0x01,  // ����������ջ��Ԫ��
    Dup_Top = 0x04,
    Binary_Multiply = 0x14,
    Binary_Module = 0x16,
    Binary_Add = 0x17,
    Binary_Subtract = 0x18,
    Binary_Subscr = 0x19,
    Store_Map = 0x36,
    Inplace_Add = 0x37,
    Store_Subscr = 0x3c,
    Delete_Subscr = 0x3d,
    Get_Iter = 0x44,
    Print_Item = 0x47,
    Print_NewLine = 0x48,
    Break_Loop = 0x50,
    Load_Locals = 0x52,
    Return_Value = 0x53,
    Pop_Block = 0x57,
    Build_Class = 0x59,
    // �ж�ָ���Ƿ񺬲εı߽�ֵ
    // op < Have_Argument ��ʾָ�������
    // op >= Have_Argument ��ʾָ�����
    Have_Argument = 0x5a,
    Store_Name = 0x5a,  // ����ջ��Ԫ�س�ջ������ֵ������ָ���ı���
    Unpack_Sequence = 0x5c,
    For_Iter = 0x5d,
    Store_Attr = 0x5f,
    Store_Global = 0x61,
    Load_Const = 0x64,  // �������б��еĲ��������ص�ջ��
    Load_Name = 0x65,  // ����������ı���ѹջ
    Build_Tuple = 0x66,  // ����һ��Ԫ�飬���������ջ�ϵ������ٸ�Ԫ�ش����Ԫ��
    Build_List = 0x67,
    Build_Map = 0x69,
    Load_Attr = 0x6a, // �������������������ѹջ
    Compare_Op = 0x6b,
    Jump_Forward = 0x6e,
    Jump_Absolute = 0x71,
    Pop_Jump_If_False = 0x72,
    Load_Global = 0x74,
    Setup_Loop = 0x78,
    Load_Fast = 0x7c,
    Store_Fast = 0x7d,
    Call_Function = 0x83,
    Make_Function = 0x84,
    Make_Closure = 0x86,  // �����հ���������ָ��Ĳ�����ʾ�β�Ĭ��ֵ�ĸ���
    Load_Closure = 0x87,  // ����free variable��ջ��
    Load_Deref = 0x88,  // ��ȡcell variable�������ã��ټ��ص�ջ��
    Store_Deref = 0x89,  // ����ջ��Ԫ�أ����䴢��Ϊcell variable
    Binary_Divide = 0x15,
    Binary_FloorDivide = 0x1a,
};

enum CompareCondition : uint8_t {
    Less = 0x00,
    Less_Equal = 0x01,
    Equal = 0x02,
    Not_Equal = 0x03,
    Greater = 0x04,
    Greater_Equal = 0x05,
    In = 0x06,
    Not_In = 0x07,
    Is = 0x08,
    Is_Not = 0x09,
    Exc_Match = 0x10,
};

#endif