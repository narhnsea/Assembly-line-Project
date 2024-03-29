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
#ifndef LINEMANAGER_H_
#define LINEMANAGER_H_
#include "Workstation.h"
#include <fstream>
#include <map>
#include <iterator> // for std::advance()
#include <utility> // for std::pair<>

namespace sdds{

class LineManager
{
private:
    std::vector<Workstation*>& m_sourceData; //

    std::vector<Workstation*> m_activeLine; // – the collection of workstations for the current assembly line.
    std::size_t m_cntCustomerOrder; // – the total number of `CustomerOrder objects
    Workstation* m_firstStation; // - points to the first active station on the current line
    std::size_t m_cntWorkstation; // – the total number of `Workstation objects

    void resetLineManager();
    std::ptrdiff_t findIndex(Workstation* station);
    void updateSourceData();
public:
    LineManager() = delete; // default constructor
    LineManager(const std::string& file, std::vector<Workstation*>& stations); // constructor 2
    void reorderStations(); //reorder the Workshops in vector m_activeLine
    bool run(std::ostream& os);
    void display(std::ostream& os) const;

    ~LineManager();
};// end class LineManager

} // end namespace sdds

#endif // LINEMANAGER_H_
