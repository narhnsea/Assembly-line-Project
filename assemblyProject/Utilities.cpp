/******************************************************************************
                          Milestone 3
Full Name  : Nancy Wobo
Student ID#: 118344209
Email      : nuwobo@myseneca.ca
Section    : ZBB

Authenticity Declaration:
I declare this submission is the result of my own work and has not been
shared with any other student or 3rd party content provider. This submitted
piece of work is entirely of my own creation.
******************************************************************************/
#include "Utilities.h"

namespace sdds{

char Utilities::m_delimiter = '\0'; // initialize static member firstly

Utilities::Utilities() : m_widthField{1U}
{

}//default constructor

std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more)
{
    std::string t;

    std::string::size_type idx = str.find(m_delimiter, next_pos);
    if(idx != std::string::npos){
        if(idx == next_pos){ // true if a delimiter xter is found at index next_pos
            //throw std::runtime_error("next_pos is a position of a delimiter character");
            more = false;
            //next_pos++;
            throw std::exception();
            return std::string("");
        }// end if_2

        t = trim( str.substr(next_pos, (idx - next_pos)) );
        next_pos = idx + 1; // update the next index of start next find
        more = true;
    }else{
        more = false;
        t = trim( str.substr(next_pos) ); // capture the rest of the string
    }// end if_else

    if(m_widthField < t.size()){
        //std::cout << "m_widthField and t.size()" << m_widthField << " and " << t.size() << std::endl;
        m_widthField = t.size();
        //setFieldWidth(t.size());
    }// end if

    return t;
}// end extractToken()

Utilities::~Utilities()
{

}//destructor

//non_member function
std::string trim(const std::string& text)
{
    std::string::size_type first_idx = text.find_first_not_of(' ');
    std::string::size_type last_idx = text.find_last_not_of(' ');
    return text.substr( first_idx, (last_idx - first_idx + 1) );
}// end trim()

} // end namespace sdds
