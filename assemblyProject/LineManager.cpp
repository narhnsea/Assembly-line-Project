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
#include "LineManager.h"


//global variables: redeclared in Workstation.h
// pointers to the next Workstations down the assembly line (not necessarily
// the adjacent one) that will fill an order in the next iteration
std::deque<sdds::Workstation*> pNext;

namespace sdds{

void LineManager::resetLineManager()
{
    //m_activeLine =
    m_cntCustomerOrder = 0U;
    m_firstStation = nullptr;
}// end resetLineManager()

/*
LineManager::LineManager()
{
    resetLineManager();
} // default constructor
*/

LineManager::LineManager(const std::string& file, std::vector<Workstation*>& stations) : m_sourceData(stations)
{
    resetLineManager();

    if (file == std::string("")) {
		//std::cout << "ERROR: No filename provided." << std::endl;
		throw std::string("ERROR: No filename provided.");
        return;
	}// end if_1

	std::ifstream fin(file);
	if (!fin){
		//std::cout << "Unable to open [" + file + "] file." << std::endl;
		throw std::string("Unable to open [") + file + "] file.";
		return;
	}// end if_2

	std::string record;
	std::map<std::string, std::string> t_map;  // this groups the records in AssemblyLine.txt in to key-value pairs

	while (fin.peek() != EOF){
		std::getline(fin, record);

		Utilities util;
        std::size_t pos = 0;
        bool more = true;

        std::string key = std::string(""); // this will always be assigned (see AssemblyLine.txt); it doesn't really need to be assigned a default value
        std::string val = std::string(""); // this will not always be assigned (see AssemblyLine.txt) thus it has a default value

        int i = 1;
        do{
            switch(i)
            {
            case 1:
                key = util.extractToken(record, pos, more);
                break;
            case 2:
                val = util.extractToken(record, pos, more);
                break;
            default:
                more = false; // execution should not get here
            }//end switch

            i++;
        }while(more); // end do_while

        t_map.insert(std::pair<const std::string, std::string>(key, val)); // insert

        //populate m_activeLine
        auto iter = std::find_if(stations.cbegin(), stations.cend(), //iter is of type: std::vector<Workstation*>::const_iterator
          [ &key ] (Workstation* elem){
                return (elem->getItemName() == key);
          } );// end fjnd_if algorithm

        if(iter != stations.cend()){
            m_activeLine.push_back( new Workstation(*(*iter)) );
        }// end if_3
	}//end while
	fin.close();

	/*
	// print t_map
    std::cout << "{" << std::endl;
	for(const auto& elem: t_map){
        std::cout << "  {" << elem.first << ": " << elem.second << "}" << std::endl;
    }// end for
    std::cout << "}" << std::endl;

	NB: with AssemblyLine.txt as argument file, elements of m_map will end up thus below.
	Here, the key strings should precede their value strings in vector m_activeLine.
	That is: the key string is < the the corresponding value string in a sorted container.
	See method reorderStations(). t_map could also have been an onordered_map<>
	t_map =
	{
	    {"Armchair": "Nighttable"},
	    {"Bed": "Dresser"},
	    {"Bookcase": ""},
	    {"Desk": "Office Chair"},
	    {"Dresser": "Armchair"},
	    {"Filing Cabinet": "Bookcase"},
	    {"Nighttable": "Desk"},
	    {"Office Chair": "Filing Cabinet"}
	}
	*/

	//
	m_cntWorkstation = m_activeLine.size(); //update member m_cntWorkstations

	//Establish links: update m_pNextStation member of each Workshop in vector m_activeLine
	auto iter = m_activeLine.begin(); // iter is of type std::vector<Workstation*>::iterator
    auto iterEnd = m_activeLine.end(); // iterEnd is of type std::vector<Workstation*>::iterator
	std::for_each(m_activeLine.begin(), m_activeLine.end(),
          [ &t_map, &iter, &iterEnd ] (Workstation* elem){
                std::string t = elem->getItemName();
                auto iter2 = t_map.find(t); // NB- iter is of type: std::map<std::string, std::string>::iterator
                t = iter2->second;

                if( t != std::string("") ){
                    auto iter3 = std::find_if(iter, iterEnd, //iter3 is of type std::vector<Workstation*>::iterator
                        [ &t ] (Workstation* elem){
                            return (elem->getItemName() == t);
                        } );// end fjnd_if algorithm
                    elem->setNextStation( *iter3 );
                }else{
                    elem->setNextStation( nullptr);
                }// end if_else
          } );// end for_each algorithm

    //populate member m_pOrders and establish heirachy in which these orders were placed in this LineManager
    if(!g_pending.empty()){
        m_cntCustomerOrder = g_pending.size(); //update m_cntCustomerOrder

        g_copies.resize(m_cntCustomerOrder);
        std::copy(g_pending.cbegin(), g_pending.cend(), // source range
                  g_copies.begin()                      // destination range beginning
                  );
    }// end if

    //Assign m_firstStation: The first Workshop is the one whose ItemName is not
    //found as ItemName of the m_pNextStation for each of the elements in m_activeLine
    if(m_activeLine.empty()){
        return;
    }// end if_3

    iter = m_activeLine.begin(); // iter is of type std::vector<Workstation*>::iterator
    iterEnd = m_activeLine.end(); // iterEnd is of type std::vector<Workstation*>::iterator
	auto iter2 = std::find_if(m_activeLine.begin(), m_activeLine.end(), //iter2 is of type std::vector<Workstation*>::iterator
          [ &iter, &iterEnd ] (Workstation* elem){//
                std::string t = elem->getItemName();

                bool b_exist = false;
                std::for_each(iter, iterEnd,
                    [ &t, &b_exist ] (Workstation* elem2){
                        Workstation* temp = elem2->getNextStation();
                        if(temp != nullptr){
                            b_exist = b_exist || (temp->getItemName() == t );
                        }// end if
                    } );// end for_each algorithm

                return !b_exist;
          } );// end find_if algorithm
    m_firstStation = *iter2;
} // constructor 2

void LineManager::reorderStations()
{
    if(m_activeLine.empty()){
        return;
    }// end if

    struct Func
    {
        const std::vector<Workstation*> al;
        std::vector<Workstation*> al_ordered;

        Func(const std::vector<Workstation*>& arg, Workstation* first) : al(arg)
        { push_back(first); }

        void push_back(Workstation* arg){ al_ordered.push_back(arg); }

        void operator()(){
            Workstation* temp = (al_ordered.back())->getNextStation();
                // elem not used here
                if(temp == nullptr) return;

                std::for_each(al.cbegin(), al.cend(),
                    [ this, temp ] (const auto& elem2){
                        if(temp == elem2){
                            (this->al_ordered).push_back(elem2);
                        }// end if
                    } );// end for_each algorithm
        }// end operator()()
    };// end struct Func

    Func func(m_activeLine, m_firstStation);

    std::size_t len = m_activeLine.size();
    for(std::size_t i=0; i<len-1; ++i){
        func();
    }

    m_activeLine = std::move(func.al_ordered);
}// end reorderStations()

bool LineManager::run(std::ostream& os)
{
    static std::size_t cnt = 0;
    os << "Line Manager Iteration: " << ++cnt << std::endl;

    std::vector<short> run_once(m_cntWorkstation, 0); // necessary to ensure no station operates more than once in a session
    std::ptrdiff_t idx;

    //1.
    std::deque<Workstation*> pPrevScheduledWks;
    while(!pNext.empty()){
        pPrevScheduledWks.push_back( std::move(pNext[0]) );
        pNext.pop_front();
    }// end while_1

    //for debug
    //for(i=0; i<pPrevScheduledWks.size(); ++i)
        //std::cout << " At station: " << pPrevScheduledWks[i]->getItemName() << " for_1" << std::endl; // for debug

    //2.
    bool doit = false;
    if(m_firstStation->isEmpty()){
        if(!g_pending.empty()){
            (*m_firstStation) += std::move(g_pending[0]); // invoke the operator+=()
            g_pending.pop_front();
            doit = true;
        }// end if
    }else{
        doit = true;
    } // end if_else

    //3.
    if(doit){
        //std::cout << " At station: " << m_firstStation->getItemName() << " m_firststation before while_2" << std::endl; // for debug
        m_firstStation->fill(os); // fill/attend to the order in its deque m_orders

        idx = findIndex(m_firstStation);
        run_once[idx]++;

        m_firstStation->attemptToMoveOrder();
    }// end if

    //4. Cater for the stations scheduled in the last iteration
    while(!pPrevScheduledWks.empty()){
        //std::cout << " At station: " << pPrevScheduledWks[0]->getItemName() << " while_2" << std::endl; // for debug
        idx = findIndex(pPrevScheduledWks[0]);

        if( !pPrevScheduledWks[0]->isEmpty() ){
            //std::cout << " At station: " << pPrevScheduledWks[0]->getItemName() << " while_2_if" << std::endl; // for debug
            if(run_once[idx] == 0){
                pPrevScheduledWks[0]->fill(os); // fill/attend to the order in its deque m_orders
                run_once[idx]++;
            }else{
                pNext.push_back( std::move(pPrevScheduledWks[0]) ); //take it back
                pPrevScheduledWks.pop_front();
                continue;
            }// end if_else

            pPrevScheduledWks[0]->attemptToMoveOrder();
        }//end if

        pPrevScheduledWks.pop_front();
    }// end while_2

    //std::cout << "Number of customer orders remaining in g_pending = " << g_pending.size() << std::endl;  // for debug

    // 5. Test for exit
    bool b_ending = ( ((g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder) && (pNext.empty()) );

    // 6. prepare data for reporting
    if(b_ending){
        os << "Line Manager Iteration: " << ++cnt << std::endl;
        sortCustomerOrderContainer(g_completed);
        sortCustomerOrderContainer(g_incomplete);
        updateSourceData();
    }// end if

    return b_ending;

    // for debug
    //if( cnt == 19) return true;
    //else return false;
}// end run()

void LineManager::display(std::ostream& os) const
{
    if(m_activeLine.empty()){
        return;
    }// end if

    for(const auto& elem: m_activeLine){
        elem->display(os);
    }// end for
}// end display()

std::ptrdiff_t LineManager::findIndex(Workstation* pStation)
{
    auto iter = std::find(m_activeLine.cbegin(), m_activeLine.cend(), pStation); //iter is of type: std::vector<Workstation*>::const_iterator
    if(iter != m_activeLine.cend())
		return std::distance(m_activeLine.cbegin(), iter);

	return -1;
}// end findIndex()

void LineManager::updateSourceData()
{
    for(const auto& elem1 : m_activeLine){
        auto iter = std::find_if(m_sourceData.begin(), m_sourceData.end(), // iter is of type: std::vector<Workstation*>::iterator
               [ elem1 ](Workstation* elem2){
                    return (elem1->getItemName() == elem2->getItemName());
                }); // end algorithm find_if()

        if(iter != m_sourceData.end()){
            *(*iter) = *elem1;
        }// end if
    }// end for
}// end updateSourceData()

LineManager::~LineManager()
{
    for(auto& elem: m_activeLine){
        elem->setNextStation(nullptr); // b/c they were assigned directly in constructor 2
        delete elem; // b/c they were allocated dynamically in constructor 2
    }// end for
} // destructor

} // end namespace sdds
