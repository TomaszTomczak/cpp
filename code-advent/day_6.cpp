

// Example program
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>

class Object
{
public:
    std::string name;
    std::string kid;
    Object(std::string rawData)
    {
        
        
         name = rawData.substr(0, 3);
        kid =  rawData.substr(4,6);
      //  name = rawData.substr(0, 1);
       
        //kid =  rawData.substr(2,3);
        std::cout<<name<<" "<<kid<<std::endl;

    }
};
std::vector<Object> stepsToParent(std::string nameOfChild, std::vector<Object> objects)
{
        std::vector<Object> retVec; 
        bool parentFound = true;
        std::string currentOcject = nameOfChild; // was name
        while (parentFound)
        {

            for (auto x : objects)
            {
                if (currentOcject == x.kid)
                {
                   // std::cout<<currentOcject<<" ---- ";
                    currentOcject = x.name;
                    parentFound = true;
                    retVec.push_back(x);
                    break;
                }
                else
                {
                    
                    parentFound = false;
                }
            }
        }
return retVec;

}
void day_6()
{
    std::vector<Object> objects;
    std::string line;
    std::ifstream myfile("data.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            //cout << line << '\n';
            objects.push_back(line);
        }
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
    int totalCount = 0;
    for (auto o : objects)
    {
        bool parentFound = true;
        std::string currentOcject = o.kid; // was name
        while (parentFound)
        {
            int count = 0;
            for (auto x : objects)
            {
                if (currentOcject == x.kid)
                {
                   // std::cout<<currentOcject<<" ---- ";
                    count++;
                    currentOcject = x.name;
                    parentFound = true;
                    break;
                }
                else
                {
                    
                    parentFound = false;
                }
            }
            totalCount += count;
        }

     // std::cout<<std::endl;
    }
     std::cout << "total count: " << totalCount << std::endl;
}

void day_6_2()
{
    std::vector<Object> objects;
    std::string line;
    std::ifstream myfile("data.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            //cout << line << '\n';
            objects.push_back(line);
        }
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
    
    std::vector<Object> santa = stepsToParent("SAN", objects);
    std::cout << "santa length: "<<santa.size()<<std::endl;
    std::vector<Object> mine = stepsToParent("YOU", objects);
        std::cout << "my length: "<<mine.size()<<std::endl;


    // test if the same path
  /*  for(auto s : santa)
    {
        if (s.kid == "YOU")
        {
            std::cout<<"jestes synem mikolaja"<<std::endl;
        }
        
    }
    for(auto m : mine)
    {
        if (m.kid == "SAN")
        {
            std::cout<<"mikolaja jest moim synem"<<std::endl;
        }
        
    }*/

    for(int i = 0; i<santa.size(); i++)
    {
        for(int x = 0; x<mine.size(); x++)
        {
            if(mine[x].name == santa[i].name)
            {
                std::cout<<"odleglosc: "<<x+i<<std::endl;
                break;
            }
        }
    }



}


int main()
{
    // adventCodeDay2_2();
    // adventCodeDay3_2();
    day_6_2();
}
