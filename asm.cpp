// Eʀᴜᴍ Mᴇʀᴀᴊ
// ʀᴏʟʟ ɴᴏ.: 2201ᴄs24
// Dᴇᴄʟᴀʀᴀᴛɪᴏɴ ᴏғ Aᴜᴛʜᴏʀsʜɪᴘ
// Tʜɪs ᴄᴘᴘ ғɪʟᴇ, ᴀsᴍ.ᴄᴘᴘ, ɪs ᴘᴀʀᴛ ᴏғ ᴛʜᴇ ᴀssɪɢɴᴍᴇɴᴛ ᴏғ CS210 ᴀᴛ ᴛʜᴇ ᴅᴇᴘᴀʀᴛᴍᴇɴᴛ ᴏғ CSE, IIT Pᴀᴛɴᴀ.
#include <bits/stdc++.h>

using namespace std;
#define ll long long
#define loop while
#define vec vector
#define zero 0
#define one 1
#define newline "\n"

// Define structure for symbols and literalss
struct literal
{
    int mem_addresss;
    int literal;
};
struct symbol
{
    string name;
    int mem_addresss;
    bool set;
    int set_value;
};

// Literal and symbol table
map<string, string> mot;
vector<literal> table_lit;
vector<symbol> table_symboll;
// MOT table

static inline string &trim(string &test_string);
int str_to_num(string s);
bool check_num(const string &s);
bool belongs(string name);
int finding_if_valid_label(string label);
void mot_init();
string str_init();
string instruction_to_table(string inp_string, int line, int *location_ptr);
tuple<string, string, string> instruction_to_code(string inp_string, int *location_ptr, int line);
void pass_2_assembler(string file, ofstream &out_file_assembler_stream, ofstream &log_file_assembler_stream, ofstream &obj_file_assembler_stream);
void pass_1_assembler(string file, ofstream &log_file_assembler_stream);

int main(int argc, char *argv[])
{
    // Initalize Machine operation table
    mot_init();

    // Argument for input file
    string in_file_assembler = argv[1];

    // Argument for output file
    string out_file_assembler = in_file_assembler.substr(0, in_file_assembler.find(".", 0)) + ".L";

    // Argument for object file
    string obj_file_assembler = in_file_assembler.substr(0, in_file_assembler.find(".", 0)) + ".o";
    // Argument for log file
    string log_file_assembler = in_file_assembler.substr(0, in_file_assembler.find(".", 0)) + ".log";

    // Defining output listing, log file
    ofstream logfile(log_file_assembler);
    ofstream outfile(out_file_assembler);
    ofstream objfile(obj_file_assembler, ios::out | ios::binary);

    // Pass-1 of assembly, analysis phase
    pass_1_assembler(in_file_assembler, logfile);

    // Pass-2 of assembly, synthesis phase
    pass_2_assembler(in_file_assembler, outfile, logfile, objfile);

    logfile.close();
    objfile.close();
    // Close files
    outfile.close();
}

int str_to_num(string s)
{
    s = trim(s);
    if (s.find("0x") == zero)
    {
        return stoul(s, nullptr, 16);
    }
    else if (s.find("-") == zero)
    {
        return -stoul(s.substr(1, s.length()), nullptr, 10);
    }
    else if (s.find("+") == zero)
    {
        return stoul(s.substr(1, s.length()), nullptr, 10);
    }
    else if (s.find("0") == zero)
    {
        return stoul(s, nullptr, 8);
    }
    else
    {
        return stoul(s, nullptr, 10);
    }
}

static inline string &trim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(),
                    not1(ptr_fun<int, int>(isspace)))
                .base(),
            s.end());
    s.erase(s.begin(), find_if(s.begin(), s.end(),
                               not1(ptr_fun<int, int>(isspace))));
    return s;
}

void mot_codes()
{
    string codes[] = {"00",
                      "01",
                      "02",
                      "03",
                      "04",
                      "05",
                      "06",
                      "07",
                      "08",
                      "09",
                      "0a",
                      "0b",
                      "0c",
                      "0d",
                      "0e",
                      "0f",
                      "10",
                      "11",
                      "12",
                      "13",
                      "14"};
    string mnemo[] = {"ldc",
                      "adc",
                      "ldl",
                      "stl",
                      "ldnl",
                      "stnl",
                      "add",
                      "sub",
                      "shl",
                      "shr",
                      "adj",
                      "a2sp",
                      "sp2a",
                      "call",
                      "return",
                      "brz",
                      "brlz",
                      "br",
                      "HALT",
                      "data",
                      "SET"};
    for (int i = 0; i < 21; i++)
    {
        mot[mnemo[i]] = codes[i];
    }
}
int check_valid_label(string label)
{
    if ((1 || (label[0] >= 'A' && label[0] <= 'Z') || (label[0] >= 'a' && label[0] <= 'z')))
    {
        int i = 0;
        while (i < label.length())
        {
            if (!(isdigit(label[i]) || (label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z')))
            {
                return false;
            };
            i++;
        }

        return true;
    }
    else
    {
        return false;
    }
}

// if string is a number
bool check_num(const std::string &s)
{
    
            int i = 0;
            string str_temp;
            str_temp = s;
            if (s.front() == '-' or s.front() == '+')
            {
                str_temp = s.substr(1, s.length());
            }
            else if (s.find("0x") == zero)
            {
                str_temp = s.substr(2), 16;
            }
            std::string::const_iterator it = str_temp.begin();
            while (it != str_temp.end() && std::isdigit(*it))
                ++it;
            return !str_temp.empty() && it == str_temp.end();
    
}

bool belongs(string name)
{
    if (name != "")
    {
        auto iter = find_if(table_symboll.begin(), table_symboll.end(),
                            [&](const symbol &ts)
                            { return ts.name == name; });
        return iter != table_symboll.end();
    }
    else
    {
        auto iter = find_if(table_symboll.begin(), table_symboll.end(),
                            [&](const symbol &ts)
                            { return ts.name == name; });
        return iter != table_symboll.end();
    }
}

void mot_init()
{
    mot_codes();
}

// Utility function to convert int to hex
string int_to_hex(int i)
{
    stringstream stream;
    stream << setfill('0') << setw(8)
           << hex << i;
    return stream.str();
}

string instruction_to_table(string instr, int line, int *location__ptr)
{
    int loc = *location__ptr;
    string errors = str_init();

    // Identify label and variables
    if (instr.find(':') != string::npos)
    {
        int lable_colon = instr.find(":", 0);

        if (belongs(instr.substr(0, lable_colon)))
        {
            string e = "ERROR: Duplicate Label at line ";
            cout << e << line << endl;
            errors += e + to_string(line) + newline;
        }

        if (!check_valid_label(instr.substr(0, lable_colon)))
        {
            string e = "WARNING: Incorrect label format at line ";
            cout << e << line << endl;
            errors += e + to_string(line) + newline;
        }

        // Instruction could be present after the lable_colon
        if (lable_colon != instr.length() - 1)
        {
            string subs = instr.substr(lable_colon + 1, instr.length());
            subs = trim(subs);
            instruction_to_table(subs, line, &loc);
            int space = subs.find(" ", 0);
            string sub_op = subs.substr(0, space);
            string sub_val = subs.substr(space + 1, subs.length());
            sub_op = trim(sub_op);
            sub_val = trim(sub_val);

            // Dealing with set instructions
            if (sub_op != "SET")
            {
                table_symboll.push_back({instr.substr(0, lable_colon), loc, 0, -1});
            }
            else
            {
                table_symboll.push_back({instr.substr(0, lable_colon), loc, 1, stoi(sub_val)});
            }
        }
        else
        {
            table_symboll.push_back({instr.substr(0, lable_colon), loc, 0, -1});
            *location__ptr = *location__ptr - 1;
        }
    }

    // Identify literals and constants
    else
    {
        // Considering that second part of instruction is operand
        int space = instr.find(" ", 0);
        string subs = instr.substr(space + 1, instr.length());
        subs = trim(subs);

        // Checking for numeral
        if (check_num(subs))
        {
            table_lit.push_back({str_to_num(subs), -1});
        }
        
    }
    return (errors);
}

tuple<string, string, string> instruction_to_code(string instr, int *location__ptr, int line)
{
    // Program Counter
    int loc = *location__ptr;

    // Hex Code of Program Counter
    string encoding = int_to_hex(loc) + " ";

    // Warnings and errors
    string encoding_ = str_init();
    string errors = str_init();
    string machine_code = str_init();

    // Identify label and variables
    if (instr.find(':') != string::npos)
    {
        int lable_colon = instr.find(":", 0);

        // If no instruction after lable_colon, PC shouldn't change
        
        if (lable_colon == instr.length() - 1)
        {
            encoding += "         ";
            *location__ptr = *location__ptr - 1;
        }
        // Instruction could be present after the lable_colon
        else
        {
            string subs = instr.substr(lable_colon + 1, instr.length());
            subs = trim(subs);
            tie(encoding_, errors, machine_code) = instruction_to_code(subs, &loc, line);
            string str_temp = encoding_;
            str_temp = str_temp.substr(9, 9);
            encoding += str_temp;
        }
        encoding += instr + newline;
    }

    // Identify literals and constants
    else
    {
        // Considering that second part of instruction is operand
        int space = instr.find(" ", 0);

        string hex_string;
        string sub_operation = instr.substr(0, space);

        // String containing operand
        string sub_operand = instr.substr(space + 1, instr.length());

        sub_operand = trim(sub_operand);
        sub_operation = trim(sub_operation);

        // Checking for invalid mnemonics
        if (mot[sub_operation] == "")
        {
            string e = "ERROR: Mnemonic not defined at line ";
            errors += e + to_string(line) + newline;
            cout << e << line << endl;
        }

        // Checking for No operand instructions
        else if (sub_operation == "add" || sub_operation == "sub" || sub_operation == "shl" || sub_operation == "shr" || sub_operation == "a2sp" || sub_operation == "sp2a" || sub_operation == "return" || sub_operation == "HALT")
        {
            encoding += "000000" + mot[sub_operation] + " ";
            machine_code += "000000" + mot[sub_operation] + " ";
            if (sub_operation.length() != instr.length())
            {
                string e = "ERROR: Operand not expected at line ";
                errors += e + to_string(line) + newline;
                cout << e << line << endl;
            }
        }
        // Checking for numeral operand to encode directly
        else if (check_num(sub_operand))
        {
            hex_string = int_to_hex(str_to_num(sub_operand));
            if (sub_operation == "SET" || sub_operation == "data")
                encoding += hex_string + " ";
            else
                encoding += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " ";
            // encoding += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " ";
            machine_code += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " ";
        }
        // Checking for variable operand to encode address
        else
        {
            int defined = 0;
            int i = 0;
            while (i < table_symboll.size())
            {
                if (table_symboll[i].name.compare(sub_operand) == zero)
                {
                    defined = 1;
                    // SET variables considered numeral
                    if (table_symboll[i].set)
                    {
                        hex_string = int_to_hex(table_symboll[i].set_value);
                    }
                    // Operands which need offset from PC
                    else if (sub_operation == "call" || sub_operation == "brz" || sub_operation == "brlz" || sub_operation == "br")
                    {
                        hex_string = int_to_hex(table_symboll[i].mem_addresss - loc - 1);
                    }
                    // Normal case, encode mem_addresss
                    else
                    {
                        hex_string = int_to_hex(table_symboll[i].mem_addresss);
                    }
                    encoding += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " ";
                    machine_code += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " ";
                    break;
                }
                i++;
            }
            if (sub_operation.length() == instr.length())
            {
                string e = "ERROR: Operand expected at line ";
                errors += e + to_string(line) + newline;
                cout << e << line << endl;
            }
            else if (!defined)
            {
                string e = "ERROR: Unknown Symbol as operand at line ";
                errors += e + to_string(line) + newline;
                cout << e << line << endl;
            }
        }
        encoding += instr + newline;
    }
    return (make_tuple(encoding, errors, machine_code));
}

string str_init()
{
    return "";
}

void pass_2_assembler(string file, ofstream &outfile, ofstream &logfile, ofstream &objfile)
{
    int line_count = 1;
    // str_temp str variable for get line
    string line;
    ifstream MyFile(file);
    // Program Counter
    int loc = 0;
    while (getline(MyFile, line))
    {
        // Pre-process the line, trim extra spaces
        
            string instr;
            instr = line.substr(0, line.find(";", 0));
            instr = trim(instr);

            // Skip empty lines
            if (instr == "")
            {
                line_count++;
                continue;
            }
            else
            {
                // Write Encoded instruction into listing file
                string errors, machine_code,encoding ;
                tie(encoding, errors, machine_code) = instruction_to_code(instr, &loc, line_count++);
                outfile << encoding;
                if (machine_code != "")
                {
                    uint32_t str_temp = stoul("0x" + trim(machine_code), nullptr, 16);
                    objfile.write((char *)&str_temp, sizeof(str_temp));
                }
                logfile << errors;
                loc++;
            }
        
    }
}

void pass_1_assembler(string file, ofstream &logfile)
{
    // get line
    string line;
    int loc = 0;
    int line_count = 1;

    // Reading the input file
    ifstream MyFile(file);

    // Read individual lines
    while (getline(MyFile, line))
    {
        // Pre-process the line, trim extra spaces
        string instr;
        instr = line.substr(0, line.find(";", 0));
        instr = trim(instr);

        // Skip empty lines
        if (instr == "")
        {
            line_count++;
            continue;
        }

        logfile << instruction_to_table(instr, line_count++, &loc);
        loc++;
    }

    // LTORG loop for literals
    int i = 0;
    while (i < table_lit.size())
    {
        if (table_lit[i].mem_addresss == -1)
        {
            table_lit[i].mem_addresss = loc++;
        }
        i++;
    }

    MyFile.close();
}
