#include <bits/stdc++.h>

using namespace std;

string int_to_hex(int i)
{
	stringstream stream;
	stream << setfill('0') << setw(8)
		   << hex << i;
	return stream.str();
}

//functions used
tuple<int32_t, int32_t> inst_to_operand_opcode(int32_t instr);
void mem_dump(int completion_point, ofstream &trcfile);
void mot_init();
int prog_trace(int PC, int completion_point, ofstream& trcfile);
map<int, string> mot;
int32_t memory[10000];
vector<int> data_pc;
int32_t A, B, PC, SP;

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cout << " ./a.exe [option] file.o" << endl;
		cout << "Available Options:" << endl;
		cout << "-trace   show instruction trace" << endl;
		cout << "-before  show memory dump before execution" << endl;
		cout << "-after   show memory dump after execution" << endl;
		return (0);
	}


	mot_init();
	string in_file = argv[2];
	string trace_file = in_file.substr(0, in_file.find(".", 0)) + ".trace";
	ifstream infile;
	ofstream trcfile;
	infile.open(in_file, ios::out | ios::binary);
	trcfile.open(trace_file);

	// 32 bit integer for machine code
	int32_t instr_code;

	int completion_point = 0;

	// mchine code into main memory
	while (infile.read((char *)&instr_code, sizeof(instr_code)))
	{
		int32_t operand, operation;
		tie(operand, operation) = inst_to_operand_opcode(instr_code);

		
		if (operation == 19 || operation == 20)
		{
			memory[completion_point] = operand;
			data_pc.push_back(completion_point++);
		}

		// Other instructions stored in memory
		else if (operation >= 0 && operation < 20)
		{
			memory[completion_point++] = instr_code;
		}

		// Identify invalid instruction
		else
		{
			cout << "Invalid instruction\n";
			return (0);
		}
	}

	A = 0;
	B = 0;
	PC = 0;
	SP = sizeof(memory) / sizeof(memory[0]) - 1;

	// Tracing for each executed instruction
	if (string(argv[1]) == "-trace")
		prog_trace(0, completion_point, trcfile);

	// Memory dump before execution
	else if (string(argv[1]) == "-before")
		mem_dump(completion_point, trcfile);

	// Memory dump after execution
	else if (string(argv[1]) == "-after")
	{
		prog_trace(0, completion_point, trcfile);
		mem_dump(completion_point, trcfile);
	}
	else{
		cout<<"Invalid option"<<endl;
	}

	infile.close(); // Close file
}


// MOT table
void mot_init()
{
	mot[0] = ("ldc");
	mot[1] = ("adc");
	mot[2] = ("ldl");
	mot[3] = ("stl");
	mot[4] = ("ldnl");
	mot[5] = ("stnl");
	mot[6] = ("add");
	mot[7] = ("sub");
	mot[8] = ("shl");
	mot[9] = ("shr");
	mot[10] = ("adj");
	mot[11] = ("a2sp");
	mot[12] = ("sp2a");
	mot[13] = ("call");
	mot[14] = ("return");
	mot[15] = ("brz");
	mot[16] = ("brlz");
	mot[17] = ("br");
	mot[18] = ("HALT");
	mot[19] = ("data");
	mot[20] = ("SET");
}


//split to operand n operation
tuple<int32_t, int32_t> inst_to_operand_opcode(int32_t instr)
{
	int32_t opcode, operand;
	opcode = instr & 0xff;
	operand = instr & 0xffffff00;
	operand >>= 8;
	return (make_tuple(operand, opcode));
}


//memory state
void mem_dump(int completion_point, ofstream &trcfile)
{
	cout << "Memory Dump";
	trcfile << "Memory Dump";
	// cout<<completion_point<<endl;
	for (int i = 0; i < completion_point; i++)
	{
		if (!(i % 4))
		{
			cout << endl
				 << endl
				 << int_to_hex(i) << "\t" << int_to_hex(memory[i]) << " ";
			trcfile << endl
					<< endl
					<< int_to_hex(i) << "\t" << int_to_hex(memory[i]) << " ";
		}
		else
			cout << int_to_hex(memory[i]) << " ";
		trcfile << int_to_hex(memory[i]) << " ";
	}
	cout << endl;
}



//trace feature
int prog_trace(int PC, int completion_point, ofstream& trcfile)
{
	
	int count = 0;

	while(PC< completion_point)
	{
		int old_pc = PC;
		int32_t operand, operation;
		int32_t instr = memory[PC];
		tie(operand, operation) = inst_to_operand_opcode(instr);
		trcfile << "PC: " << int_to_hex(PC) << "\tSP: " << int_to_hex(SP) << "\tA: " 
		<< int_to_hex(A) << "\tB: " << int_to_hex(B) << "\t" << mot[operation] 
		<< " " <<operand << endl << endl;
		cout << "PC: " << int_to_hex(PC) << "\tSP: " << int_to_hex(SP) << "\tA: " 
		<< int_to_hex(A) << "\tB: " << int_to_hex(B) << "\t" << mot[operation] 
		<< " " <<operand << endl << endl;
		if (std::find(data_pc.begin(), data_pc.end(), PC) != data_pc.end())
		{
        	PC++;
        	continue;
		}
		switch(operation) {
            case 0:
                B = A;
                A = operand;
                break;
            case 1:
                A += operand;
                break;
            case 2:
                B = A;
                A = memory[SP+operand];
                break;
            case 3:
                memory[SP+operand] = A;
                A = B;
                break;
            case 4:
                A = memory[A+operand];
                break;
            case 5:
                memory[A+operand] = B;
                break;
            case 6:
                A += B;
                break;
            case 7:
                A = B - A;
                break;
            case 8:
                A = B << A;
                break;
            case 9:
                A = B >> A;
                break;
            case 10:
                SP += operand;
                break;
            case 11:
                SP = A;
                A = B;
                break;
            case 12:
                B = A;
                A = SP;
                break;
            case 13:
                B = A;
                A = PC;
                PC += operand;
                break;
            case 14:
                PC = A;
                A = B;
                break;
            case 15:
                if(A == 0) 
                	PC += operand;
                break;
            case 16:
                if(A < 0) 
                	PC += operand;
                break;
            case 17:
                PC += operand;
                break;
            case 18:
            	cout << "Number of instructions executed =>  "<<count << endl << endl;
                return 0;
        }
        PC++;
		if(old_pc == PC)
		{
			cout << "Infinite loop found..... Terminating" << endl;
			break;
		}
		count++;
	}
	cout << "Number of instructions executed =>  "<<count << endl << endl;
                return 0;
}
