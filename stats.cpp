#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

bool readFromFile(std::string fileName, std::vector<int>& dailyCases, std::vector<float>& serial)
{
    std::string line;
    std::vector<std::string> textData;
    fileName += ".txt";
    std::ifstream file(fileName.c_str());
    if(file.is_open())
    {
        while(getline(file, line))
        {
            textData.push_back(line);
            static bool next = false;
            // get rid of any empty spaces on the back
           
            // ignore first line
            if(line == "serial")
            {
                continue;
            }

            while(textData.back().back() == ' ')
            {
                textData.back().pop_back();
            }
            while(textData.back().front() == ' ')
            {
                textData.back().erase(0,1);
            }

            // ignore commented lines
            if(line[0] == '/' && line[1] == '/')
            {
                textData.pop_back();
                continue;
            }

            if (line == "cases")
            {
                // stop reading serial data
                next = true;
                continue;
            }

            if(line.empty())
            {
                textData.back().pop_back();
                continue;
            }
           
            if(!next)
            {
                // read serial data
                serial.push_back(std::strtof(textData.back().c_str(), nullptr));
            }
            else
            {
                dailyCases.push_back(static_cast<int>(std::strtol(textData.back().c_str(), nullptr, 10)));
            }
        }
        file.close();
        return true;
    }
    return false;
}

void calculate(const std::vector<int>& dailyCases, const std::vector<float>& serial)
{
    // find out how far back we need to care about old infections
    int serialLength = static_cast<int>(serial.size());
    // get the value of the last case, this will be used for refining R
    int lastCases = dailyCases.back();
    // get the number of the next day we will predict
    int t = static_cast<int>(dailyCases.size() + 1);

    // number of new cases based on our previous days cases (this will accumulate)
    float newCases = 0;

    // iterate backwards for the multiplication of the serial data into the equation & multiply r into the equation
    for(int i = 1; i < serialLength + 1; i++)
    {
        newCases += static_cast<float>(dailyCases.at(dailyCases.size() - i)) * serial.at(i - 1);
    }

    float R = static_cast<float>(lastCases) / newCases;

    newCases *= R;

    std::cout << "Predicted cases for day " << t << " with R = " << R << ":\n" << newCases << std::endl;
}

int main()
{
    std::vector<int> dailyCases;
    std::vector<float> serial;

    top:
    std::cout << "\nInput the exact name of the file without the .txt extension:\t";
   
    std::string fileName;
    std::cin >> fileName;

    if(!readFromFile(fileName, dailyCases, serial))
    {
        std::cout << "Invalid file! Try again.\n\n";
        goto top;
    }
   
    std::cout << "\nCalculating...\n";
    calculate(dailyCases, serial);

std::cout << "\n\n\n\nType anything to close\n";
char x;
std::cin >> x;
    return 0;
}
