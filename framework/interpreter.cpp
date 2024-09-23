#include "interpreter.hpp"
#include "Universe.hpp"
#include "FrameObject.hpp"
#include "PyFunction.hpp"
#include <cstdio>
#include <cstdlib>
#include "PyObject.hpp"
#include "Block.hpp"
#include "nativeFunctions.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "MethodKlass.hpp"
#include "PyMethod.hpp"
#include "StringKlass.hpp"
#include "CellKlass.hpp"
#include "PyCell.hpp"
#include "PyList.hpp"
#include "PyDict.hpp"
#include "ListKlass.hpp"
#include "StringTable.hpp"
#include "DictKlass.hpp"
#include <algorithm>
#include "TypeKlass.hpp"
#include "ObjectKlass.hpp"
#include "PyTypeObject.hpp"
#include "FloatKlass.hpp"
#include "PyFloat.hpp"
#include "SuperKlass.hpp"
#include "RangeKlass.hpp"

Interpreter* Interpreter::instance = nullptr;

Interpreter::Interpreter() {
    _curFrame = nullptr;
    _retValue = nullptr;

    _status = IS_OK;

    _builtins = PyDict::createDict();

    _builtins->set(StringTable::str_true, Universe::PyTrue);
    _builtins->set(StringTable::str_false, Universe::PyFalse);
    _builtins->set(StringTable::str_none, Universe::PyNone);
    
    _builtins->set(StringTable::str_len, PackNativeFunc(NativeFunction::len));
    _builtins->set(StringTable::str_isinstance, PackNativeFunc(NativeFunction::isinstance));
    _builtins->set(StringTable::str_typeof, PackNativeFunc(NativeFunction::type_of));
    _builtins->set(PyString::createString("id"), PackNativeFunc(NativeFunction::id));

    _builtins->set(StringTable::str_object, ObjectKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_type, TypeKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_int, IntegerKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_str, StringKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_list, ListKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_dict, DictKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_float, FloatKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_super, SuperKlass::getInstance()->getTypeObject());
    _builtins->set(StringTable::str_range, RangeKlass::getInstance()->getTypeObject());

    _builtins->set(PyString::createString("sysgc"), PackNativeFunc(NativeFunction::sysgc));
}

void Interpreter::run(CodeObject* codeObject) {
    _curFrame = new FrameObject(codeObject);
    _curFrame->_locals->set(PyString::createString("__name__"), PyString::createString("__main__"));
    evalFrame();
    destroyCurFrame();
}

#define POP() (_curFrame->popFromStack())
#define TOP() (_curFrame->getTopInStack())
#define PUSH(x) (_curFrame->pushToStack(x))
#define POP_BLOCK() (_curFrame->_blockStack->pop())
#define PUSH_BLOCK(type, target, level) \
    (_curFrame->_blockStack->push({type, target, level}))
#define PC (_curFrame->pc)
#define Consts (_curFrame->_consts)
#define StackLength (_curFrame->_stack->getLength())

void Interpreter::evalFrame() {
    
    assert(_curFrame != nullptr);

    uint8_t op_code;
    bool hasArugment;
    uint16_t op_arg;

    Handle<PyObject*> rhs = nullptr;
    Handle<PyObject*> lhs = nullptr;

    while (_curFrame->hasMoreCode()) {

        if (_status != IS_OK) {
            goto fast_handle_exception;
        }

        op_code = _curFrame->getOpCode();
        hasArugment = op_code >= ByteCode::Have_Argument;
        op_arg = -1;

        // ���ָ��Σ���һ��ȡ����ȡ��ʱ��ע����С����
        if (hasArugment) {
            op_arg = _curFrame->getOpArgument();
        }

        switch (op_code) {
            case ByteCode::Pop_Top:
                POP();
                break;

            case ByteCode::Dup_Top:
                PUSH(TOP());
                break;

            case ByteCode::Build_List: {
                Handle<PyList*> list = PyList::createList();
                while (op_arg-- > 0) {
                    Handle<PyObject*> temp = POP();
                    list->set(op_arg, temp);
                }
                PUSH(list->as<PyObject>());
                break;
            }
            
            // ��ʱ����list����һ��tuple
            case ByteCode::Build_Tuple: {
                Handle<PyList*> list = PyList::createList();
                while (op_arg-- > 0) {
                    Handle<PyObject*> temp = POP();
                    list->set(op_arg, temp);
                }
                PUSH(list->as<PyObject>());
                break;
            }

            case ByteCode::Build_Map: {
                PUSH(PyDict::createDict());
                break;
            }

            case ByteCode::Store_Map: {
                Handle<PyObject*> key = POP();
                Handle<PyObject*> value = POP();
                // ��Ϊ�����ж�����Ԫ��Ҫ�����ֵ䣬
                // �����ֵ������ʱ���ܴ�ջ�ϵ���
                Handle<PyDict*> dict = static_cast<PyDict*>(TOP());
                dict->set(key, value);
                break;
            }

            case ByteCode::Load_Const: {
                Handle<PyObject*> temp = (_curFrame->_consts)->get(op_arg);
                PUSH(temp);
                break;
            }

            case ByteCode::Print_Item:
                lhs = POP();
                if (isPyInteger(lhs())) {
                    printf("%lld", toRawInteger(lhs()));
                }
                else {
                    lhs->print(FLAG_PyString_PRINT_RAW);
                }
                break;

            case ByteCode::Print_NewLine:
                putchar('\n');
                break;

            case ByteCode::Binary_Multiply:
                rhs = POP();  // �Ҳ�����
                lhs = POP();
                if (isPyInteger(lhs())) {
                    if (isPyInteger(rhs())) {
                        PUSH(toPyInteger(toRawInteger(lhs()) * toRawInteger(rhs())));
                    }
                    else if (rhs->getKlass() == FloatKlass::getInstance()) {
                        PUSH(rhs->mul(lhs));
                    }
                    else {
                        printf("can't multiply\n");
                        exit(-1);
                    }
                }
                else {
                    PUSH(lhs->mul(rhs));
                }
                break;

            case ByteCode::Binary_Add: {
                rhs = POP();  // �Ҳ�����
                lhs = POP();
                if (isPyInteger(lhs())) {
                    if (isPyInteger(rhs())) {
                        PUSH(toPyInteger(toRawInteger(lhs()) + toRawInteger(rhs())));
                    }
                    else if (rhs->getKlass() == FloatKlass::getInstance()) {
                        PUSH(rhs->add(lhs));
                    }
                    else {
                        printf("can't add\n");
                        exit(-1);
                    }
                }
                else {
                    PUSH(lhs->add(rhs));
                }
                break;
            }

            case ByteCode::Binary_Divide:
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������

                if (isPyInteger(lhs())) {
                    if (isPyInteger(rhs())) {
                        if (toRawInteger(rhs()) == 0) {
                            printf("division by zero");
                            exit(-1);
                        }
                        PUSH(new PyFloat(toRawInteger(lhs()) / toRawInteger(rhs())));
                    }
                    else if (rhs->getKlass() == FloatKlass::getInstance()) {
                        if (rhs->as<PyFloat>()->getValue() == 0.0) {
                            printf("division by zero");
                            exit(-1);
                        }
                        PUSH(new PyFloat(toRawInteger(lhs()) / rhs->as<PyFloat>()->getValue()));
                    }
                    else {
                        printf("can't divide\n");
                        exit(-1);
                    }
                }
                else {
                    PUSH(lhs->div(rhs));
                }
                break;

            case ByteCode::Binary_FloorDivide:
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������

                if (isPyInteger(lhs())) {
                    if (isPyInteger(rhs())) {
                        if (toRawInteger(rhs()) == 0) {
                            printf("division by zero");
                            exit(-1);
                        }
                        PUSH(toPyInteger(toRawInteger(lhs()) / toRawInteger(rhs())));
                    }
                    else if (rhs->getKlass() == FloatKlass::getInstance()) {
                        if (rhs->as<PyFloat>()->getValue() == 0.0) {
                            printf("division by zero");
                            exit(-1);
                        }
                        PUSH(new PyFloat(floor(toRawInteger(lhs()) / rhs->as<PyFloat>()->getValue())));
                    }
                    else {
                        printf("can't divide\n");
                        exit(-1);
                    }
                }
                else {
                    PUSH(lhs->floor_div(rhs));
                }
                break;

            case ByteCode::Binary_Module:
                rhs = POP();
                lhs = POP();
                if (isPyInteger(lhs()) && isPyInteger(rhs())) {
                    PUSH(toPyInteger(toRawInteger(lhs()) % toRawInteger(rhs())));
                }
                else {
                    PUSH(lhs->mod(rhs));
                }
                break;

            case ByteCode::Inplace_Add:
                rhs = POP();
                lhs = POP();
                if (isPyInteger(lhs()) && isPyInteger(rhs())) {
                    PUSH(toPyInteger(toRawInteger(lhs()) + toRawInteger(rhs())));
                }
                else {
                    PUSH(lhs->inplace_add(rhs));
                }
                break;

            case ByteCode::Binary_Subtract:
                rhs = POP();  // �Ҳ�����
                lhs = POP();
                if (isPyInteger(lhs())) {
                    if (isPyInteger(rhs())) {
                        PUSH(toPyInteger(toRawInteger(lhs()) - toRawInteger(rhs())));
                    }
                    else if (rhs->getKlass() == FloatKlass::getInstance()) {
                        PUSH(new PyFloat(toRawInteger(lhs()) - rhs->as<PyFloat>()->getValue()));
                    }
                    else {
                        printf("can't substract\n");
                        exit(-1);
                    }
                }
                else {
                    PUSH(lhs->sub(rhs));
                }
                break;

            case ByteCode::Binary_Subscr:
                rhs = POP();  // �Ҳ��������ڴ˴�ΪҪȡ���±꣩
                lhs = POP();  // ����������ڴ˴�Ϊ��ȡ�±��Ԫ�أ�
                PUSH(lhs->subscr(rhs));
                break;

            case ByteCode::Store_Subscr: {
                rhs = POP();  // �Ҳ��������ڴ˴�ΪҪȡ���±꣩
                lhs = POP();  // ����������ڴ˴�Ϊ��ȡ�±��Ԫ�أ�
                Handle<PyObject*> newObject = POP();
                lhs->store_subscr(rhs, newObject);
                break;
            }

            case ByteCode::Delete_Subscr: {
                rhs = POP();  // �Ҳ��������ڴ˴�ΪҪȡ���±꣩
                lhs = POP();  // ����������ڴ˴�Ϊ��ȡ�±��Ԫ�أ�
                lhs->delete_subscr(rhs);
                break;
            }

            case ByteCode::Compare_Op: {
                rhs = POP();  // �Ҳ�����
                lhs = POP();  // �������
                compareTwoPythonObjects(lhs, rhs, op_arg);
                break;
            }

            // Pop_Jump_If_Falseָ�����Compare_Opָ��֮��ʹ��
            // ��ָ��ִ�е��Ǿ��Ե�ַ��ת
            case ByteCode::Pop_Jump_If_False:
                lhs = POP();
                if (!isPyTrue(lhs())) { 
                    PC = op_arg;
                }
                break;

            // �������ľ��Ե�ַ��ת
            case ByteCode::Jump_Absolute:
                PC = op_arg;
                break;

            // ����������Ե�ַ��ǰ��ת
            case ByteCode::Jump_Forward:
                PC += op_arg;
                break;

            // ��ʼ��Blockʵ��
            case ByteCode::Setup_Loop:
                PUSH_BLOCK(op_code, PC + op_arg, StackLength);
                break;
            
            // �����˳�����������ջ�������Ԫ��
            // ʹ��ջ�ĳ��Ȼָ��������������֮ǰ
            case ByteCode::Pop_Block: {
                Block block = POP_BLOCK();
                while (StackLength > block._level) POP();
                break;
            }

            // ѭ��ִ����;ǿ���˳�ѭ��
            case ByteCode::Break_Loop: {
                Block block = POP_BLOCK();
                while (StackLength > block._level) POP();
                PC = block._target;
                break;
            }

            // ����ջ����CodeObject���󣬾ݴ˴�������
            // ��ָ��в�������ʾ��ҪΪ�ú����󶨶��ٸ�Ĭ�ϲ���
            case ByteCode::Make_Function: {
                makeFunction(op_arg, false);
                break;
            }
            
            case ByteCode::Make_Closure: {
                makeFunction(op_arg, true);
                break;
            }

            case ByteCode::Call_Function: {
                uint8_t argNumber_pos = op_arg & 0xff;
                uint8_t argNumber_kw = op_arg >> 8;
                // ��Ϊ���ú������ݼ�ֵ��չ����ʱ��key��value����ѹջ����������Ҫ*2
                uint8_t argNumber_total = argNumber_pos + 2 * argNumber_kw;
                // �Ȱ����в�����ջ�ϵ��������浽��ʱ�б���ȥ
                Handle<PyList*> rawArgs = PyList::createList(argNumber_total);
                while (argNumber_total-- > 0) {
                    rawArgs->set(argNumber_total, POP());
                }
                // ���������ã��Ժ��������Ľ�һ��������֮����
                Handle<PyObject*> callableObject = POP();
                entryIntoNewFrame(callableObject, rawArgs, 
                    argNumber_pos, argNumber_kw);
                break;
            }
            
            case ByteCode::Return_Value:
                // ��Ϊcallee��ջ�弴�������٣�������Ҫ������ֵ��ʱ����һ��
                _retValue = POP();
                _status = IS_RETURN;
                // �˳�������calleeջ�壬�ָ�callerջ��
                //exitFromCurFrame();
                break;

            // ��ջ��Ԫ�ش��浽ȫ�ֱ���
            case ByteCode::Store_Global: {
                // ջ�������Ҫд���PyObject����������Ҫ�Ȱ���������
                Handle<PyObject*> sourceObject = POP();
                // �ȸ���ָ��������Ҫ���ʱ���������
                Handle<PyObject*> variableName = _curFrame->_names->get(op_arg);
                // �ٸ�������������ȫ�ֱ����Ķ���
                _curFrame->_globals->set(variableName, sourceObject);
                break;
            }
            
            // ��ָ����ȫ�ֱ���ѹ��ջ��
            case ByteCode::Load_Global: {
                // �ȸ���ָ������ҳ�Ҫ���ʱ����ı�����
                Handle<PyObject*> variableName = _curFrame->_names->get(op_arg);
                // �ٸ��ݱ�����������Python����
                Handle<PyObject*> variableObject = _curFrame->_globals->get(variableName);
                if (variableObject != nullptr) {
                    PUSH(variableObject);
                    break;
                }
                // �������û�ҵ���ȥ�����ڽ���������������õ�None��
                variableObject = this->_builtins->get(variableName);
                if (variableObject != nullptr) {
                    PUSH(variableObject);
                }
                else {
                    printf("name '%s' is not defined\n", 
                           variableName->as<PyString>()->getValue());
                    exit(-1);
                }
                break;
            }

            // ��ջ��Ԫ�ش��浽ָ�����Ƶı��ر���
            case ByteCode::Store_Name: {
                Handle<PyObject*> sourceObject = POP();
                Handle<PyObject*> variableName = _curFrame->_names->get(op_arg);
                // ����Python���ں������ڳ��ԶԱ�����ֵʱ��
                // ����ȫ����ȫ�ֱ���
                // ��������ֱ������_locals���ɣ�����ȥ���_globals��
                _curFrame->_locals->set(variableName, sourceObject);
                break;
            }
            
            // ����ָ�����Ƶı�����ջ��
            case ByteCode::Load_Name: {
                Handle<PyObject*> variableName = 
                    _curFrame->_names->get(op_arg);
                // ��ȥ���ر���������û����Ҫ�ҵı���
                Handle<PyObject*> variableObject = 
                    _curFrame->_locals->get(variableName);
                if (variableObject != nullptr) {
                    PUSH(variableObject);
                    break;
                }
                // û�ҵ��Ļ���ȥȫ�ֱ�����������
                variableObject = _curFrame->_globals->get(variableName);
                if (variableObject != nullptr) {
                    PUSH(variableObject);
                    break;
                }
                // �������û�ҵ���ȥ�����ڽ�������
                variableObject = this->_builtins->get(variableName);
                if (variableObject != nullptr) {
                    PUSH(variableObject);
                } else {
                    printf("name '%s' is not defined\n",
                        variableName->as<PyString>()->getValue());
                    exit(-1);
                }
                break;
            }
            
            case ByteCode::Load_Attr: {
                // ��Ҫ�������ԵĶ���ջ
                lhs = POP();  
                // ����Ҫ��ȡ����������name
                rhs = _curFrame->_names->get(op_arg);
                // ����õ����Զ���ѹջ
                Handle<PyObject*> attr = lhs->getattr(rhs);
                PUSH(attr);
                break;
            }
            
            /*
                a.x = 123
                # LOAD_CONST  2 (123)
                # LOAD_NAME   1 (a)
                # STORE_ATTR  2 (x)
            */
            case ByteCode::Store_Attr: {
                Handle<PyObject*> attr = _curFrame->_names->get(op_arg);
                lhs = POP();
                rhs = POP();
                lhs->setattr(attr, rhs);
                break;
            }

            case ByteCode::Load_Fast: {
                Handle<PyObject*> elem = _curFrame->_fastLocals->get(op_arg);
                if (elem != nullptr) {
                    PUSH(elem);
                } else {
                    Handle<PyString*> varname = 
                        static_cast<PyString*>(_curFrame->_varNames->get(op_arg));
                    printf("local variable '%s' referenced before assignment\n",
                        varname->getValue());
                    exit(-1);
                }
                break;
            }

            case ByteCode::Store_Fast:
                _curFrame->_fastLocals->set(op_arg, POP());
                break;

            case ByteCode::Load_Locals: {
                PUSH(_curFrame->_locals);
                break;
            }

            // ����ջ��Ԫ�أ�������ص�����ʱ��cells����
            /*
                ��Python�ıհ������У�ֻ��outer function���п��ܵ���Store_Deref
                �����inner function�г��Զ�ͬ��������ֵ���������Ϊinner function
                �����µ�local variable�������޸�outer function�е�ͬ������
            */
            case ByteCode::Store_Deref:
                _curFrame->_cells->set(op_arg, POP());
                break;
            
            // ��ȡcell variable�������ã��ټ��ص�ջ��
            case ByteCode::Load_Deref: {
                Handle<PyObject*> object = _curFrame->_cells->get(op_arg);
                if (!object) {
                    Handle<PyObject*> cellName =
                        _curFrame->_codeObject->_cellVars->get(op_arg);
                    size_t i = _curFrame->_varNames->index(cellName);
                    object = _curFrame->_fastLocals->get(i);
                    // �ҵ���֮������˰�cellObject���ص�ջ���_cells��ȥ
                    _curFrame->_cells->set(op_arg, object);
                }
                if (
                    !isPyInteger(object()) 
                    && object->getKlass() == CellKlass::getInstance()
                ) {
                    Handle<PyCell*> cell = object->as<PyCell>();
                    Handle<PyObject*> temp = cell->getObject();
                    PUSH(temp);
                }
                else {
                    PUSH(object);
                }
                break;
            }


            // ��cells���е�Ԫ��ת��ΪPyCell��ѹջ
            // ��ָ����Make_Closureָ�����ʹ�ã����ڹ����հ�����
            case ByteCode::Load_Closure: {
                Handle<PyObject*> cellObject = _curFrame->_cells->get(op_arg);
                /* 
                    �����cells�б���û�ҵ���Ҳ�п�����inner functionǡ������
                    outer function��argument�����Ի�Ҫ��ȥ�������ҡ�
                */
                if (!cellObject) {
                    Handle<PyObject*> cellName = 
                        _curFrame->_codeObject->_cellVars->get(op_arg);
                    size_t i = _curFrame->_varNames->index(cellName);
                    cellObject = _curFrame->_fastLocals->get(i);
                    // �ҵ���֮������˰�cellObject���ص�ջ���_cells��ȥ
                    _curFrame->_cells->set(op_arg, cellObject);
                }
                if (
                    isPyInteger(cellObject()) 
                    || cellObject->getKlass() != CellKlass::getInstance()
                ) {
                    cellObject = new PyCell(_curFrame->_cells, op_arg);
                }
                PUSH(cellObject);
                break;
            }
            
            // ��ջ�ϵ���Ҫ��ȡ��������Ԫ�أ��ٰѻ�ȡ���ĵ�����ѹ��ջ��
            case ByteCode::Get_Iter: {
                lhs = POP();
                PUSH(lhs->getIter());
                break;
            }
            
            case ByteCode::For_Iter: {
                // ��ȡ�������������ڵ�����Ҫ�ظ�ʹ�ã����Բ��ܴ�ջ�ϵ�����
                Handle<PyObject*> iter = TOP();
                // ���õ����������next����
                Handle<PyObject*> ret = iter->next();
                // �������ֵ��None����˵������������ִ����ת
                // ע�����������TOP������POP��
                // ��Ϊ��������ǰ����ջ�ϵķ���ֵ���ܻᱻ����ָ��ʹ��
                if (ret == nullptr) {
                    PC += op_arg;
                }
                else {
                    PUSH(ret);
                }
                break;
            }

            case ByteCode::Unpack_Sequence: {
                Handle<PyList*> list = static_cast<PyList*>(POP());
                // Python�н��ֵ���ձ���������ʹ�����б�ĳ���һ��
                size_t length = list->getLength();
                if (op_arg < length) {
                    printf("too many values to unpack (expected %lld)", length);
                    exit(-1);
                }
                else if (op_arg > length) {
                    printf("not enough values to unpack (expected %d, got %lld)", 
                        op_arg, length);
                    exit(-1);
                }
                // �Ӻ���ǰ�����ȷ���б���Ԫ�ؿ���λ��ջ����������ָ����ȡ��
                while (op_arg-- > 0) {
                    PUSH(list->get(op_arg));
                }
                break;
            }

            case ByteCode::Build_Class: {
                Handle<PyObject*> dict = POP();
                Handle<PyObject*> supers = POP();
                Handle<PyObject*> name = POP();
                PUSH(Klass::createKlass(dict, supers, name));
                break;
            }
                
            default:
                printf("Unknown bytecode 0x%x\n", op_code);
                exit(-1);
        }
    fast_handle_exception:
        if (_status != IS_OK && _curFrame->_blockStack->getLength() <= 0) {
            if (_status == IS_RETURN) {
                _status = IS_OK;
            }
            if (_curFrame->isRootFrame() || _curFrame->isEntryFrame()) {
                return;
            }
            exitFromCurFrame();
        }            
    }
}

void Interpreter::compareTwoPythonObjects(
    Handle<PyObject*> lhs, Handle<PyObject*> rhs, uint16_t op
) {

    if (isPyInteger(lhs()) && isPyInteger(rhs())) {
        switch (op) {
            case CompareCondition::Less:
                PUSH(packBoolean((int64_t)lhs() < (int64_t)rhs()));
                break;
            case CompareCondition::Less_Equal:
                PUSH(packBoolean((int64_t)lhs() <= (int64_t)rhs()));
                break;
            case CompareCondition::Equal:
                PUSH(packBoolean((int64_t)lhs() == (int64_t)rhs()));
                break;
            case CompareCondition::Greater_Equal:
                PUSH(packBoolean((int64_t)lhs() >= (int64_t)rhs()));
                break;
            case CompareCondition::Greater:
                PUSH(packBoolean((int64_t)lhs() > (int64_t)rhs()));
                break;
            case CompareCondition::Not_Equal:
                PUSH(packBoolean((int64_t)lhs() != (int64_t)rhs()));
                break;
            default:
                printf("Illegal compare operator between integers.\n");
                exit(-1);
        }
    } 
    else {
        switch (op) {
            case CompareCondition::Less:
                if (isPyInteger(lhs())) lhs = new PyInteger(toRawInteger(lhs()));
                PUSH(lhs->less(rhs));
                break;
            case CompareCondition::Less_Equal:
                if (isPyInteger(lhs())) lhs = new PyInteger(toRawInteger(lhs()));
                PUSH(lhs->less_equal(rhs));
                break;
            case CompareCondition::Equal:
                if (isPyInteger(lhs())) lhs = new PyInteger(toRawInteger(lhs()));
                PUSH(lhs->equal(rhs));
                break;
            case CompareCondition::Greater_Equal:
                if (isPyInteger(lhs())) lhs = new PyInteger(toRawInteger(lhs()));
                PUSH(lhs->greater_equal(rhs));
                break;
            case CompareCondition::Greater:
                if (isPyInteger(lhs())) lhs = new PyInteger(toRawInteger(lhs()));
                PUSH(lhs->greater(rhs));
                break;
            case CompareCondition::Not_Equal:
                if (isPyInteger(lhs())) lhs = new PyInteger(toRawInteger(lhs()));
                PUSH(lhs->not_equal(rhs));
                break;
            // is��is not�ؼ������ڱȽ�����python����ĵ�ַ�Ƿ�һ��
            case CompareCondition::Is:
                PUSH(packBoolean(lhs() == rhs()));
                break;
            case CompareCondition::Is_Not:
                PUSH(packBoolean(lhs() != rhs()));
                break;
            case CompareCondition::In:
                PUSH(rhs->has(lhs));
                break;
            case CompareCondition::Not_In:
                PUSH(packBoolean(rhs->has(lhs) == Universe::PyFalse));
                break;
            default:
                printf("Unknown Compare Condition Code 0x%x\n", op);
                exit(-1);
        }
    }
}

void Interpreter::makeFunction(int16_t defaultArgCount, bool isClosure) {
    Handle<PyFunction*> funcObject = new PyFunction(static_cast<CodeObject*>(POP()));

    // �����Ҫ�����ĺ�������Ϊ�հ���������Ҫ��cells�б�
    if (isClosure) {
        Handle<PyObject*> temp = POP();
        funcObject->setFreevars(temp->as<PyList>());
    }

    // ������Ҫ��Ĭ�ϲ����ĺ���
    if (defaultArgCount > 0) {
        Handle<PyList*> DefaultArgs = PyList::createList(defaultArgCount);
        while (defaultArgCount-- > 0) {
            DefaultArgs->set(defaultArgCount, POP());
        }
        funcObject->setDefaultArgs(DefaultArgs);
    }

    // ͬ��ջ��ȫ�ֱ�����ͺ��������ȫ�ֱ�����
    funcObject->setGlobalMap(_curFrame->_globals);
    PUSH(funcObject->as<PyObject>());
}

/*
callableObject ���� ��ջ��ȡ�����ı����ö��󣬿�����Python������Python����
    ����C++�ڽ�����

rawArgs ���� ��ջ��ȡ�������û����ú��������б�����˳��Ϊ��ͨλ�ò�����λ����չ������
    ��ֵ��չ����

rawArgNumber_pos ���� ���û�����Call_Functionʱ��op_args�ĵ�8λ��
    ������ͨλ�ò�����λ����չ�������ܸ���

rawArgNumber_kw ���� ���û�����Call_Functionʱ��op_args�ĸ�8λ��
    ������չ��ֵ�������ܸ���
*/
void Interpreter::entryIntoNewFrame(
    Handle<PyObject*> callableObject, Handle<PyList*> rawArgs, 
    uint8_t rawArgNumber_pos, uint8_t rawArgNumber_kw
) {
    /*
        ��Python2�У�������ͨ��λ�ò�����λ����չ������*args����
        ��ִ��Call_Functionָ��ʱ���ᱻ�ǵ�op_args�ﴫ��argNumber��

        �ں���CodeObject��argcount�У�ֻ�������ͨλ�ò�����������

        op_args�ĸ�8λ������չ��ֵ�����ĸ�����
        ��8λ������ͨλ�ò�����λ����չ�������ܸ�����
    */
    
    // ȡ��callableObject��klass���жϺ��������Ƿ�Ϸ�
    Handle<Klass*> klass = callableObject->getKlass();
    if (!isMethod(klass) && !isTypeObject(klass) && !isCommonFuncKlass(klass)) {
        printf("Unknown callable object!");
        exit(-1);
    }

    // �ж�callable object����ʵ���ͣ���ȡ��������callee function
    Handle<PyFunction*> calleeFunc = nullptr;
    Handle<PyObject*> owner = nullptr;
    if (isMethod(klass)) {
        //Handle<PyMethod*> method = static_cast<PyMethod*>((PyObject*)callableObject);
        Handle<PyMethod*> method = callableObject->as<PyMethod>();
        calleeFunc = method->getFunc();
        owner = method->getOwner();
    }
    else if (isCommonFuncKlass(klass)) {
        //calleeFunc = static_cast<PyFunction*>((PyObject*)callableObject);
        calleeFunc = callableObject->as<PyFunction>();
    }
    else if (isTypeObject(klass)) {
        //Handle<PyObject*> inst = static_cast<PyTypeObject*>((PyObject*)callableObject)
        Handle<PyObject*> inst = callableObject
            ->as<PyTypeObject>()
            ->getOwnKlass()
            ->allocateInstance(callableObject, rawArgs);
        PUSH(inst);
        return;
    }

    if (calleeFunc == nullptr) {
        fputs("Try to call a function with null pointer", stderr);
        exit(-1);
    }

    // ���ڷ�������Ҫ������б��������������
    if (isMethod(klass)) {
        rawArgs->insert(0, owner);
        ++rawArgNumber_pos;
    }

    klass = calleeFunc->getKlass();
    Handle<PyList*> finalArgs = PyList::createList(rawArgNumber_pos);

    // ����һ���python����������Ҫ����һϵ�еĲ�������
    Handle<PyList*> posExArgs = nullptr;
    Handle<PyDict*> keywordExArgs = nullptr;
    Handle<CodeObject*> code = nullptr;
    if (isPythonFuncKlass(klass)) {

        code = calleeFunc->funcCode;
        size_t formalArgNumber_total = code->_argCount;
        size_t formalArgNumber_default = 0;

        // ��ʼ��λ����չ�������ͼ�ֵ��չ����
        int flags = code->_flag;
        if (flags & PyFunction::CO_VARARGS) {
            posExArgs = PyList::createList(rawArgNumber_pos - formalArgNumber_total);
        }
        if (flags & PyFunction::CO_VARKEYWORDS) {
            keywordExArgs = PyDict::createDict();
        }

        // �������β��б��е�Ĭ�ϲ���ֵ
        // ��������İ취�����Ȱ����е�Ĭ�ϲ���ֵд��finalArgs�ϣ�����ʵ�θ���
        Handle<PyList*> defaultArgs = calleeFunc->_defaultArgs;
        if (defaultArgs) {
            formalArgNumber_default = defaultArgs->getLength();
            /* 
                Python�д�Ĭ��ֵ����ֻ�ܷ�����Ĭ��ֵ������ߡ�
                
                �����û�����Make_Functionʱ�����еĲ���Ĭ��ֵ�Ѱ��������ҵ�
                ��ȷ˳������ں�����defaultArgs�б���

                ��������󵨴Ӻ���ǰд����
            */
            size_t cnt_total = formalArgNumber_total;
            size_t cnt_def = formalArgNumber_default;
            while (cnt_def-- > 0) {
                finalArgs->set(--cnt_total, defaultArgs->get(cnt_def));
            }
        }

        /*  ���û����õ�ʵ��д��finalArgs��
            
            ����û������rawArgNumber_posС�ڵ���formalArgNumber_total��
            ˵���϶�û�ж���Ĳ�����Ϊλ����չ������*args����
            ��ʱֱ�Ӱ��û�����Ĳ���д��finalArgs�м���

            ��֮��Ҫ����λ����չ����
        */
        auto funcName = code->_name->getValue();
        if (rawArgNumber_pos <= formalArgNumber_total) {
            for (size_t i = 0; i < rawArgNumber_pos; ++i) {
                finalArgs->set(i, rawArgs->get(i));
            }
        }
        else {
            size_t i = 0;
            if (posExArgs == nullptr) {
                printf("%.200s() takes %s %lld arguments (%d given)\n", 
                    funcName, 
                    formalArgNumber_default > 0 ? "at most" : "exactly", 
                    formalArgNumber_total, rawArgNumber_pos);
                exit(-1);
            }
            for (; i < formalArgNumber_total; ++i) {
                finalArgs->set(i, rawArgs->get(i));
            }
            for (; i < rawArgNumber_pos; ++i) {
                posExArgs->append(rawArgs->get(i));
            }
        }

        // ������������չ��ֵ������**kwargs��
        for (size_t i = 0; i < rawArgNumber_kw; ++i) {
            Handle<PyObject*> key = rawArgs->get(rawArgNumber_pos + 2 * i);
            Handle<PyObject*> value = rawArgs->get(rawArgNumber_pos + 2 * i + 1);
            
            if (key->getKlass() != StringKlass::getInstance()) {
                printf("%.200s() keywords must be strings\n", funcName);
                exit(-1);
            }
            
            // ���ڱ���������varnames����һ�£������ܲ����ҵ�keyword
            // ������ҵ���˵���û����ڳ���Ϊ�������βθ�ֵ
            // �����ҵ����ü�ֵ�Լ�����չ��ֵ����
            size_t foundIndex = 0;
            bool found = false;
            for (; foundIndex < formalArgNumber_total; ++foundIndex) {
                if (
                    isTrue(key->equal(code->_varNames->get(foundIndex)))
                ) {
                    found = true;
                    break;
                }
            }
            if (found) {
                if (
                    foundIndex < finalArgs->getLength() && 
                    finalArgs->get(foundIndex) != nullptr
                ) {
                    printf("%.200s() got multiple values for keyword "
                           "argument '%.400s'\n", 
                           funcName,
                           key->as<PyString>()->getValue());
                    exit(-1);
                }
                finalArgs->set(foundIndex, value);
            }
            else if (keywordExArgs != nullptr) {
                keywordExArgs->set(key, value);
            }
            else {
                printf("%.200s() got an unexpected keyword "
                       "argument '%.400s'\n", 
                       funcName, 
                       key()->as<PyString>()->getValue());
                exit(-1);
            }
        }

        // ������չλ�ò���ȱʧ���
        if (rawArgNumber_pos < formalArgNumber_total) {
            size_t border = formalArgNumber_total - formalArgNumber_default;
            size_t lenFinalArgs = finalArgs->getLength();
            for (size_t i = rawArgNumber_pos; i < border; ++i) {
                if (finalArgs->get(i) == nullptr) {
                    size_t given = 0;
                    for (size_t j = 0; j < std::min(formalArgNumber_total, lenFinalArgs); ++j) {
                        if (finalArgs->get(j) != nullptr) ++given;
                    }
                    printf("%.200s() takes %s %lld arguments (%lld given)", 
                        funcName,
                        formalArgNumber_default > 0 || posExArgs != nullptr ? "at least" : "exactly",
                        border, given);
                    exit(-1);
                }
            }
        }

        // �������˰���չ����װ�ص�finalArgs��ȥ
        if (posExArgs != nullptr) {
            finalArgs->append(posExArgs->as<PyObject>());
        }
        if (keywordExArgs != nullptr) {
            finalArgs->append(keywordExArgs->as<PyObject>());
        }
    }
    // ����C++�ڽ�������ֱ�Ӵ��μ���
    else {
        finalArgs = rawArgs;
    }

    // ǰ��׼��������ϣ����������ĺ�������
    if (isNativeFuncKlass(klass)) {
        _curFrame->pushToStack(calleeFunc->callNativeFunc(finalArgs));
    }
    else if (isPythonFuncKlass(klass)) {
        // ������ջ��
        Handle<FrameObject*> calleeFrame = 
            FrameObject::allocate(calleeFunc, _curFrame, false, finalArgs);

        /* 
           ����callee�󶨵�cells����callee��������ʱ
           ��Ҫ������freevars����װ�ص��µ�ջ����ȥ��
           ������callee����ʱ���Ϳ���ͨ��Load_Closureָ��
           ������Щfreevars�ˡ�
        */
        Handle<PyList*> freevarsList = calleeFunc->_freevars;
        if (freevarsList) {
            size_t freevar_len = freevarsList->getLength();
            size_t cells_len = calleeFunc->funcCode->_cellVars->getLength();
            for (size_t i = 0; i < freevar_len; ++i) {
                Handle<PyCell*> temp = freevarsList->get(i)->as<PyCell>();
                calleeFrame->_cells->set(cells_len + i, temp);
            }
        }
        // ���������һ�����л���������ִ��������
        _curFrame = calleeFrame;
    }
    else {
        printf("Unknown function object!");
        exit(-1);
    }
}

// �˳������ٵ�ǰջ�壬�ٰѽ�����ִ���������л�Ϊcaller��ջ��
void Interpreter::destroyCurFrame() {
    FrameObject* callerFrame = _curFrame->_callerFrame;
    // ����callee��ջ��
    delete _curFrame;
    // �ָ�caller��ջ��
    _curFrame = callerFrame;
}

// ���ڲ���Rootջ��Ҳ������C++��������������ջ�壬
// ��Ҫ��callee�ķ���ֵѹ��caller������ʱջ
void Interpreter::exitFromCurFrame() {
    destroyCurFrame();
    PUSH(_retValue);
}

PyObject* Interpreter::callVirtual(Handle<PyObject*> callable, Handle<PyList*> args) {
    Handle<Klass*> klass = callable->getKlass();
    if (klass == NativeFunctionKlass::getInstance()) {
        return callable()->as<PyFunction>()->callNativeFunc(args);
    }
    else if (klass == MethodKlass::getInstance()) {
        Handle<PyMethod*> method = callable()->as<PyMethod>();
        Handle<PyFunction*> func = method->getFunc();
        if (args == nullptr) {
            args = PyList::createList(1);
        }
        args->insert(0, method->getOwner());
        return callVirtual(func(), args);
    }
    else if (klass == FunctionKlass::getInstance()) {
        // ����Pythonջ��
        Handle<FrameObject*> frame = FrameObject::allocate(
            callable()->as<PyFunction>(),
            /* 
                ��Python������ӽ������������Ǳ�C++������õ�ĳ��Python������
                ���߼���Ҳ�Ǳ�ĳһ��Python���봥���ġ�
                �����Ȼ��Ҫָ��callerFrame���Ա������ִ����Ŀ�꺯����
                �ܹ��˻ص�Python�����е���ȷλ�á�
            */
            _curFrame,
            /*
                ��C++���õ�Python�����ķ���ֵ������ֱ�ӱ�Python�����õ���
                ��˽�isEntryFrame��Ϊtrue����ʾ��������Ҫ�Զ�
                �����ؽ��ѹ��caller��ջ���С�
            */
            true,
            args
        );
        // �ֹ�ִ����ջ��
        _curFrame = frame;
        evalFrame();
        // �ֹ����ص���Python�������õķ���ֵ��C++����
        destroyCurFrame();
        return _retValue;
    }
    else {
        printf("Unknown function object!");
        exit(-1);
    }
}

FrameObject* Interpreter::getCurrentFrame() {
    return _curFrame;
}

/* GC��ؽӿ� ��ʼ */
void Interpreter::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&_builtins));
    closure->do_oop(&_retValue);

    if (_curFrame) _curFrame->oops_do(closure);
}
/* GC��ؽӿ� ���� */