//
// Created by brahan on 01/01/2020.
//
#include <iostream>
#include <cmath>
#include <boost/tokenizer.hpp>
#include "Fractal.h"

/**
 * abstract function, defines the "building brick" of the current fractal
 * @return the same type of fractal, but one dimension smaller
 */
Fractal::Fractal(int dim) :
        _curDim(dim)
{

}

/**
 * getter for the fractal
 * @return the fractal dim
 */
int Fractal::getCurDim() const
{
    return _curDim;
}

/**
 * getter for the fractal
 * @return the template
 */
const vector<std::string> &Fractal::getTemplate() const
{
    return _template;
}

/**
 * getter for the fractal
 * @return the fractal
 */
const vector<std::string> &Fractal::getFractal() const
{
    return _fractal;
}

/**
 * draws the fractal
 */
void Fractal::draw()
{
    for (const auto &cell : this->_fractal)
    {
        std::cout << cell;
        std::cout << std::endl;
    }
}

/**
 * builds the fractal
 */
void Fractal::buildFractal()
{
    if (getCurDim() == MIN_DIM)
    {
        setFractal(getTemplate());
        return;
    }
    Fractal *base = baseForm();
    size_t size = pow(getTemplate().size(), getCurDim() - 1);
    for (int j = 0; j < (int) getTemplate().size(); ++j)
    {
        _rowTemplate(base, size, j);
    }
    delete (base);
}

/**
 * setter for the fractal
 * @param fractal the new fractal
 */
void Fractal::setFractal(const vector<std::string> &fractal)
{
    _fractal = fractal;
}

/**
 * helper function for the build fractal function, build fractal row
 * @param baseForm the "building bricks" of the current fractal
 * @param size the fractal size
 * @param fractalLineNum which fractal line we building
 */
void Fractal::_rowTemplate(Fractal *baseForm, int size, int fractalLineNum)
{
    std::string fractalRow;
    std::string spaceFactor = _buildSpaceFactor(size);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < (int) getTemplate().size(); ++j)
        {
            if (getTemplate()[fractalLineNum][j] == BASE_DRAWING)//if we have # we need to put baseform instead
            {
                fractalRow += baseForm->getFractal().at(i);
            }
            else // else we have space, so we need to put the spaces according the the fractal dim
            {
                fractalRow += spaceFactor;
            }
        }
        this->_fractal.push_back(fractalRow);
        fractalRow = "";
    }
}

std::string Fractal::_buildSpaceFactor(int size)
{
    std::string spaceFactor;
    for (int i = 0; i < (int) size; ++i)
    {
        spaceFactor += SPACE_DRAWING;
    }
    return spaceFactor;
}

/**
 * constructor for the class
 * @param dim the fractal dim
 */
SierpinskiCarpet::SierpinskiCarpet(int dim) : Fractal(dim)
{
    this->_template = {"###", "# #", "###"};
    (dim == MIN_DIM) ? setFractal(getTemplate()) : buildFractal();
}

/**
 * constructor for the class
 * @param dim the fractal dim
 */
SierpinskiTriangle::SierpinskiTriangle(int dim) : Fractal(dim)
{
    this->_template = {"##", "# "};
    (dim == MIN_DIM) ? setFractal(getTemplate()) : buildFractal();
}

/**
 * constructor for the class
 * @param dim the fractal dim
 */
VicsekFractal::VicsekFractal(int dim) : Fractal(dim)
{
    this->_template = {"# #", " # ", "# #"};
    (dim == MIN_DIM) ? setFractal(getTemplate()) : buildFractal();
}

/**
 * the fractal base form
 * @return the fractal with one dimension less
 */
Fractal *SierpinskiCarpet::baseForm()
{
    return new SierpinskiCarpet(getCurDim() - 1);
}

/**
 * the fractal base form
 * @return the fractal with one dimension less
 */
Fractal *SierpinskiTriangle::baseForm()
{
    return new SierpinskiTriangle(getCurDim() - 1);
}

/**
 * the fractal base form
 * @return the fractal with one dimension less
 */
Fractal *VicsekFractal::baseForm()
{
    return new VicsekFractal(getCurDim() - 1);
}

/**
 * static factory function
 * @param fractalType the fractal type
 * @param dim the fractal dimension
 * @return a new fractal
 */
Fractal *FractalFactory::fractalFactory(int fractalType, int dim)
{
    switch (fractalType)
    {
        case CARPET:
            return new SierpinskiCarpet(dim);
        case TRIANGLE:
            return new SierpinskiTriangle(dim);
        case VICSEK:
            return new VicsekFractal(dim);
        default:
            return nullptr;
    }
}

/**
 * validate the data
 * @param data the data from the file
 * @return 1 if the data is not valid 0 otherwise
 */
int FractalFactory::validateData(const std::vector<std::vector<std::string> > &data)
{
    for (auto &i : data)
    {
        int type = stoi(i.at(TYPE_COL));
        int dim = stoi(i.at(DIM_COL));
        if (type < CARPET || type > VICSEK || dim < MIN_DIM || dim > MAX_DIM)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * helper method that validate the line in the file
 * @param line a single file line
 * @param p the line regex
 * @return 1 if there is error, 0 else
 */
int FileHandler::_validateLine(const std::string &line, const std::regex &p)
{
    if (line.empty())
    {
        return EXIT_FAILURE;
    }
    if (!regex_match(line, p))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * parse the file data
 * @param path the file path
 * @param isValid indicator if the file is valid
 * @return vector with the file data if everything ok
 */
std::vector<std::vector<std::string> > FileHandler::parseFile(const std::string &path, int &isValid)
{
    std::ifstream file(path);
    std::regex p(LINE_PATTERN);
    if (path.compare(path.size() - FILE_SUFF_LEN, FILE_SUFF_LEN, CSV_FILE))
    {
        isValid = EXIT_FAILURE;
    }
    if (file.bad() || file.fail())
    {
        file.close();
        isValid = EXIT_FAILURE;
    }
    boost::char_separator<char> separator{COMMA};
    std::vector<std::vector<std::string> > dataList;
    std::string line;
    while (getline(file, line))
    {
        if (_validateLine(line, p))
        {
            file.close();
            isValid = EXIT_FAILURE;
            break;
        }
        boost::tokenizer<boost::char_separator<char>> pattern{line, separator};
        std::vector<std::string> parsedLine;
        for (const auto &t:pattern)
        {
            parsedLine.push_back(t);
        }
        dataList.push_back(parsedLine);
    }
    file.close();
    return dataList;
}