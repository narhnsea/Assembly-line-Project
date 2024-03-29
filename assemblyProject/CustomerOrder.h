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
#ifndef CUSTOMERORDER_H_
#define CUSTOMERORDER_H_
#include <string>
#include "Utilities.h"
#include <vector>
#include "Station.h"

namespace sdds{

class CustomerOrder
{
private:
    struct Item{
        std::string m_itemName;
        std::size_t m_serialNumber{0};
        bool m_isFilled{false};

        Item(const std::string& src) : m_itemName(src) {};
    }; // end struct Item

    std::string m_name; // the name of the customer (e.g., John, Sara, etc)
    std::string m_product; // the name of the product being assembled (e.g., Desktop, Laptop, etc)
    std::size_t m_cntItem; // a count of the number of items in this customer's order
    Item** m_lstItem; // a dynamically allocated array of pointers. Each element of the array points to a dynamically allocated object of type `Item`
    static size_t m_widthField; // the maximum width of a field, used for display purposes

    void deallocate();
    void resetOrder();
public:
    CustomerOrder(); //default constructor
    CustomerOrder(const std::string& order); // constructor 4
    CustomerOrder(const CustomerOrder& obj); // copy constructor
    CustomerOrder(CustomerOrder&& obj) noexcept; //move constructor

    CustomerOrder& operator=(const CustomerOrder& obj); // copy assignment operator
    CustomerOrder& operator=(CustomerOrder&& obj) noexcept; // move assignment operator
    bool isOrderFilled() const;
    bool isItemFilled(const std::string& itemName) const;
    void fillItem(Station& station, std::ostream& os);
    void display(std::ostream& os) const;

    std::size_t getCntItem() const { return m_cntItem; }
    std::string getCustomerName() const { return m_name; }

    ~CustomerOrder(); //destructor
};// end class CustomerOrder

} // end namespace sdds


#endif // CUSTOMERORDER_H_
