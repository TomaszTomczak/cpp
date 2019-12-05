

// Example program
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

class IntComputer
{
};
class Operation
{
};
std::vector<int> prepareData()
{
    std::vector<int> data = {3, 225, 1, 225, 6, 6, 1100, 1, 238, 225, 104, 0, 1102, 45, 16, 225, 2, 65, 191, 224, 1001, 224, -3172, 224, 4, 224, 102, 8, 223, 223, 1001, 224,
                             5, 224, 1, 223, 224, 223, 1102, 90, 55, 225, 101, 77, 143, 224, 101, -127, 224, 224, 4, 224, 102, 8, 223, 223, 1001, 224, 7, 224, 1, 223, 224, 223, 1102, 52, 6, 225, 1101, 65,
                             90, 225, 1102, 75, 58, 225, 1102, 53, 17, 224, 1001, 224, -901, 224, 4, 224, 1002, 223, 8, 223, 1001, 224, 3, 224, 1, 224, 223, 223, 1002, 69, 79, 224, 1001, 224, -5135, 224,
                             4, 224, 1002, 223, 8, 223, 1001, 224, 5, 224, 1, 224, 223, 223, 102, 48, 40, 224, 1001, 224, -2640, 224, 4, 224, 102, 8, 223, 223, 1001, 224, 1, 224, 1, 224, 223, 223, 1101, 50,
                             22, 225, 1001, 218, 29, 224, 101, -119, 224, 224, 4, 224, 102, 8, 223, 223, 1001, 224, 2, 224, 1, 223, 224, 223, 1101, 48, 19, 224, 1001, 224, -67, 224, 4, 224, 102, 8, 223, 223,
                             1001, 224, 6, 224, 1, 223, 224, 223, 1101, 61, 77, 225, 1, 13, 74, 224, 1001, 224, -103, 224, 4, 224, 1002, 223, 8, 223, 101, 3, 224, 224, 1, 224, 223, 223, 1102, 28, 90, 225, 4,
                             223, 99, 0, 0, 0, 677, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1105, 0, 99999, 1105, 227, 247, 1105, 1, 99999, 1005, 227, 99999, 1005, 0, 256, 1105, 1, 99999, 1106, 227, 99999, 1106, 0, 265,
                             1105, 1, 99999, 1006, 0, 99999, 1006, 227, 274, 1105, 1, 99999, 1105, 1, 280, 1105, 1, 99999, 1, 225, 225, 225, 1101, 294, 0, 0, 105, 1, 0, 1105, 1, 99999, 1106, 0, 300, 1105, 1,
                             99999, 1, 225, 225, 225, 1101, 314, 0, 0, 106, 0, 0, 1105, 1, 99999, 7, 226, 677, 224, 102, 2, 223, 223, 1005, 224, 329, 1001, 223, 1, 223, 8, 226, 677, 224, 1002, 223, 2, 223, 1005,
                             224, 344, 101, 1, 223, 223, 8, 226, 226, 224, 1002, 223, 2, 223, 1006, 224, 359, 101, 1, 223, 223, 1008, 677, 226, 224, 1002, 223, 2, 223, 1005, 224, 374, 1001, 223, 1, 223, 108,
                             677, 677, 224, 1002, 223, 2, 223, 1005, 224, 389, 1001, 223, 1, 223, 1107, 226, 677, 224, 1002, 223, 2, 223, 1006, 224, 404, 101, 1, 223, 223, 1008, 226, 226, 224, 102, 2, 223, 223,
                             1006, 224, 419, 1001, 223, 1, 223, 7, 677, 226, 224, 1002, 223, 2, 223, 1005, 224, 434, 101, 1, 223, 223, 1108, 226, 226, 224, 1002, 223, 2, 223, 1005, 224, 449, 101, 1, 223, 223, 7,
                             226, 226, 224, 102, 2, 223, 223, 1005, 224, 464, 101, 1, 223, 223, 108, 677, 226, 224, 102, 2, 223, 223, 1005, 224, 479, 1001, 223, 1, 223, 1007, 677, 226, 224, 1002, 223, 2, 223,
                             1006, 224, 494, 1001, 223, 1, 223, 1007, 677, 677, 224, 1002, 223, 2, 223, 1006, 224, 509, 1001, 223, 1, 223, 107, 677, 677, 224, 1002, 223, 2, 223, 1005, 224, 524, 101, 1, 223, 223,
                             1108, 226, 677, 224, 102, 2, 223, 223, 1006, 224, 539, 1001, 223, 1, 223, 8, 677, 226, 224, 102, 2, 223, 223, 1005, 224, 554, 101, 1, 223, 223, 1007, 226, 226, 224, 102, 2, 223, 223,
                             1006, 224, 569, 1001, 223, 1, 223, 107, 677, 226, 224, 102, 2, 223, 223, 1005, 224, 584, 1001, 223, 1, 223, 108, 226, 226, 224, 102, 2, 223, 223, 1006, 224, 599, 1001, 223, 1, 223, 107,
                             226, 226, 224, 1002, 223, 2, 223, 1006, 224, 614, 1001, 223, 1, 223, 1108, 677, 226, 224, 1002, 223, 2, 223, 1005, 224, 629, 1001, 223, 1, 223, 1107, 677, 677, 224, 102, 2, 223, 223, 1005,
                             224, 644, 1001, 223, 1, 223, 1008, 677, 677, 224, 102, 2, 223, 223, 1005, 224, 659, 101, 1, 223, 223, 1107, 677, 226, 224, 1002, 223, 2, 223, 1006, 224, 674, 101, 1, 223, 223, 4, 223, 99, 226};

    return data;
}
int getValueBased(int opcode, int parameterNumber, int value, const std::vector<int> &data)
{

    char mode;
    std::ostringstream os;
    os << opcode;
    std::string digits = os.str();
    int returnVal;
    if (digits.length() > 1)
    {
        if (digits.length() - 2 >= parameterNumber)
        {
            mode = digits.at(digits.length() - 2 - parameterNumber);
        }
        else
        {
            mode = '0';
        }
        std::cout << "opcode: " << opcode << " parameterNumber " << parameterNumber << " value: " << value << " mode: " << mode << std::endl;
        switch (mode)
        {
        case '0':
            returnVal = data[value];
            break;
        case '1':
            returnVal = value;
            break;
        default:
            returnVal = -1;
            break;
        }
    }
    else
    {
        return data[value];
        ;
    }

    return returnVal;
}

int calculateData(std::vector<int> &&oper)
{
    int inputStore = 1;
    int outputStore;
    int stepForward = 4;
    //for (int i = 0; i <= oper.size(); i = i + stepForward)
    int i=0;
    while(i<oper.size())
    {
        bool terminate = false;
        std::ostringstream os;
        os << oper[i];
        std::string digits = os.str();
        char op = digits.back();

        switch (op)
        {
        case '1':
            //oper[oper[i + 3]] = oper[oper[i + 1]] + oper[oper[i + 2]];
            std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
            oper[oper[i + 3]] = getValueBased(oper[i], 1, oper[i + 1], oper) + getValueBased(oper[i], 2, oper[i + 2], oper);
            i+ = 4;
            break;
        case '2':
            //oper[oper[i + 3]] = oper[oper[i + 1]] * oper[oper[i + 2]];
            std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
            oper[oper[i + 3]] = getValueBased(oper[i], 1, oper[i + 1], oper) * getValueBased(oper[i], 2, oper[i + 2], oper);
            i+ = 4;
            break;
        case '3':
            std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << std::endl;
            std::cout << "load input: " << inputStore << " to position: " << oper[i + 1] << std::endl;
            oper[oper[i + 1]] = inputStore;
            i+ = 2;
            break;
        case '4':
            std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << std::endl;
            std::cout << "save output: " << outputStore << " from: " << oper[i + 1] << std::endl;
            outputStore = oper[oper[i + 1]];
            i+ = 2;
            break;
        case '9':
            terminate = true;
            std::cout << "termination" << std::endl;
            break;
        default:

            break;
        }
        if (terminate)
        {
            break;
        }
    }

    std::cout << "output: " << outputStore << std::endl;
    std::cout << "input: " << inputStore << std::endl;
    return oper[0];
}

int main()
{
    // adventCodeDay2_2();
    // adventCodeDay3_2();
    calculateData(prepareData());
}
