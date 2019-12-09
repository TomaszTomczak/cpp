

// Example program
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include <sstream>

int getNumberOfChar(std::string layer, char character)
{
    int numberOf0 = 0;
    for (int x = 0; x < layer.length(); x++)
    {
        if (layer.at(x) == character)
        {
            numberOf0++;
        }
    }
    return numberOf0;
}

int main()
{

    std::string line;
    std::ifstream myfile("image.txt");
    if (myfile.is_open())
    {
        getline(myfile, line);

        myfile.close();
    }

    int imageSize = 25 * 6;

    int few0MinAddress;

    int minimumNumberOf0;
    bool firstLoop = true;
    int onesxTwos;
    std::string helpString = std::string( 150, 'X');

    for (int i = 0; i < line.length(); i = i + imageSize)
    {

        std::string layer = line.substr(i, 150);
        //  std::cout<<"range: "<<i<<" "<<_max<<std::endl;
        std::cout << layer.length() << std::endl
                  << std::endl;

        for (int xx = 0; xx < layer.length(); xx++)
        {
            if (layer.at(xx) == '1')
            {
                if (helpString.at(xx) != '0')
                {
                    helpString.at(xx) = '1';
                }
            }
            else
            {
                if (layer.at(xx) == '0')
                {
                    if (helpString.at(xx) != '1')
                    {
                        helpString.at(xx) = '0';
                    }
                }
            }
        }

        /*   int numberOf0 = getNumberOfChar(layer, '0');
        if (firstLoop)
        {
            minimumNumberOf0 = numberOf0;
            few0MinAddress = i;
            onesxTwos = getNumberOfChar(layer, '1') * getNumberOfChar(layer, '2');
            firstLoop = false;
        }
        else
        {
            if (minimumNumberOf0 > numberOf0)
            {
                minimumNumberOf0 = numberOf0;

                few0MinAddress = i;
                onesxTwos = getNumberOfChar(layer, '1') * getNumberOfChar(layer, '2');
            }
        }*/
    }

    std::cout << minimumNumberOf0 << "ones x twos: " << onesxTwos;
    std::cout<< std::endl<<helpString<<std::endl;
}
