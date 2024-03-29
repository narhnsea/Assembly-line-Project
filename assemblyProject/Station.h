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
#ifndef STATION_H_
#define STATION_H_
#include <iomanip>
#include "Utilities.h"

namespace sdds{

class Station
{
private:
    static std::size_t m_widthField; // the maximum number of characters required to print to the screen the *item name* for any object of type `Station`
    static int id_generator; // a variable used to generate IDs for new instances of type `Station`

    int m_id; //the id of the station (integer)
    std::string m_name; // the name of the item handled by the station (string)
    std::string m_description; // the description of the station (string)
    std::size_t m_next_serial_number; //the next serial number to be assigned to an item at this station (non-negative integer)
    std::size_t m_num_items; // the number of items currently in stock (non-negative integer)

    void resetStation();
public:
    Station();  //default constructor
    Station(const std::string& station); // constructor 2
    const std::string& getItemName() const { return m_name; }
    std::size_t getNextSerialNumber() { std::size_t temp = m_next_serial_number++; return temp; }
    std::size_t getQuantity() const { return m_num_items; }
    void updateQuantity() { if(m_num_items > 0U) m_num_items--; }
    void display(std::ostream& os, bool full) const;

    virtual ~Station();  //destructor
};// end class Station

} // end namespace sdds

#endif // STATION_H_
