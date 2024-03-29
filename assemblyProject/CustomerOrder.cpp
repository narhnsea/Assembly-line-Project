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
#include "CustomerOrder.h"

namespace sdds {

    // initialize static members firstly
    std::size_t CustomerOrder::m_widthField = 0;

    void CustomerOrder::deallocate()
    {
        for (std::size_t i = 0; i < m_cntItem; ++i) {
            delete m_lstItem[i];
        }// end for

        delete[] m_lstItem;
    }// end deallocate()

    void CustomerOrder::resetOrder()
    {
        m_name = std::string("");
        m_product = std::string("");
        m_cntItem = 0U;
        m_lstItem = nullptr;
    }// end resetOrder()

    CustomerOrder::CustomerOrder()
    {
        resetOrder();
    }//default constructor

    CustomerOrder::CustomerOrder(const std::string& order)
    {
        resetOrder();

        Utilities util;
        std::size_t pos = 0;
        bool more = true;

        std::vector<std::string> vec;

        int i = 1;
        do {
            switch (i)
            {
            case 1:
                m_name = util.extractToken(order, pos, more);
                break;
            case 2:
                m_product = util.extractToken(order, pos, more);
                break;
            case 3:
                while (more) {
                    vec.push_back(util.extractToken(order, pos, more));
                } // end while
                break;
            default:
                more = false; // execution should not get here
            }//end switch

            i++;
        } while (more); // end do_while

        //update m_widthField static field
        if (m_widthField < util.getFieldWidth()) {
            m_widthField = util.getFieldWidth();
        }// end if

        m_cntItem = vec.size();
        m_lstItem = new Item * [m_cntItem];
        for (std::size_t i = 0; i < m_cntItem; ++i) {
            m_lstItem[i] = new Item(vec[i]);
            m_lstItem[i]->m_serialNumber = 0;
            m_lstItem[i]->m_isFilled = false;
        }// end for
    } // constructor 4

    CustomerOrder::CustomerOrder(const CustomerOrder& obj)
    {
        //this->deallocate(); //free up m_lstItem and its contents

        //assign members
        this->m_name = obj.m_name;
        this->m_product = obj.m_product;
        this->m_cntItem = obj.m_cntItem;

        m_lstItem = new Item * [obj.m_cntItem];
        for (std::size_t i = 0; i < obj.m_cntItem; ++i) {
            m_lstItem[i] = new Item(*(obj.m_lstItem[i]));
        }// end for
    }//copy constructor

    CustomerOrder& CustomerOrder::operator=(const CustomerOrder& obj)
    {
        if (this == &obj) {
            return *this;
        }// end if

        this->deallocate(); //free up m_lstItem and its contents

        //assign members
        this->m_name = obj.m_name;
        this->m_product = obj.m_product;
        this->m_cntItem = obj.m_cntItem;

        m_lstItem = new Item * [obj.m_cntItem];
        for (std::size_t i = 0; i < obj.m_cntItem; ++i) {
            m_lstItem[i] = new Item(*(obj.m_lstItem[i]));
        }// end for

        return *this;
    }// copy assignment operator

    CustomerOrder::CustomerOrder(CustomerOrder&& obj) noexcept
    {
        this->m_lstItem = obj.m_lstItem; // steal address

        //assign members
        this->m_name = obj.m_name;
        this->m_product = obj.m_product;
        this->m_cntItem = obj.m_cntItem;

        obj.resetOrder(); //then invalidate members
    }//move constructor

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& obj) noexcept
    {
        if (this == &obj) {
            return *this;
        }// end if

        this->deallocate(); //free up m_lstItem and its contents

        this->m_lstItem = obj.m_lstItem; // steal address

        //assign members
        this->m_name = obj.m_name;
        this->m_product = obj.m_product;
        this->m_cntItem = obj.m_cntItem;

        obj.resetOrder(); //invalidate members

        return *this;
    } // move assignment operator

    bool CustomerOrder::isOrderFilled() const
    {
        bool agg = true;

        for (std::size_t i = 0; agg && i < m_cntItem; ++i) {
            agg = agg && (m_lstItem[i]->m_isFilled == true);
        }// end for

        return agg;
    }// end isOrderFilled()

    bool CustomerOrder::isItemFilled(const std::string& itemName) const
    {
        bool agg = true;
        //int count = 0;

        for (std::size_t i = 0; agg && i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == itemName) {
                agg = agg && (m_lstItem[i]->m_isFilled == true);
                //count++;
            }// end if
        }// end for

        //return ( count == 0 ? true : agg);
        return agg;
    }// isItemFilled()

    void CustomerOrder::fillItem(Station& station, std::ostream& os)
    {
        for (std::size_t i = 0; i < m_cntItem; ++i) {
            if ((m_lstItem[i]->m_itemName == station.getItemName()) && !(m_lstItem[i]->m_isFilled)) {// if
                if (station.getQuantity() > 0U) {// if_else
                    m_lstItem[i]->m_isFilled = true;
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    station.updateQuantity(); // decrement inventory

                    os << "    Filled ";
                }
                else {
                    os << "    Unable to fill ";
                }// end if_else
                os << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                return; // b/c we have to fill just one item
            }// end if
        }// end for
    }// end fillItem()

    void CustomerOrder::display(std::ostream& os) const
    {
        if (m_name == std::string("")) {
            return;
        }// end if

        os << m_name << " - " << m_product << std::endl;
        for (std::size_t i = 0; i < m_cntItem; ++i) {
            os << "[" << std::setw(6) << std::setfill('0') << std::right << m_lstItem[i]->m_serialNumber << "] "
                << std::setw(m_widthField) << std::setfill(' ') << std::left << m_lstItem[i]->m_itemName << "   - "
                << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED");
            os << std::endl;
        }// end for
    }// end display()

    CustomerOrder::~CustomerOrder()
    {
        deallocate();
    }//destructor

} // end namespace sdds
