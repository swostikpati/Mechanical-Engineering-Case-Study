//*******************************************************************//
//** Author: Swostik Pati                                          **//
//** Date Created: 19/10/21                                        **//
//** Assignment 2: Mechanical Engineering Case Study               **//
//** Flight Simulator Wind Speed                                   **//
//** ENGR-UH-1000 Computer Programming for Engineers, NYUAD        **//
//** Problem:                                                      **//
//** The objective of this software is to generate a               **//
//** computer simulation of wind speed                             **//
//*******************************************************************//

//Importing dependencies
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

//creating function prototypes
void loadConfiguration(double wsd[], double sd[], double bd[]);
void generateWindSimulation(double wsd[]);
void generateStormData(double wsd[], double sd[]);
void generateBurstData(double wsd[], double sd[], double bd[]);
void saveToFile(double wsd[], double sd[], double bd[]);

int main()
{
    //declaring and initializing arrays of double datatype to store data from the file
    double wsd[4] = {0}; //wsd will store wind speed parameters from the file
    double sd[5] = {0};  //sd will store storm parameters from the file
    double bd[5] = {0};  //bd will store burst parameters from the file

    //calling the functions
    loadConfiguration(wsd, sd, bd);
    generateWindSimulation(wsd);
    generateStormData(wsd, sd);
    generateBurstData(wsd, sd, bd);
    saveToFile(wsd, sd, bd);

    return 0;
}

//defining loadConfiguration function - it takes in three arrays as parameters and stores value from the file
void loadConfiguration(double wsd[], double sd[], double bd[])
{
    //opening file simulationConfiguration.txt in ifstream to load values from the file
    ifstream simData;
    simData.open("simulationConfiguration.txt", ios::in);
    //accepting exceptions in case the file fails to open
    if (simData.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    //saving the first line from simulationConfiguration.txt to the windspeed array
    for (int i = 0; i < 4; i++)
    {
        simData >> wsd[i];
    }
    //saving the second line from simulationConfiguration.txt to the storm data array
    for (int i = 0; i < 5; i++)
    {
        simData >> sd[i];
    }
    //saving the third line from simulationConfiguration.txt to the burst data array
    for (int i = 0; i < 5; i++)
    {
        simData >> bd[i];
    }
    //closing the file handler
    simData.close();
}

//defining generateWindSimulation function - it takes in the wind speed array as a parameter and generates windspeed at each time step value
void generateWindSimulation(double wsd[])
{
    //creating and opening WindSpeed.txt in ofstream to output wind speed values into the file
    ofstream windSpeed;
    windSpeed.open("WindSpeedData.txt", ios::out);
    //accepting exceptions in case the file fails to open
    if (windSpeed.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    //putting a random seed in srand function - using system time
    srand(time(0));

    //starting a loop to traverse from 0 seconds to the time duration for
    //which the storm has to be produced with uniform steps taken from the file
    for (double i = 0; i <= wsd[2]; i += wsd[3])
    {
        //creating minimum and maximum value for windspeed between
        //which random windspeed values will be generated
        double a = wsd[0] + wsd[1];
        double b = wsd[0] - wsd[1];

        //generating random value of windspeed between avg windspeed +- gust value
        double ws = ((double)rand() / RAND_MAX) * (b - a) + a;

        //printing windspeed value with corresponding time stamps
        windSpeed << i << "  " << ws << endl;
    }
    //closing the windSpeed file handler
    windSpeed.close();
}

//defining generateStormData function - it takes in the wind speed array and storm data array as parameters and generates a storm
void generateStormData(double wsd[], double sd[])
{
    //creating and opening StormData.txt in ofstream to output storm data values into the file
    ofstream storm;
    storm.open("StormData.txt", ios::out);
    //accepting exceptions in case the file fails to open
    if (storm.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    //making a while loop work as a for loop with i (double) starting from 0 to the end of time duration
    double i = 0;
    while (i <= wsd[2])
    {
        //generating a random number between 0 and 1
        double randStorm = ((double)rand() / RAND_MAX);
        //the code block gets executed when the random number is less than pStorm -> there was a storm
        if (randStorm <= sd[0])
        {
            //generating random storm amplitude between maximum and mimimum storm speed
            double stormAmp = ((double)rand() / RAND_MAX) * (sd[2] - sd[1]) + sd[1];
            //generating random storm duration between maximum and minimum storm duration
            double sTime = ((double)rand() / RAND_MAX) * (sd[4] - sd[3]) + sd[3];

            //making another while loop work as a for loop starting from 0 and going up to the end of storm duration
            double j = 0;
            double temp = i; //storing the value of i in a temporary variable temp as i keeps changing
            while (j <= sTime)
            {
                //printing in the values of storm corresponding to the time into the StormData.txt file
                storm << j + temp << "  " << stormAmp << endl;
                //incrementing both j and i by time step value of windspeed
                j += wsd[3];
                i += wsd[3];
            }
        }
        //this code block gets executed if there is no storm
        else
        {
            //since there is no storm each time value shows 0 mph storm speed
            //this is printed into the StormData.txt file
            storm << i << "  " << 0 << endl;
            //incrementing i by time step value of windspeed
            i += wsd[3];
        }
    }
    //closing the file handler for the StormData.txt file
    storm.close();
}

//defining generateBurstData function - it takes in the wind speed array, storm data array and burst data array as parameters and generates a microburst
void generateBurstData(double wsd[], double sd[], double bd[])
{
    //creating and opening BurstData.txt in ofstream to output storm data values into the file
    ofstream burst;
    //accepting exceptions in case the file fails to open
    burst.open("BurstData.txt", ios::out);
    if (burst.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    //opening StormData.txt in ifstream to extract data from the file
    ifstream storm;
    //accepting exceptions in case the file fails to open
    storm.open("StormData.txt", ios::in);
    if (storm.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    //creating two variables skip and data
    double skip, data;
    //skip takes in the first data in the line which is the time step
    storm >> skip; //skip has basically no work in the program except helping us to skip the time step value
    storm >> data; //data stores the storm speed from the file

    //creating a for loop using a while loop where i moves from 0 to the end of time duration
    double i = 0;
    while (i <= wsd[2])
    {
        //generating a random number between 0 and 1
        double randBurst = ((double)rand() / RAND_MAX);
        //the code block gets executed when the random number is less than pBurst and the storm speed is non-zero -> there was a microburst
        if (randBurst <= bd[0] && data != 0)
        {
            //generating random burst amplitude between maximum and mimimum burst speed
            double burstAmp = ((double)rand() / RAND_MAX) * (bd[2] - bd[1]) + bd[1];
            //generating random burst duration between maximum and minimum burst duration
            double bTime = ((double)rand() / RAND_MAX) * (bd[4] - bd[3]) + bd[3];

            //creating another for loop using a while loop where j moves from 0 to the end of burst duration
            double j = 0;
            double temp = i;
            //this block of code works only when j is less than the burst duration and the storm speed is non-zero
            while (j <= bTime && data != 0)
            {
                //printing in the values of burst corresponding to the time into the BurstData.txt file
                burst << j + temp << "  " << burstAmp << endl;
                //incrementing i and j by time step value of wind speed
                j += wsd[3];
                i += wsd[3];

                //again skipping the time value from the next line in the file
                storm >> skip;
                //taking the storm speed value from the next line and storing it in data
                storm >> data;
            }
        }
        //this code block gets executed if there is no burst
        else
        {
            //since there is no burst, each time value shows 0 mph burst speed
            //this is printed into the BurstData.txt file
            burst << i << "  " << 0 << endl;
            //incrementing i and j by time step value of wind speed
            i += wsd[3];

            //again skipping the time value from the next line in the file
            storm >> skip;
            //taking the storm speed value from the next line and storing it in data
            storm >> data;
        }
    }
    //closing the storm and burst file handlers
    storm.close();
    burst.close();
}

//defining saveToFile function - it takes in the wind speed array, storm data array and burst data array as parameters
//it compiles all the data into a single WindSimulation.txt file
void saveToFile(double wsd[], double sd[], double bd[])
{
    //opening windSpeed.txt in ifstream to extract data from the file
    ifstream windSpeed;
    windSpeed.open("WindSpeedData.txt", ios::in);
    //accepting exceptions in case the file fails to open
    if (windSpeed.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    //opening StormData.txt in ifstream to extract data from the file
    ifstream storm;
    storm.open("StormData.txt", ios::in);
    //accepting exceptions in case the file fails to open
    if (storm.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    //opening BurstData.txt in ifstream to extract data from the file
    ifstream burst;
    burst.open("BurstData.txt", ios::in);
    //accepting exceptions in case the file fails to open
    if (burst.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    //creating and opening WindSimulation.txt in ofstream to store winds simulation data after compiling data from all files
    ofstream windSim;
    windSim.open("WindSimulation.txt", ios::out);
    //accepting exceptions in case the file fails to open
    if (windSim.fail())
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    //creating variables wSD, sSD and bSD to store speed values of wind speed, storm speed and burst speed
    double wSD, sSD, bSD;

    //starting a for loop from 0 to end of time duration for generating wind speed with time step values
    for (double i = 0; i <= wsd[2]; i += wsd[3])
    {
        //starting a for loop to skip the timesstamp value from each files
        //and storing the windspeed, stormspeed and burst speed values in the wSD, sSD, and bSD
        for (int j = 0; j < 2; j++)
        {
            windSpeed >> wSD;
            storm >> sSD;
            burst >> bSD;
        }
        //checking if there was a storm
        if (sSD > 0)
        {
            //printing the cumulated windspeed and a binary value(1) denoting storm existence into the WindSimulation.txt
            windSim << i << "    " << (wSD + sSD + bSD) << "\t  1" << endl;
        }
        else
        {
            //printing the cumulated windspeed and a binary value(0) denoting storm non-existence into the WindSimulation.txt
            windSim << i << "    " << (wSD + sSD + bSD) << "\t  0" << endl;
        }
    }
    //terminating windSpeed, storm, burst, and windSim file handlers
    windSpeed.close();
    storm.close();
    burst.close();
    windSim.close();
}
