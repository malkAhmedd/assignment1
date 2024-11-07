#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

class ALU {
public:
    string decimalToHex(int decimal) {
        if (decimal == 0) return "0";

        string hex = "";
        char hexDigits[] = "0123456789ABCDEF";
        while (decimal > 0) {
            int remainder = decimal % 16;
            hex = hexDigits[remainder] + hex;
            decimal /= 16;
        }
        return hex;
    }

    bool isValidLine(const string& line) {
        if (line.size() != 4) return false;

        char firstChar = line[0];
        return (firstChar >= '1' && firstChar <= '6') || (firstChar == 'B' || firstChar == 'C');
    }
};

class Memory {
protected:
    map<string, string> m;
    ALU alu;

public:
    Memory() {
        for (int i = 0; i < 256; i++) {
            string ch = alu.decimalToHex(i);
            m[ch] = "00";
        }
    }
//function
    void display() {
        for (const auto& it : m) {
            cout << it.first << ": " << it.second << endl;
        }
    }
};

class Registers {
protected:
    map<string, string> reg;
    ALU alu;

public:
    Registers() {
        for (int i = 0; i < 16; i++) {
            string ch = alu.decimalToHex(i);
            reg[ch] = "00";
        }
    }

    void display() {
        for (const auto& it : reg) {
            cout << it.first << ": " << it.second << endl;
        }
    }
};

class CU : public ALU {
protected:
    map<string, string>& m;
    map<string, string>& reg;

public:
    CU(map<string, string>& m, map<string, string>& reg) : m(m), reg(reg) {}

    void execute(const string& s) {
        if (s[0] == '2') {  // تحقق إذا كانت التعليمات تبدأ بـ "2"
            string address = s.substr(2, 2);
            auto it = m.find(address);

            if (it != m.end()) {
                string indexreg = s.substr(1, 1);
                if (indexreg == "4") {  // تأكد من أنه السجل 4
                    reg["4"] = address;  // تخزين "A3" في السجل "4"
                    cout << "Stored value " << address << " into register 4" << endl;
                } else {
                    reg[indexreg] = it->first;
                    cout << "Loaded address " << it->first << " into register " << indexreg << endl;
                }
            } else {
                cout << "Memory address " << address << " not found!" << endl;
            }
        }
        // باقي التعليمات...
    }

    void displayErrorState() {
        cout << "Error: Invalid State!" << endl;
    }
};

class CPU : public Memory, public Registers, public CU {
public:
    CPU(map<string, string>& m, map<string, string>& reg) : CU(m, reg) {}

    void loadProgrammingFile() {
        ifstream inputFile("E:/2nd year/OOP/assignments/file of task4.txt");
        if (!inputFile) {
            cout << "Error opening file!" << endl;
            return;
        }

        string instructionLine;
        while (getline(inputFile, instructionLine)) {
            if (isValidLine(instructionLine)) {
                cout << "Executing instruction: " << instructionLine << endl;
                execute(instructionLine);
            } else {
                cout << "Invalid line format: " << instructionLine << endl;
                std::exit(0);
            }
        }

        inputFile.close();
    }

    void display() {
        cout << "Registers:" << endl;
        Registers::display();
        cout << "\nMemory:" << endl;
        Memory::display();
    }
};

int main() {
    map<string, string> memoryMap, regMap;
    CPU cpu(memoryMap, regMap);

    char s;
    while (true) {
        cout << "Enter the choice:\n A: Load programming file \n B: Display Memory and Registers \n C: Exit \n";
        cin >> s;

        s = toupper(s);

        if (s != 'A' && s != 'B' && s != 'C') {
            cout << "Please enter a valid choice!\n";
            continue;
        }

        if (s == 'A') {
            cpu.loadProgrammingFile();
        }
        else if (s == 'B') {
            cpu.display();
        }
        else {
            cout << "Exit" << endl;
            break;
        }
    }

    return 0;
}
