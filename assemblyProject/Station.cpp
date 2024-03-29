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
#include "Station.h"

namespace sdds{

// initialize static members firstly
std::size_t Station::m_widthField = 0;
int Station::id_generator = 0;

void Station::resetStation()
{
    m_name = std::string("");
    m_description = std::string("");
    m_next_serial_number = 0U;
    m_num_items = 0U;
}// end resetStation()

Station::Station()
{
    m_id = ++id_generator;
    resetStation();
} //default constructor

Station::Station(const std::string& station)
{
    m_id = ++id_generator;
    resetStation();

    Utilities util;
    std::size_t pos = 0;
    bool more = true;

    int i = 1;
    do{
        switch(i)
        {
        case 1:
            m_name = util.extractToken(station, pos, more);
            break;
        case 2:
            m_next_serial_number = std::stoull(util.extractToken(station, pos, more));
            break;
        case 3:
            m_num_items = std::stoull(util.extractToken(station, pos, more));
            break;
        case 4:
            m_description = util.extractToken(station, pos, more);
            break;
        default:
            more = false; // execution should not get here
        }//end switch

        i++;
    }while(more); // end do_while

    //update m_widthField static field
    //if(m_widthField < util.getFieldWidth()){
        m_widthField = util.getFieldWidth(); //update m_widthField static field
    //}// end if
} // constructor 2

void Station::display(std::ostream& os, bool full) const
{
    if(m_name == std::string("")){
        return;
    }// end if_1

    os << std::setw(3) << std::setfill('0') << std::right << m_id << " | "
        << std::setw(m_widthField) << std::setfill(' ') << std::left << m_name << "| "
        << std::setw(6) << std::setfill('0') << std::right << m_next_serial_number << " | ";

    if(!full){
        os << std::endl;
        return;
    }// end if_2

    os << std::setw(4) << std::setfill(' ') << std::right << m_num_items << " | "
        << m_description;
        os << std::endl;
}// end display()

Station::~Station()
{

} //destructor

} // end namespace sdds
