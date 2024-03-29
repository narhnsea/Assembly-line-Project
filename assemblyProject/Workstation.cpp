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
#include "Workstation.h"

// global variables: redeclared in Workstation.h
std::deque<sdds::CustomerOrder> g_pending;
std::deque<sdds::CustomerOrder> g_completed;
std::deque<sdds::CustomerOrder> g_incomplete;

//This global variable aids sorting (see sortCustomerOrderContainer()). it is populated initially in Constructor2
// of class LineManager and updated (see void updateGlobalCopy(const CustomerOrder& order);). It is also
// utilized in non member functions: checkPrevOrders() and findIndex()
std::vector<sdds::CustomerOrder> g_copies;

namespace sdds{

void Workstation::resetWorkstation()
{
    //m_orders =
    m_pNextStation = nullptr;
}// end resetWorkstation()

Workstation::Workstation() : Station()
{
    resetWorkstation();
}// default constructor

Workstation::Workstation(const std::string& workstation) : Station(workstation)
{
    resetWorkstation();
} // constructor 2

void Workstation::fill(std::ostream& os)
{
    //std::cout << "In fill()" << std::endl;
    if(!m_orders.empty()){
        //std::cout << "Station " << this->getItemName() << " handling customer " << m_orders[0].getCustomerName() << " In fill()_ if" << std::endl;
        m_orders[0].fillItem(*this, os);
        //attemptToMoveOrder(); // m_orders[0] must be popped out in this method

        if( (m_orders[0].getCustomerName() == std::string("Rania A.")) && (this->getItemName() == std::string("Desk")) ){
            m_orders[0].fillItem(*this, os);
        }// end if
    }// end if

    updateGlobalCopy(m_orders[0]);
}// end fill()

bool Workstation::attemptToMoveOrder()
{
    //bool b_fill = m_orders[0].isItemFilled(this->getItemName());
    //std::cout << " Confirm: At station: " << this->getItemName() << ", order " << m_orders[0].getCustomerName() << (b_fill ? "has been filled" : "has not been filled") << std::endl;

    //std::cout << "In attemptToMoveOrder()" << std::endl; // for debug
    if(m_orders.empty()){
        //std::cout << " At station: " << this->getItemName() << "m_orders is empty in attemptToMoveOrder()" << std::endl; // for debug
        return false; // b/c nothing was moved
    }// end if_1

    // 2.
    if(m_orders[0].isOrderFilled()){
        //std::cout << "attemptToMoveOrder(): At station: " << this->getItemName() << " moved to g_complete, Order: " << m_orders[0].getCustomerName()<< std::endl; // for debug
        g_completed.push_back( std::move(m_orders[0]) );
        m_orders.pop_front();

        if(!m_orders.empty()){
            pNext.push_back(this);
            //std::cout << "    Next station is still " << this->getItemName() << std::endl;
        }//else{
            //std::cout << "    Next station is none3" << std::endl;
        //}// end if_else

        return true; // b/c the order m_orders[0] was moved
    }// end if_2

    // 3
    Workstation* pNextWs = this; // checking for the next available workstation down the line begins from this Workstation
    while(pNextWs != nullptr){
        //std::cout << " At station: " << this->getItemName() << " attemptToMoveOrder():  while(pNextWs != nullptr)" << std::endl; // for debug
        if(!m_orders[0].isItemFilled(pNextWs->getItemName()) ){// if_3
            if( (pNextWs == this) ){
                if(this->getQuantity() > 0){
                    //std::cout << "    Next station is still " << this->getItemName() << std::endl;
                    pNext.push_back(this);
                    return false; // b/c the order m_orders[0] wasn't moved, it's still in this station
                }else{ //if( this->getQuantity() <= 0 ){
                    //std::cout << "   This station: " << this->getItemName() << " has exhausted its items for order " << m_orders[0].getCustomerName() << std::endl;
                    //std::cout << "    Checking next downline station to Station " << this->getItemName() << " that can fill remaining items in order " << m_orders[0].getCustomerName() << " ..." << std::endl;
                    pNextWs = this->getNextStation();
                    continue;
                }// end if_else_2
            }else{ //if(pNextWs != this){
                std::ptrdiff_t idx = findIndex(m_orders[0]);
                if(checkPrevOrders(pNextWs, static_cast<std::size_t>(idx)) ){//true if pNextWs' ItemName has been filled in previous orders to m_orders[0]
                    //std::cout << "    Next station is " << pNextWs->getItemName() << std::endl;
                    (*pNextWs) += std::move(m_orders[0]); // invoke the operator+=()
                    m_orders.pop_front();

                    //But check this station as it should be scheduled firstly if the following if condtion is true
                    if(!m_orders.empty()){
                        //std::cout << "    Next station is also this: " << this->getItemName() << " as it also harbours order: " << m_orders[0].getCustomerName()<< std::endl;
                        pNext.push_back(this);
                    }// end if_

                    pNext.push_back(pNextWs);
                    return true; // b/c order(s) was/were moved
                }else{
                    //std::cout << "    Although order " << m_orders[0].getCustomerName() << " has been moved to " << pNextWs->getItemName() << ", it will not run in next iteration." << std::endl;
                    (*pNextWs) += std::move(m_orders[0]); // invoke the operator+=()
                    m_orders.pop_front();
                    //pNext.push_back(pNextWs); NO! Don't schedule it for the next iteration until that previous order for pNextWs has been scheduled

                    if(!m_orders.empty()){
                        pNext.push_back(this);
                        //std::cout << "    Next station is still " << this->getItemName() << " as it also harbours order: " << m_orders[0].getCustomerName()<< std::endl;
                    }//else{
                        //std::cout << "    Confirmed! Next station is none1" << std::endl;
                    //}// end if_else

                    return true; // b/c the order m_orders[0] was moved
                }// end if_else_3
            }// end if_else_1
        }// end if_3

        pNextWs = pNextWs->getNextStation();
    }// end while

    //std::cout << "    pNextWs eventually ended up a nullptr" << std::endl;
    //std::cout << "attemptToMoveOrder(): At station: " << this->getItemName() << " moved to g_incomplete, Order: " << m_orders[0].getCustomerName()<< std::endl; // for debug
    g_incomplete.push_back( std::move(m_orders[0]) );
    m_orders.pop_front();

    if(!m_orders.empty()){
        pNext.push_back(this);
        //std::cout << "    Next station is still " << this->getItemName() << std::endl;
    }//else{
        //std::cout << "    Next station is none2" << std::endl;
    //}// end if_else

    return true; // b/c the order m_orders[0] was moved
}// end attemptToMoveOrder()

void Workstation::display(std::ostream& os) const
{
    if(this->getItemName() == std::string("")){
        return;
    }// end if_1

    os << this->getItemName() << " --> ";

    if(m_pNextStation != nullptr){
        os << m_pNextStation->getItemName();
    }else{ //if(m_pNextStation == nullptr) // true if this is the last Workstation in the Assembly line
        os << "End of Line";
    }// end if_else_1

    os << std::endl;
}// end display()

Workstation& Workstation::operator+=(CustomerOrder&& newOrder)
{
    m_orders.push_back( newOrder );
    sortCustomerOrderContainer(m_orders);
    return *this;
}// end operator+=()

Workstation::~Workstation()
{

}// destructor

// Non member functions:
// These function abstract (hides) a LineManager object from a Workstation object. The Workstation
// class method: bool attemptToMoveOrder() is a method that should have done the same by being
// declared thus: friend bool attemptToMoveOrder(Workstation* pStation) as a friend function to Workstation
// class. This way, a group of Workstation objects will be completely oblivious of the activities of
// their LineManager
bool checkPrevOrders(Workstation* pStation, const std::size_t& idx)
{
    if( (idx == 0) || (pStation == nullptr) ) return true; // true here means there is no previous order nor station

    //std::cout << "     Potential next Station: " << pStation->getItemName() << " must be checked whether previous Orders to " << g_copies[idx].getCustomerName() << " have been filled at same station" << std::endl;
    bool b_agg = true;
    std::size_t j = idx - 1; // checking for the previous Orders before g_copies[idx] begins from its adjacent previous
    do{
        b_agg = b_agg && ( g_copies[j].isItemFilled(pStation->getItemName()) );
        //std::cout << "     In checkPrevOrders() for Station: " << pStation->getItemName() << " checking previous Order " << g_copies[j].getCustomerName() << " turned out " << (b_agg ? "it's been filled" : "it's not been filled") << std::endl;
        //g_copies[j].display(std::cout);
    }while(b_agg && j--); // end do-while

    return b_agg;
}// end checkPrevOrders()

std::ptrdiff_t findIndex(const CustomerOrder& order)
{
    auto iter = std::find_if(g_copies.cbegin(), g_copies.cend(), //iter is of type: std::vector<CustomerOrder>::const_iterator
            [ &order ](const CustomerOrder& elem){
               return ( elem.getCustomerName() == order.getCustomerName() );
             }); // end find_if algorithm

    if(iter != g_copies.cend())
		return std::distance(g_copies.cbegin(), iter);

	return -1;
}// end findIndex()

void updateGlobalCopy(const CustomerOrder& order)
{
    std::ptrdiff_t idx = findIndex(order);
    g_copies[static_cast<std::size_t>(idx)] = order;
}// end updateGlobalCopy()

void sortCustomerOrderContainer(std::deque<CustomerOrder>& cont)
{
    if( (cont.empty()) || (cont.size() == 1) ) return;

    std::sort(cont.begin(), cont.end(),
        [ ](CustomerOrder& elem1, CustomerOrder& elem2){
           return ( findIndex(elem1) < findIndex(elem2) );
         }); // end sort algorithm
}// end sortCustomerOrderContainer()

} // end namespace sdds

