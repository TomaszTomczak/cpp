

// Example program
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

class IntComputer
{
public:
    std::vector<int> oper;
    IntComputer *nextComp;
    bool isLast;
    int phase;
    int finalValue;
    std::string name; 
    bool firstInputRequested = false;
    int i = 0;
    bool terminated = false;
    int outputStore;
    IntComputer() = default;
    IntComputer(std::vector<int> initData, IntComputer *_nextComp, bool last, int _phase, std::string _name)
    {
        oper = initData;
        nextComp = _nextComp;
        isLast = last;
        phase = _phase;
        name = _name;
    }
    void reset(std::vector<int> initData)
    {
        oper = initData;
    }
    void terminate()
    {
        std::cout << "termination" << std::endl;
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

            switch (mode)
            {
            case '0':
                returnVal = data[value];
                //std::cout << "opcode: " << opcode << " parameterNumber " << parameterNumber << " value: " << returnVal << " mode: " << mode << std::endl;
                break;
            case '1':
                returnVal = value;
                //std::cout << "opcode: " << opcode << " parameterNumber " << parameterNumber << " value: " << returnVal << " mode: " << mode << std::endl;
                break;
            default:
                returnVal = -1;
                break;
            }
            //std::cout << "opcode: " << opcode << " parameterNumber " << parameterNumber << " value: " << returnVal << " mode: " << mode << std::endl;
        }
        else
        {
            return data[value];
            
        }

        return returnVal;
    }

    void calculateData(int input)
    {

        //  int inputStore = 5;
       // int i = 0;
        int stepForward = 4;
    //    std::cout<<"resuming iteration from istruction pointer: "<<i<<std::endl;
        //for (int i = 0; i <= oper.size(); i = i + stepForward)

        while (i < oper.size())
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
                //   std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
                oper[oper[i + 3]] = getValueBased(oper[i], 1, oper[i + 1], oper) + getValueBased(oper[i], 2, oper[i + 2], oper);
                i += 4;
                break;
            case '2':
               // oper[oper[i + 3]] = oper[oper[i + 1]] * oper[oper[i + 2]];
            //    std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
                oper[oper[i + 3]] = getValueBased(oper[i], 1, oper[i + 1], oper) * getValueBased(oper[i], 2, oper[i + 2], oper);
                i += 4;
                break;
            case '3':
                 //  std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << std::endl;
                //   std::cout << "load input: " << inputStore << " to position: " << oper[i + 1] << std::endl;
                if (!firstInputRequested)
                {
                //    std::cout << "get input: "<<phase<< std::endl;
                    oper[oper[i + 1]] = phase;
                    firstInputRequested = true;
                }
                else
                {
                 //   std::cout << "get input: "<<input<< std::endl;
                    oper[oper[i + 1]] = input;
                }

                i += 2;
                break;
            case '4':
               //   std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << std::endl;

                outputStore = getValueBased(oper[i], 1, oper[i + 1], oper); //oper[oper[i + 1]];
                                                                            //  std::cout << "save output: " << outputStore << " from: " << oper[i + 1] << std::endl;
                                                                            i += 2;
             //   std::cout<<"send output:"<<outputStore<<" to: "<<nextComp->name<<std::endl;
                nextComp->calculateData(outputStore);
                
                break;
            case '5':
            //      std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
                //   std::cout<< "----> !=0: "<<getValueBased(oper[i], 1, oper[i + 1], oper)<<std::endl;
                if (getValueBased(oper[i], 1, oper[i + 1], oper) != 0)
                {
                    i = getValueBased(oper[i], 2, oper[i + 2], oper);
                    //  std::cout<<"set pointer to: "<<i<<std::endl;
                }
                else
                { //std::cout<<"do nothing"<<std::endl;
                    i += 3;
                };
                break;
            case '6':
           //      std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
                //  std::cout<< "----> ==0: "<<getValueBased(oper[i], 1, oper[i + 1], oper)<<std::endl;
                if (getValueBased(oper[i], 1, oper[i + 1], oper) == 0)
                {
                    i = getValueBased(oper[i], 2, oper[i + 2], oper);
                    //std::cout<<"set pointer to: "<<i<<std::endl;
                }
                else
                { //std::cout<<"do nothing"<<std::endl;
                    i += 3;
                };
                break;
            case '7':
             //      std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
                //   std::cout<< "----> "<<getValueBased(oper[i], 1, oper[i + 1], oper)<< "< " <<getValueBased(oper[i], 2, oper[i + 2], oper) <<std::endl;
                if (getValueBased(oper[i], 1, oper[i + 1], oper) < getValueBased(oper[i], 2, oper[i + 2], oper))
                {

                    oper[oper[i + 3]] = 1;
                    //    std::cout << "----> set: "<<oper[i + 3]<< " to 1"<<std::endl;
                }
                else
                {
                    oper[oper[i + 3]] = 0;
                    //   std::cout << "----> set: "<<oper[i + 3]<< " to 0"<<std::endl;
                };
                i += 4;
                break;
            case '8':
              //   std::cout << "opercode: " << digits << " " << oper[i + 1] << " " << oper[i + 2] << " " << oper[i + 3] << std::endl;
                //  std::cout<< "----> "<<getValueBased(oper[i], 1, oper[i + 1], oper)<< "== " <<getValueBased(oper[i], 2, oper[i + 2], oper) <<std::endl;
                if (getValueBased(oper[i], 1, oper[i + 1], oper) == getValueBased(oper[i], 2, oper[i + 2], oper))
                {
                    oper[oper[i + 3]] = 1;
                    // std::cout << "----> set: "<<oper[i + 3]<< " to 1"<<std::endl;
                }
                else
                {
                    oper[oper[i + 3]] = 0;
                    // std::cout << "----> set: "<<oper[i + 3]<< " to 0"<<std::endl;
                };
                i += 4;
                break;
            case '9':

              
                terminate = true;
               // std::cout << "termination" << std::endl;
                break;
            default:

                break;
            }
            if (terminate)
            {

                break;
            }
        }

       // std::cout << "output: " << outputStore << std::endl;
    }
};

std::vector<int> prepareData()
{
    std::vector<int> data = {3, 8, 1001, 8, 10, 8, 105, 1, 0, 0, 21, 46, 55, 68, 89, 110, 191, 272, 353, 434, 99999, 3, 9, 1002, 9, 3, 9, 1001, 9, 3, 9, 102, 4, 9, 9, 101, 4, 9, 9, 1002, 9, 5, 9, 4, 9, 99, 3, 9, 102, 3, 9, 9, 4, 9, 99, 3, 9, 1001, 9, 5, 9, 102, 4, 9, 9, 4, 9, 99, 3, 9, 1001, 9, 5, 9, 1002, 9, 2, 9, 1001, 9, 5, 9, 1002, 9, 3, 9, 4, 9, 99, 3, 9, 101, 3, 9, 9, 102, 3, 9, 9, 101, 3, 9, 9, 1002, 9, 4, 9, 4, 9, 99, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 1001, 9, 2, 9, 4, 9, 3, 9, 1001, 9, 2, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 1001, 9, 2, 9, 4, 9, 99, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 101, 1, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 99, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 101, 1, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 99, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 101, 1, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 1001, 9, 2, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 99, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 101, 1, 9, 9, 4, 9, 3, 9, 101, 2, 9, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 3, 9, 1002, 9, 2, 9, 4, 9, 3, 9, 1001, 9, 1, 9, 4, 9, 3, 9, 102, 2, 9, 9, 4, 9, 99};
    std::vector<int> helpData = {3, 26, 1001, 26, -4, 26, 3, 27, 1002, 27, 2, 27, 1, 27, 26,
                                 27, 4, 27, 1001, 28, -1, 28, 1005, 28, 6, 99, 0, 0, 5};
    return data;
}

class Combination
{
public:
    int first;
    int second;
    int third;
    int fourth;
    int fifth;

    int outputValue;
};

std::vector<Combination> generateCombinations()
{
    int greatest = 0;
    std::vector<Combination> comb;
    for (int c1 = 5; c1 < 10; c1++)
    {
        for (int c2 = 5; c2 < 10; c2++)
        {
            if (c2 == c1)
                continue;
            for (int c3 = 5; c3 < 10; c3++)
            {
                if (c3 == c2 || c3 == c1)
                    continue;
                for (int c4 = 5; c4 < 10; c4++)
                {
                    if (c4 == c3 || c4 == c2 || c4 == c1)
                        continue;
                    for (int c5 = 5; c5 < 10; c5++)
                    {
                        if (c5 == c4 || c5 == c3 || c5 == c2 || c5 == c1)
                            continue;
                        std::cout << c1 << " " << c2 << " " << c3 << " " << c4 << " " << c5 << std::endl;
                        /* int a = calculateData(prepareData(), c1, 0);
                        int b = calculateData(prepareData(), c2, a);
                        int c = calculateData(prepareData(), c3, b);
                        int d = calculateData(prepareData(), c4, c);
                        int e = calculateData(prepareData(), c5, d);
                        if (e >= greatest)
                        {
                            greatest = e;
                        }*/
                        IntComputer A;
                        IntComputer B;
                        IntComputer C;
                        IntComputer D;
                        IntComputer E;

                        A = IntComputer(prepareData(), &B, false, c1, "A");
                        B = IntComputer(prepareData(), &C, false, c2, "B");
                        C = IntComputer(prepareData(), &D, false, c3, "C");
                        D = IntComputer(prepareData(), &E, false, c4, "D");
                        E = IntComputer(prepareData(), &A, true, c5, "E");

                        A.calculateData(0);
                        if (E.outputStore >= greatest)
                        {
                            greatest = E.outputStore;
                        }
                    }
                }
            }
        }
    }
    std::cout << "greatest: " << greatest;
    return comb;
}

int main()
{
    // adventCodeDay2_2();
    // adventCodeDay3_2();
    //calculateData(prepareData()),1,1;
    //generateCombinations();

   /* IntComputer A;
    IntComputer B;
    IntComputer C;
    IntComputer D;
    IntComputer E;
    A = IntComputer(prepareData(), &B, false, 9, "A");
    B = IntComputer(prepareData(), &C, false, 8, "B");
    C = IntComputer(prepareData(), &D, false, 7, "C");
    D = IntComputer(prepareData(), &E, false, 6, "D");
    E = IntComputer(prepareData(), &A, true, 5, "E");

    A.calculateData(0);
    std::cout<<"E output: "<<E.outputStore<<std::endl;*/
    generateCombinations();
}
