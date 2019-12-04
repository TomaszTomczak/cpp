#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

bool checkDigits(int number)
{
    std::ostringstream os;
    os << number;
    std::string digits = os.str();
    bool result = false; 
    bool adjacentSame = false;
    for(int i=0; i<5; i++)
    {
        int current = digits.at(i) - '0';
        int next = digits.at(i+1) - '0';

        if(current <=next)  
        {
            if (current == next)
            {
                adjacentSame |= true;
            }
            
            result = true;
        }      
        else
        {
            result = false; 
            break;
        }      
        
    }

    return result && adjacentSame;
}

bool checkDigitsGroup(int number)
{
    std::ostringstream os;
    os << number;
    std::string digits = os.str();
    bool result = false; 
    bool adjacentSame = false;
    bool oneDoubleFound = false;
    int sameInGroup = 0;
    int localSame = 0;
    for(int i=0; i<5; i++)
    {
    
        int current = digits.at(i) - '0';
        int next = digits.at(i+1) - '0';

        if(current <=next)  
        {
            if (current == next)
            {
                adjacentSame |= true;
                localSame ++;
            }
            else
            {
                if(localSame == 1)
                {
                 oneDoubleFound|=true;
                 std::cout<<"local double found in digit: "<<number<<std::endl;
                }
                else
                {
                    if(localSame>1)
                    {
                    oneDoubleFound|=false;
                    std::cout<<"larger group found: "<<number<<std::endl;
                    }
                }
                
                localSame = 0;
                
            }            
            
            result = true;
        }      
        else
        {
            result = false; 
            break;
        }      
        
    }
    if(localSame == 1) // last loop result loss protection
    {
        oneDoubleFound|=true;
    }

    return result && adjacentSame && oneDoubleFound;
}

void adventCodeDay4_1()
{
    int inputLow = 165432;
    int inputHight = 707912;
    
    //std::vector<int> numbers; 
    int possiblePasswords = 0;
    for(int i = inputLow; i<= 707912; i++ ) // find increasing values
    {
        if(checkDigits(i))
        {
            possiblePasswords++;
            std::cout<<i<<std::endl;
        }

    }
    std::cout<<"possible passwords: "<<possiblePasswords<<std::endl;

}

void adventCodeDay4_2()
{
    int inputLow = 165432;
    int inputHight = 707912;
    
    //std::vector<int> numbers; 
    int possiblePasswords = 0;
    for(int i = inputLow; i<= 707912; i++ ) // find increasing values
    {
        if(checkDigitsGroup(i))
        {
            possiblePasswords++;
            std::cout<<i<<std::endl;
        }
        
    }
    std::cout<<"possible passwords: "<<possiblePasswords<<std::endl;

}

int main()
{

    //adventCodeDay4_1();
    adventCodeDay4_2();
    
}

