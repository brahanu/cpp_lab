//
// Created by brahan on 01/01/2020.
//

#ifndef CPP_EX2_FRACTAL_H
#define CPP_EX2_FRACTAL_H

#include <fstream>
#include <vector>
#include <regex>
#include <boost/tokenizer.hpp>


using std::vector;
//using namespace std;
#define INVALID "Invalid input"
#define TYPE_COL 0
#define DIM_COL 1

/**
 * abstract class that represent general fractal object
 */
class Fractal
{
#define BASE_DRAWING '#'
#define SPACE_DRAWING ' '

public:

    /**
     * getter for the dimension
     * @return the fractal dimension
     */
    int getCurDim() const;

    /**
     * getter for the fractal template
     * @return the fractal template
     */
    const vector<std::string> &getTemplate() const;

    /**
     * getter for the fractal
     * @return the fractal
     */
    const vector<std::string> &getFractal() const;

    /**
     * setter for the fractal
     * @param fractal the new fractal
     */
    void setFractal(const vector<std::string> &fractal);

    /**
     * draws the fractal
     */
    void draw();

    /**
     * if we have virtual methods we need virtual destructor
     */
    virtual ~Fractal() = default;

    /**
     * default copy const
     * @param other
     */
    Fractal(Fractal &other) = default;

protected:
    /**
     * constructor for the Fractal calss
     * @param dim the fractal dimension
     */
    explicit Fractal(int dim);

    /**
     * abstract function, defines the "building brick" of the current fractal
     * @return the same type of fractal, but one dimension smaller
     */
    virtual Fractal *baseForm() = 0;

    /**
     * builds the fractal
     */
    void buildFractal();

    vector<std::string> _template;

private:
    /**
     * helper function for the build fractal function, build fractal row
     * @param baseForm the "building bricks" of the current fractal
     * @param size the fractal size
     * @param fractalLineNum which fractal line we building
     */
    void _rowTemplate(Fractal *baseForm, int size, int fractalLineNum);

    /**
     * helper function for the build fractal, build the spaces in the fractal
     * @param spaceFactor the spaces
     * @param size the fractal size
     */
    std::string _buildSpaceFactor(int size);

    int _curDim;

    vector<std::string> _fractal;
};

/**
 * class that represent SierpinskiCarpet fractal object
 */
class SierpinskiCarpet : public Fractal
{
public:
    /**
     * constructor for the class
     * @param dim the fractal dim
     */
    explicit SierpinskiCarpet(int dim);

protected:
    /**
     * the fractal base form
     * @return the fractal with one dimension less
     */
    Fractal *baseForm() override;

};

/**
 * sub class that represent SierpinskiTriangle fractal object
 */
class SierpinskiTriangle : public Fractal
{
public:
    /**
     * constructor for the class
     * @param dim the fractal dim
     */
    explicit SierpinskiTriangle(int dim);

protected:
    /**
     * the fractal base form
     * @return the fractal with one dimension less
     */
    Fractal *baseForm() override;


};

/**
 * class that represent VicsekFractal fractal object
 */
class VicsekFractal : public Fractal
{
public:
    /**
     * constructor for the class
     * @param dim the fractal dim
     */
    explicit VicsekFractal(int dim);

protected:
    /**
     * the fractal base form
     * @return the fractal with one dimension less
     */
    Fractal *baseForm() override;


};

/**
 * factory class, will be responsible on creating the fractals according to the user input
 */
class FractalFactory
{
#define CARPET 1
#define TRIANGLE 2
#define VICSEK 3
#define MAX_DIM 6
#define MIN_DIM 1

public:
    /**
     * static factory function
     * @param fractalType the fractal type
     * @param dim the fractal dimension
     * @return a new fractal
     */
    static Fractal *fractalFactory(int fractalType, int dim);

    /**
     * validate the data
     * @param data the data from the file
     * @return 1 if the data is not valid 0 otherwise
     */
    static int validateData(const vector<std::vector<std::string> > &data);

};

/**
 * will be responsible to parse the file
 */
class FileHandler
{
#define LINE_PATTERN R"(^\d+\,\d+[\r\n]?$)"
#define CSV_FILE ".csv"
#define FILE_SUFF_LEN 4
#define COMMA ","

private:
    /**
     * helper method that validate the line in the file
     * @param line a single file line
     * @param p the line regex
     * @return 1 if there is error, 0 else
     */
    static int _validateLine(const std::string &line, const std::regex &p);

public:
    /**
     * parse the file data
     * @param path the file path
     * @param isValid indicator if the file is valid
     * @return vector with the file data if everything ok
     */
    static std::vector<std::vector<std::string> > parseFile(const std::string &path, int &isValid);
};

#endif //CPP_EX2_FRACTAL_H
