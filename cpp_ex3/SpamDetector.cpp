#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <boost/tokenizer.hpp>
#include <unordered_map>
#include <random>
#include "HashMap.hpp"

// input handlers:
#define USAGE "Usage: SpamDetector <database path> <message path> <threshold>"
#define NUM_ARGS 4
// input idx:
#define FILE_IDX 1
#define FIRST_COL 0
#define SECOND_COL 1
#define MSG_IDX 2
#define THRESHOLD_IDX 3
// msg outputs:
#define SPAM "SPAM"
#define NOT_SPAM "NOT_SPAM"
#define COMMA ","
#define LINE_PATTERN  R"(^((.*)\,)?\d+[\r\n]?$)"
//#define CSV_FILE ".csv" //TODO we dont need to check if its ends with csv
//#define FILE_SUFF_LEN 4
#define NON_NEGATIVE 0

/**
 * handles error msg and return values
 * @return value that indicate that the progarm has failed
 */
int errMsg()
{
    std::cerr << INVALID << std::endl;
    return EXIT_FAILURE;
}

/**
 * checks if a given string is digit
 * @param num
 * @return
 */
bool checkIfDigit(std::string num)
{
    for (int i = 0; i < (int) num.length(); ++i)
    {
        if (!std::isdigit(num[i]))
        {
            return false;
        }
    }
    return true;
}

/**
 * count how many time a substring is in the string
 * @param txt out txt
 * @param pat out pattern
 * @return the number of time the pattern is in the text
 */
int subStringCounter(const std::string &txt, const std::string &pat)
{
    int txtLen = txt.length();
    int patLen = pat.length();
    int counter = 0;
    for (int i = 0; i <= txtLen - patLen; i++)
    {
        int j;
        for (j = 0; j < patLen; j++)
        {
            if (txt[i + j] != pat[j])
            {
                break;
            }
        }
        if (j == patLen)
        {
            counter++;
            j = 0;
        }
    }
    return counter;
}

/**
 * helper method that validate the line in the file
 * @param line a single file line
 * @param p the line regex
 * @return 1 if there is error, 0 else
 */
int _validateLine(const std::string &line)
{
    if (line.empty())
    {
        return EXIT_FAILURE;
    }
    else if (subStringCounter(line, ",") != 1)
    {
        return EXIT_FAILURE;
    }
    int startIdx = line.find(',');
    std::string curNum = line.substr(startIdx + 1, line.length() - 1);
    if (curNum.empty() || !checkIfDigit(curNum))
    {
        return EXIT_FAILURE;
    }
    int number = std::stoi(curNum);
    if (number < NON_NEGATIVE)
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
std::vector<std::vector<std::string> > parseFile(const std::string &path, int &isValid)
{
    std::ifstream file(path);
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
        if (_validateLine(line) == EXIT_FAILURE)
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

/**
 * create the vectors for teh HashMap
 * @param data the parsed csv file
 * @param firstCol the keys
 * @param secondCol the values
 */
void createVectors(vector<vector<std::string>> data, vector<std::string> &firstCol, vector<int> &secondCol)
{
    for (int j = (int) data.size() - 1; j >= 0; --j)
    {
        firstCol.push_back(data.at(j).at(FIRST_COL));
        secondCol.push_back(stoi(data.at(j).at(SECOND_COL)));
    }
}

/**
 * iterate over a string and make its letters lowercase
 * @param data the data we want to change to lower case
 */
void makeLowerCase(vector<std::string> &data)
{
    for (int j = (int) data.size() - 1; j >= 0; --j)
    {
        for (int i = 0; i < (int) data.at(j).length(); ++i)
        {
            data.at(j)[i] = tolower(data.at(j)[i]);
        }
    }
}

/**
 * parses the text file
 * @param path the txt file path
 * @param isValid a flag to check if the data is valid
 * @return parsed txt file
 */
vector<std::string> parseTxt(const std::string &path, int &isValid)
{
    std::ifstream file(path);
    if (file.bad() || file.fail())
    {
        file.close();
        isValid = EXIT_FAILURE;
    }
    boost::char_separator<char> separator{COMMA};
    std::vector<std::string> dataList;
    std::string line;
    while (getline(file, line))
    {
        dataList.push_back(line);
    }
    file.close();
    return dataList;
}

/**
 * print the spam \ notspam according to the thresholdand the spamvalue
 * @param th our threshold
 * @param spamValue the spamvalue of the txt msg
 */
void checkForThreshold(int th, int spamValue)
{
    if (spamValue >= th)
    {
        std::cout << SPAM << std::endl;
    }
    else
    {
        std::cout << NOT_SPAM << std::endl;
    }
}

/**
 * check the value of all the spam words in teh txt file
 * @param database the spam words database
 * @param txtData the txt data
 * @param words the spam words data
 * @return the spam value in the txt file
 */
int countValue(const HashMap<std::string, int> &database, vector<std::string> &txtData, vector<std::string> &words)
{
    int spamValue = 0;
    int counter = 0;
    for (std::string &s:txtData)
    {
        for (std::string &word:words)
        {
            counter = subStringCounter(s, word);
            spamValue += counter * database.at(word);
        }
    }
    return spamValue;
}

/**
 * main function
 * @param argc number of arguments
 * @param argv program arguments
 * @return 1 succ,0 fail
 */
int main(int argc, const char *argv[])
{
    if (argc != NUM_ARGS)
    {
        std::cerr << USAGE << std::endl;
        return EXIT_FAILURE;
    }
    int isValid = 0;
    vector<vector<std::string> > csv = parseFile(argv[FILE_IDX], isValid);
    if (isValid)
    {
        return errMsg();
    }
    vector<std::string> words;
    vector<int> points;
    createVectors(csv, words, points);
    makeLowerCase(words);
    try
    {
        const HashMap<std::string, int> database(words, points);
        vector<std::string> txtData = parseTxt(argv[MSG_IDX], isValid);
        if (isValid)
        {
            return errMsg();
        }
        makeLowerCase(txtData);
        int spamValue = countValue(database, txtData, words);
        std::string threshold = argv[THRESHOLD_IDX];
        if (!checkIfDigit(threshold))
        {
            return errMsg();
        }
        int thres = std::stoi(threshold);
        if (thres <= NON_NEGATIVE)
        {
            return errMsg();
        }
        checkForThreshold(thres, spamValue);
        return EXIT_SUCCESS;
    }
    catch (int e)
    {
        std::cerr << INVALID << std::endl;
        return EXIT_FAILURE;
    }
}
