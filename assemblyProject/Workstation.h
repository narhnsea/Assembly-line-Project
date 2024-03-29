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
#ifndef WORKSTATION_H_
#define WORKSTATION_H_
#include "CustomerOrder.h"
#include <deque> // double ended queue
#include <algorithm>

// global variables redeclaration (originally declared in file: Workstation.cpp)
extern std::deque<sdds::CustomerOrder> g_pending;
extern std::deque<sdds::CustomerOrder> g_completed;
extern std::deque<sdds::CustomerOrder> g_incomplete;
extern std::vector<sdds::CustomerOrder> g_copies;

namespace sdds{

class Workstation : public Station
{
private:
public:
    std::deque<CustomerOrder> m_orders; // – is a double-ended-queue with `CustomerOrders` entering the back and exiting the front.  These are orders that have been placed on this station to receive service (or already received service).
    Workstation* m_pNextStation; // – a pointer to the next/adjacent `Workstation` down the assembly line. The value does not change once assigned.

    void resetWorkstation();
public:
    Workstation(); // default constructor
    Workstation(const std::string& workstation); // constructor 2

    void fill(std::ostream& os);
    bool attemptToMoveOrder();

    void setNextStation(Workstation* station) { m_pNextStation = station; }
    Workstation* getNextStation() const { return m_pNextStation; }
    void display(std::ostream& os) const;
    Workstation& operator+=(CustomerOrder&& newOrder);

    bool isEmpty() { return m_orders.empty(); }

    virtual ~Workstation(); // destructor
};// end class Workstation

bool checkPrevOrders(Workstation* pStation, const std::size_t& idx); //check whether previous orders to pOther has been filled/attended to at pStation
std::ptrdiff_t findIndex(const CustomerOrder& order);
void updateGlobalCopy(const CustomerOrder& order);
void sortCustomerOrderContainer(std::deque<CustomerOrder>& cont);

} // end namespace sdds

//global variables redeclaration (originally declared in file: LineManager.cpp
extern std::deque<sdds::Workstation*> pNext;

#endif // WORKSTATION_H_
