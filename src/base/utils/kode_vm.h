#ifndef kode_gcc_vm_included
#define kode_gcc_vm_included
//----------------------------------------------------------------------

/*
  uses calculated gotos ('goto **ip++')..


  bk  : byte kode
  kbc : kode bytce code

  todo:
  - call external functions
*/

//----------

#include <stdio.h>  // printf
#include "base/kode_stack.h"

//----------

#define KODE_VM_CALL_STACK_SIZE   1024
#define KODE_VM_DATA_STACK_SIZE   1024
#define KODE_VM_CODE_SEGMENT_SIZE 65536
#define KODE_VM_DATA_SEGMENT_SIZE 65536
#define KODE_VM_MAX_EXTERNALS     256
#define KODE_VM_NUM_REGISTERS     256

//----------------------------------------------------------------------

typedef intptr_t KODE_Cell;

typedef KODE_Stack<KODE_Cell, KODE_VM_DATA_STACK_SIZE> KODE_DataStack;
typedef KODE_Stack<KODE_Cell*,KODE_VM_CALL_STACK_SIZE> KODE_CallStack;

struct KODE_Opcode {
  int       type;
  KODE_Cell value;
};

// opcode types
#define KODE_VM_OP_TYPE_NONE      0
#define KODE_VM_OP_TYPE_INTERNAL  1
#define KODE_VM_OP_TYPE_EXTERNAL  2
#define KODE_VM_OP_TYPE_WORD      3
#define KODE_VM_OP_TYPE_INT       4
#define KODE_VM_OP_TYPE_FLOAT     5
#define KODE_VM_OP_TYPE_PTR       6
#define KODE_VM_OP_TYPE_DATAPTR   7
#define KODE_VM_OP_TYPE_CODEPTR   8

class KODE_VirtualMachine;
typedef void (*KODE_External)(KODE_VirtualMachine* vm);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_VirtualMachine {

//------------------------------
private:
//------------------------------

  int             MCodeSegment_size                       = 0;
  KODE_Cell       MCodeSegment[KODE_VM_CODE_SEGMENT_SIZE] = {0};
  KODE_Cell       MDataSegment[KODE_VM_DATA_SEGMENT_SIZE] = {0};
  KODE_Cell       MRegisters[KODE_VM_NUM_REGISTERS]       = {0};
  uint32_t        MNumExternals                           = 0;
  KODE_External   MExternals[KODE_VM_MAX_EXTERNALS]       = {0};
  int             MNumOpcodes                             = 0;
  KODE_Opcode*    MOpcodes                                = KODE_NULL;
  KODE_Cell*      MIP                                     = KODE_NULL;
  KODE_CallStack  MCallStack;
  KODE_DataStack  MDataStack;

//------------------------------
public:
//------------------------------

  KODE_VirtualMachine() {
  }

  //----------

  ~KODE_VirtualMachine() {
  }

//------------------------------
public:
//------------------------------

KODE_Cell*  getRegisters()                  { return MRegisters; }
KODE_Cell   getRegister(int i)              { return MRegisters[i]; }
void        setRegister(int i, KODE_Cell v) { MRegisters[i] = v; }

//------------------------------
public:
//------------------------------

  int registerExternal(KODE_External AExternal) {
    int index = MNumExternals;
    MExternals[MNumExternals++] = AExternal;
    return index;
  }

  //----------

  // http://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
  // You may not use this mechanism to jump to code in a different function.
  // If you do that, totally unpredictable things will happen.

  void translate(KODE_Opcode* AOpcodes, int ASize) {
    MOpcodes = AOpcodes;
    MNumOpcodes = ASize;
    execute(-1);
  }

  //----------

  void execute(int APosition=0) {

    if (APosition >= 0) {

      //dtrace( "MCodeSegment = " << (int)&MCodeSegment[0] );
      //dtrace( "MDataSegment = " << (int)&MDataSegment[0] );

      //------------------------------
      // execute
      //------------------------------

      #define VM_NEXT       goto **MIP++
      #define VM_NEXT_cell  *MIP++
      #define VM_PUSH_call  MCallStack.push(MIP)
      #define VM_POP_call   MIP = MCallStack.pop()
      #define VM_PUSH(x)    MDataStack.push((KODE_Cell)x)
      #define VM_POP        MDataStack.pop()
      #define VM_DROP       MDataStack.drop()
      #define VM_DUP        MDataStack.dup()
      #define VM_DUP2       MDataStack.dup2()
      #define VM_POS(x)     MCodeSegment[x]

      //----------

      MIP = &MCodeSegment[APosition];
      VM_NEXT;

      // script internals

      _op_noop:
        VM_NEXT;

      _op_exit:
        //intptr_t r = (intptr_t)VM_POP;
        return;

      _op_push:
        VM_PUSH(VM_NEXT_cell);
        VM_NEXT;

      _op_pushr:
        VM_PUSH( MRegisters[VM_NEXT_cell] );
        VM_NEXT;

      _op_popr:
        {
          KODE_Cell reg = (KODE_Cell)VM_NEXT_cell;
          MRegisters[reg] = (KODE_Cell)VM_POP;
        VM_NEXT;
        }

      _op_pushd: // push from data
        {
          KODE_Cell *dp = (KODE_Cell*)VM_NEXT_cell;
          VM_PUSH(*dp);
        }
        VM_NEXT;

      _op_popd: // pop to data
        {
          KODE_Cell *dp = (KODE_Cell*)VM_NEXT_cell;
          *dp = (KODE_Cell)VM_POP;
        }
        VM_NEXT;

      _op_drop:
        VM_DROP;
        VM_NEXT;

      _op_dup:
        VM_DUP;
        VM_NEXT;

      _op_dup2:
        VM_DUP2;
        VM_NEXT;

      //----------

      _op_call:
        {
          KODE_Cell *ip = (KODE_Cell*)VM_NEXT_cell;
          VM_PUSH_call;
          MIP = ip;
        }
        VM_NEXT;

      _op_ret:
        VM_POP_call;
        VM_NEXT;

      _op_jmp:
        {
          KODE_Cell *ip = (KODE_Cell*)VM_NEXT_cell;
          MIP = ip;
        }
        VM_NEXT;

      _op_jz:
        {
          KODE_Cell* ip = (KODE_Cell*)VM_NEXT_cell;
          if ( (intptr_t)VM_POP==0 ) MIP = ip;
        }
        VM_NEXT;

      _op_jnz:
        {
          KODE_Cell* ip = (KODE_Cell*)VM_NEXT_cell;
          if ( (intptr_t)VM_POP!=0 ) MIP = ip;
        }
        VM_NEXT;

      //----------

      _op_eq:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2==v1));
        }
        VM_NEXT;

      _op_gr:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2>v1));
        }
        VM_NEXT;

      _op_le:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2<v1));
        }
        VM_NEXT;

      _op_ne:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2!=v1));
        }
        VM_NEXT;

      //----------

      _op_add:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2+v1));
        }
        VM_NEXT;

      _op_sub:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2-v1));
        }
        VM_NEXT;

      _op_mul:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2*v1));
        }
        VM_NEXT;

      _op_div:
        {
          intptr_t v1 = (intptr_t)VM_POP;
          intptr_t v2 = (intptr_t)VM_POP;
          VM_PUSH((v2/v1));
        }
        VM_NEXT;

      _op_inc:
        {
          intptr_t* top = (intptr_t*)MDataStack.getTop();
          *top+=1;
        }
        VM_NEXT;

      _op_dec:
        {
          intptr_t* top = (intptr_t*)MDataStack.getTop();
          *top-=1;
        }
        VM_NEXT;

      //----------

      _op_printi:
        {
          intptr_t v = (intptr_t)VM_POP;
          printf("%i\n",(int)v);
        }
        VM_NEXT;

      _op_ext:
        {
          intptr_t external = (intptr_t)VM_POP;
          MExternals[external](this);
        }
        VM_NEXT;

      //------------------------------

      #undef VM_NEXT
      #undef VM_NEXT_cell
      #undef VM_PUSH_call
      #undef VM_POP_call
      #undef VM_PUSH
      #undef VM_POP
      #undef VM_DROP
      #undef VM_DUP
      #undef VM_DUP2
      #undef VM_POS

    } // 0+ (exec)

    //else
    //if (APosition==-2) // jit
    //{
    //} // -2 (jit)

    else if (APosition == -1) { // relocate

      //------------------------------
      // translate
      //------------------------------

      // MOpcodes -> MCodeSegment ++ ...

      #define VM_WRITE(op) MCodeSegment[pos++] = (KODE_Cell)op;

      void* op_adr[] = {
        &&_op_noop,
        &&_op_exit,
        &&_op_push,
        &&_op_drop,
        &&_op_dup,
        &&_op_dup2,
        &&_op_pushr,
        &&_op_popr,
        &&_op_pushd,
        &&_op_popd,
        &&_op_call,
        &&_op_ret,
        &&_op_jmp,
        &&_op_jz,
        &&_op_jnz,
        &&_op_eq,
        &&_op_gr,
        &&_op_le,
        &&_op_ne,
        &&_op_add,
        &&_op_sub,
        &&_op_mul,
        &&_op_div,
        &&_op_inc,
        &&_op_dec,
        &&_op_printi,
        &&_op_ext
      };

      int pos = 0;
      for (int i=0; i<MNumOpcodes; i++) {
        int type = MOpcodes[i].type;
        KODE_Cell value = MOpcodes[i].value;
        switch (type) {
          //case KODE_VM_OP_TYPE_NONE:
          //  break;
          case KODE_VM_OP_TYPE_INTERNAL:
            VM_WRITE( op_adr[ (intptr_t)value ] );
            break;
          case KODE_VM_OP_TYPE_EXTERNAL:
          case KODE_VM_OP_TYPE_WORD:
          case KODE_VM_OP_TYPE_INT:
          case KODE_VM_OP_TYPE_FLOAT:
          case KODE_VM_OP_TYPE_PTR:
            VM_WRITE(value);
            break;
          case KODE_VM_OP_TYPE_DATAPTR:
            VM_WRITE( &MDataSegment[ (intptr_t)value ] );
            break;
          case KODE_VM_OP_TYPE_CODEPTR:
            VM_WRITE( &MCodeSegment[ (intptr_t)value ] );
            break;
        } // switch type
      } // for size
      MCodeSegment_size = pos;

      #undef VM_WRITE

    } // -1 (relocate)

  }

};

//----------------------------------------------------------------------
#endif
