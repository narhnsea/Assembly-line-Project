#ifndef UTILITIES_H_
#define UTILITIES_H_
#include <string>
#include <exception>
#include <iostream>
#include <cstddef> //for std::size_t

namespace sdds{

class Utilities
{
private:
    static char m_delimiter;
    std::size_t m_widthField;

public:
    Utilities(); //default constructor
    void setFieldWidth(size_t newWidth) { m_widthField = newWidth; }
    std::size_t getFieldWidth() const { return m_widthField; }
    std::string extractToken(const std::string& str, size_t& next_pos, bool& more);
    static void setDelimiter(char newDelimiter) { m_delimiter = newDelimiter; }
    static char getDelimiter() { return m_delimiter; }

    ~Utilities(); //destructor
};// class Utilities

//non_member function
std::string trim(const std::string& text); // for removing space characters before and after string argument

} // end namespace sdds

#endif // UTILITIES_H_
