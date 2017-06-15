#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <chrono>
#include "ByteCode.h"
#include "vm.h"


#include <stdio.h>
using namespace vm;
using namespace std::chrono;
using Type = vm::VM::Type;

#include "Measurement.h"

#define N 5.0

long long test(long double n);

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <malloc.h>
#include <crtdbg.h>

void OutputHeading(const char * explanation)
{
	_RPT1(_CRT_WARN, "\n\n%s:\n**************************************\
************************************\n", explanation);
}

#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)) // Get the old setting
#define  CLEAR_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif



int main() {

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	SET_CRT_DEBUG_FIELD(_CRTDBG_LEAK_CHECK_DF);
	SET_CRT_DEBUG_FIELD(_CRTDBG_CHECK_ALWAYS_DF);


	{
		//Type CodeLoop[] = {
		//							
		//   // N = 10						ADDRESS
		//	INSTR(IPUSH, 10.0,INT),			// 0  Push Constant (10) Onto Stack 
		//	INSTR(GSTORE, 0ull,POINTER),			// 2  Store StackTop{Constant (10)} Globally var(N)								 
		//	INSTR(IPUSH, 0.0,INT),			// 4  Push Constant (0) Onto Stack
		//	INSTR(GSTORE, 1ull,INT),			// 6  Store StackTop{Constant (10)} Globally var(i)
		//									// do{
		//	INSTR(GLOAD, 1ull,POINTER),			// 15 Load i
		//	INSTR(IPUSH, 1.0,INT),			// 17 Push Constant (1) Onto Stack 
		//	SINSTR(IADD),					// 19 i = i+1
		//	INSTR(GSTORE, 1ull,POINTER),
		//									// }while( i < N)
		//	INSTR(GLOAD, 1ull,POINTER),			// 20
		//	INSTR(GLOAD, 0ull,POINTER)	,			// 22
		//	SINSTR(ILT),					// 24
		//	INSTR(BRTRUE, 8ull,POINTER),		// 25;
		//									
		//	SINSTR(HALT)					// 26
		//};

		//Recursive
		//Type factorial[] = {
		//	//Instruction							// ADDRESS
		//	///Function factorial(var N)			// ARGS=1, LOCALS=0		
		//	INSTR(LOAD, -3.0,INT),					// [0]		Copy Given Argument N Into Current StackFrame
		//	INSTR(IPUSH, 2.0,INT),					// [2]		Push Constant (2) Onto Stack
		//	SINSTR(ILT),							// [4]		IF ((N < 2) == TRUE) Push TRUE ELSE Push FALSE Onto Stack
		//	INSTR(BRFALSE, 10ull,POINTER),			// [5]		Jump to Addr 10 if on Stack lays FALSE
		//	INSTR(IPUSH, 1.0,INT),					// [7]		Push Constant (1) Onto Stack
		//	SINSTR(RET),							// [9]		Return Constant (1)[7]
		//											// 
		//											//			RETURN N * factorial( N-1 )
		//	INSTR(LOAD, -3.0,INT),					// [10]		Copy Given Argument N Into Current StackFrame
		//	INSTR(LOAD, -3.0,INT),					// [12]		Copy Given Argument N Into Current StackFrame
		//	INSTR(IPUSH, 1.0,INT),					// [14]		Push Constant (1) Onto Stack
		//	SINSTR(ISUB),							// [16]		Substract Constant (1) of Value (N) 
		//	DINSTR(CALL, 0ull, 1.0,POINTER,INT),	// [17]		Call Function: factorial(var N)
		//	SINSTR(IMUL),							// [20]		Multiplicate Result of (factorial(var N))[17] times Value (N)[10]
		//	SINSTR(RET),							// [21]		Return Result[20]
		//	/// Function Main()						// ARGS=0, LOCALS=0
		//	//										 <-- MAIN METHOD!
		//	INSTR(IPUSH, N,INT),					// [22]		Push Constant (5) Onto StacK
		//	DINSTR(CALL, 0ull, 1.0,POINTER,INT),	// [25]		Call Function: factorial(var N)
		//    SINSTR(PRINT),							// [26]		Print Stack Top
		//	SINSTR(HALT)							// [27]		Abort
		//};
		//Recursive
		Type fibonacci[] = {

			INSTR(LOAD,-3.0,INT),				 // [0]		Copy Given Argument N Into Current StackFrame
			INSTR(IPUSH,0.0,INT),				 // [2]		Push Constant (0) Onto Stack
			SINSTR(IEQ),						 // [4]		IF ((N == 0)) Push TRUE ELSE Push FALSE Onto Stack
			INSTR(BRFALSE, 10ull ,POINTER),		 // [5]		Jump to Addr 10 if on Stack lays FALSE
			INSTR(IPUSH, 0.0,INT),				 // [7]		Push Constant (0) Onto Stack
			SINSTR(RET),						 // [9]		Return Constant (0)

			INSTR(LOAD,-3.0,INT),				 // [10]	Copy Given Argument N Into Current StackFrame
			INSTR(IPUSH,1.0,INT),				 // [12]	Push Constant (1) Onto Stack
			SINSTR(IEQ),						 // [14]	IF ((N == 0)) Push TRUE ELSE Push FALSE Onto Stack
			INSTR(BRFALSE, 20ull ,POINTER),		 // [15]	Jump to Addr 10 if on Stack lays FALSE
			INSTR(IPUSH, 1.0,INT),				 // [17]	Push Constant (1) Onto Stack
			SINSTR(RET),						 // [19]	Return Constant (1)

			INSTR(LOAD,-3.0,INT),				 // [20]    Copy Given Argument N Into Current StackFrame
			INSTR(IPUSH,1.0,INT),				 // [22]	Push Constant (1) Onto Stack
			SINSTR(ISUB),						 // [24]	Substract Constant (1) of Value (N) 
			DINSTR(CALL,0ull,1.0,POINTER,INT),	 // [25]    Call Fibonacci(N-1)

			INSTR(LOAD,-3.0,INT),				 // [28]    Copy Given Argument N Into Current StackFrame
			INSTR(IPUSH,2.0,INT),				 // [30]	Push Constant (2) Onto Stack
			SINSTR(ISUB),						 // [32]	Substract Constant (2) of Value (N) 
			DINSTR(CALL,0ull,1.0,POINTER,INT),	 // [33]    Call Fibonacci(N-2)

			SINSTR(IADD),						 // [36]    Add Fibonacci(N-1) and Fibonacci(N-2)
			SINSTR(RET)	,						 // [37]	Return Result


												 /// Function Main()					// ARGS=0, LOCALS=0
												 //									 <-- MAIN METHOD!
												 INSTR(IPUSH, N,INT),				// [38]		Push Constant (5) Onto StacK
												 DINSTR(CALL, 0ull, 1.0,POINTER,INT),// [40]		Call Function: factorial(var N)
												 SINSTR(PRINT),						// [43]		Print Stack Top
												 SINSTR(HALT)						// [44]		Abort



		};

		//Type fibonacci2[] = {
		//	INSTR(LOAD,-3.0,INT),				 // [0]		Copy Given Argument N Into Current StackFrame
		//	INSTR(IPUSH,0.0,INT),				 // [2]		Push Constant (0) Onto Stack
		//	SINSTR(IEQ),						 // [4]		IF ((N == 0)) Push TRUE ELSE Push FALSE Onto Stack
		//	INSTR(BRFALSE, 10ull ,POINTER),		 // [5]		Jump to Addr 10 if on Stack lays FALSE
		//	INSTR(IPUSH, 0.0,INT),				 // [7]		Push Constant (0) Onto Stack
		//	SINSTR(RET),						 // [9]		Return Constant (0)
		//	INSTR(LOAD,-3.0,INT),				 // [10]	Copy Given Argument N Into Current StackFrame
		//	INSTR(IPUSH,1.0,INT),				 // [12]	Push Constant (1) Onto Stack
		//	SINSTR(IEQ),						 // [14]	IF ((N == 0)) Push TRUE ELSE Push FALSE Onto Stack
		//	INSTR(BRFALSE, 20ull ,POINTER),		 // [15]	Jump to Addr 10 if on Stack lays FALSE
		//	INSTR(IPUSH, 1.0,INT),				 // [17]	Push Constant (1) Onto Stack
		//	SINSTR(RET),						 // [19]	Return Constant (1)
		//	INSTR(IPUSH, 1.0,INT),				 // [20]  Push Constant (0) Onto Stack
		//	INSTR(GSTORE, 0ull,INT),			 // [22]  Store StackTop{Constant (0)} Globally var(i)
		//	INSTR(IPUSH, 0.0,INT),				 // [24]  Push Constant (0) Onto Stack
		//	INSTR(GSTORE, 1ull,INT),			 // [26]  Store StackTop{Constant (0)} Globally var(H1)
		//	INSTR(IPUSH, 1.0,INT),				 // [28]  Push Constant (0) Onto Stack
		//	INSTR(GSTORE, 2ull,POINTER),		 // [30]  Store StackTop{Constant (0)} Globally var(H2)
		//	//BEGIN LOOP
		//	INSTR(GLOAD,0ull,POINTER),			 // [32] LOAD Var(i)
		//	INSTR(LOAD,-3.0,INT),				 // [34] Copy Given Argument N Into Current StackFrame
		//	SINSTR(ILT),						 // [36] IF( i < N)
		//	INSTR(BRFALSE, 54ull, POINTER),		 // [37 ]LOOP
		//	
		//	INSTR(GLOAD,0ull,POINTER),			 // [39] LOAD Var(i)
		//	INSTR(IPUSH,1.0,INT),				 // [41] Push Constant (1) Onto Stack
		//	SINSTR(IADD),						 // [43] ADD Var(i) and 1
		//	INSTR(GSTORE,0ull,POINTER),			 // [44] Var(i) = Var(i) + 1;
		//	INSTR(GLOAD,1ull,POINTER),			 // [46] LOAD Var(H1)
		//	INSTR(GLOAD,2ull,POINTER),			 // [48] LOAD Var(H2)
		//	SINSTR(IADD),						 // [50] Add H1 + H2;
		//	INSTR(GSTORE,3ull,POINTER),			 // [51] Store Var(Result)
		//	INSTR(GLOAD,1ull,POINTER),			 // [53] LOAD Var(H1)
		//	INSTR(GSTORE, 2ull,INT),			 // [55] Store Var(H1) Into Var(H2)
		//	INSTR(GLOAD,3ull,POINTER),			 // [57] LOAD Var(Result)
		//	INSTR(GSTORE, 2ull,INT),			 // [59] Store Var(Result) Into Var(H2)
		//	INSTR(BRANCH,32ull,POINTER),		 // [61] JUMP to [24]
		//	//END LOOP
		//	INSTR(GLOAD,3ull,POINTER),			 // [63]
		//	SINSTR(RET),						 // [65]
		//	/// Function Main()					// ARGS=0, LOCALS=0
		//	//									 <-- MAIN METHOD!
		//	INSTR(IPUSH, N,INT),				// [66]		Push Constant (5) Onto StacK
		//	DINSTR(CALL, 0ull, 1.0,POINTER,INT),// [68]		Call Function: Fibonacci(var N)
		//	SINSTR(PRINT),						// [71]		Print Stack Top
		//	SINSTR(HALT)						// [72]		Abort
		//};


		//VM::Memory CodeInstructions2(std::begin(fibonacci2), std::end(fibonacci2));
		VM::Memory CodeInstructions(std::begin(fibonacci), std::end(fibonacci));


		VM Machine(CodeInstructions, 38);
		//VM Machine(CodeInstructions2, 66);


		try {
			///auto exe = &VM::cpu;
			//auto avg = (measure<std::chrono::nanoseconds>::duration(Machine) );

			//auto avg2 = (measure<std::chrono::nanoseconds>::duration(test,N));



			//std::cout << "Average Time: " << avg.count() * std::pow(10, -6) << " ms\n";
			//std::cout << "Average Time: " << avg2.count()* std::pow(10, -6) << " ms\n";
			//std::cout << "Delta Time: " << (avg - avg2).count() * std::pow(10,-6)<<"ms\n";

			//Machine.cpu();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	_CrtDumpMemoryLeaks();
	_CrtCheckMemory();

	std::cin.get();
	return 0;
}

long long test(long double n)
{
	if (n < 2)
		return 1;
	return (n*test(n - 1));
}