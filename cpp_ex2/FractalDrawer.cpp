/**
 * @file FractalDrawer.c
 * @author  Brahan Wassan <brahan>
 * @version 1.0
 * @date 8 Jan 2020
 *
 * @brief Program that build a tree from a given txt file
 *
 * @section DESCRIPTION
 * The program draws fractals from csv file
 * Input  : csv file with integer that represent the fractal type and size
 * program build explanation: i created abstract class - Fractal that  defines the basic fractal features
 * there are 3 type of fractals, all of them are subclass of Fractal, and they override the method buildFractal
 * in addition to those 4 classes, to match the single resoponsibilty prinsible i created a factory class which
 * is responsible on creating the fractals, and file handler class which handles the file validation and parsing.
 */
#include <iostream>
#include "Fractal.h"

#define USAGE "Usage: FractalDrawer <file path>"
#define NUM_ARGS 2
#define FILE_IDX 1

/**
 * the function draws all the fractals
 * @param data the data from the file
 */
void drawAll(std::vector<std::vector<std::string> > data)
{
    for (int j = (int) data.size() - 1; j >= 0; --j)
    {
        Fractal *fractal = FractalFactory::fractalFactory(stoi(data.at(j).at(TYPE_COL)), stoi(data.at(j).at(DIM_COL)));
        fractal->draw();
        std::cout << std::endl;
        delete (fractal);
    }
}

/**
 * main function
 * @param argc num of arg
 * @param argv program args
 * @return 1 if error, 0 if not
 */
int main(int argc, const char *argv[])
{
    if (argc != NUM_ARGS)
    {
        std::cerr << USAGE << std::endl;
        return EXIT_FAILURE;
    }
    int isValid = 0;
    std::vector<std::vector<std::string> > data = FileHandler::parseFile(argv[FILE_IDX], isValid);
    if (isValid)
    {
        std::cerr << INVALID << std::endl;
        return EXIT_FAILURE;
    }
    isValid = FractalFactory::validateData(data);
    if (isValid)
    {
        std::cerr << INVALID << std::endl;
        return EXIT_FAILURE;
    }
    drawAll(data);
    return EXIT_SUCCESS;
}