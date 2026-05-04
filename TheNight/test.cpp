#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


#include "NightManager.h"
#include "NightLinkedList.h"
#include "NightBase.h"
#include "NightComp.h"
#include "NightOver.h"
#include "NightDerived.h"
#include "NightException.h"
#include "NightStack.h"
#include "NightQueue.h"
#include "NightLocationMap.h"
#include "NightJSONLoader.h"
#include <sstream>
#include <fstream>
#include <cstdio>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>




/*
TEST_CASE("NightBase default ctor initializes fields") {
    NightBase b;   //  Cannot instantiate abstract class
    CHECK(b.getDate() == "");
    CHECK(b.getHour24() == 0);
    CHECK(b.getPhase() == DAYTIME);
}

TEST_CASE("NightBase parameterized ctor initializes fields") {
    NightBase b("2026-02-08", 23, MID_NIGHT);
    CHECK(b.getDate() == "2026-02-08");
    CHECK(b.getHour24() == 23);
    CHECK(b.getPhase() == MID_NIGHT);
}

TEST_CASE("NightBase setters/getters work") {
    NightBase b;
    b.setDate("2000-01-01");
    b.setHour24(18);
    b.setPhase(EARLY_NIGHT);
    CHECK(b.getDate() == "2000-01-01");
    CHECK(b.getHour24() == 18);
    CHECK(b.getPhase() == EARLY_NIGHT);
}
*/

#include "NightTemplate.h"
#include "NightDynamic.h"

//dynamic test 1&2
TEST_CASE("Class template works with int")
{
    DynamicArray<int> arr;

    arr.add(5);
    arr.add(10);

    CHECK(arr.getSize() == 2);
    CHECK(arr[0] == 5);
    CHECK(arr[1] == 10);
}

TEST_CASE("Class template resize and remove")
{
    DynamicArray<int> arr(2);

    arr.add(1);
    arr.add(2);
    arr.add(3);  // forces resize

    CHECK(arr.getSize() == 3);

    arr.remove(1); // remove 2

    CHECK(arr.getSize() == 2);
    CHECK(arr[0] == 1);
    CHECK(arr[1] == 3);
}

//  Linkedlist ADT directly.
TEST_CASE("NightLinkedList insertBack works on an empty list")
{
    NightLinkedList list;

    list.insertBack(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    CHECK(list.getSize() == 1);
    CHECK(list.getAt(0)->getLocation() == "Detroit");
}

TEST_CASE("NightLinkedList supports front and back insertion")
{
    NightLinkedList list;

    list.insertBack(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Back"), 2
    ));

    list.insertFront(new NightDerived(
        "2026-02-08", 18, EARLY_NIGHT,
        NightComp("Front"), 1
    ));

    CHECK(list.getSize() == 2);
    CHECK(list.getAt(0)->getLocation() == "Front");
    CHECK(list.getAt(1)->getLocation() == "Back");
}

TEST_CASE("NightLinkedList throws when deleting a node that does not exist")
{
    NightLinkedList list;

    CHECK_THROWS_AS(list.removeAt(0), NightException);

    list.insertBack(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Only"), 2
    ));

    CHECK_THROWS_AS(list.removeAt(3), NightException);
}

TEST_CASE("NightLinkedList search returns -1 when location is not found")
{
    NightLinkedList list;

    list.insertBack(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    CHECK(list.searchByLocation("Missing") == -1);
}

TEST_CASE("NightLinkedList traversal handles an empty list")
{
    NightLinkedList list;
    std::ostringstream oss;

    list.printAll(oss);

    CHECK(oss.str() == "");
}

TEST_CASE("NightLinkedList iterator traverses each node in order")
{
    NightLinkedList list;

    list.insertBack(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("A"), 1
    ));

    list.insertBack(new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("B"), 2
    ));

    NightLinkedList::Iterator it = list.begin();

    CHECK(it.hasCurrent() == true);
    CHECK(it.getData()->getLocation() == "A");

    it.next();

    CHECK(it.hasCurrent() == true);
    CHECK(it.getData()->getLocation() == "B");

    it.next();

    CHECK(it.hasCurrent() == false);
}

TEST_CASE("NightLinkedList recursive count works")
{
    NightLinkedList list;

    list.insertBack(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("A"), 1
    ));

    list.insertBack(new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("B"), 2
    ));

    CHECK(list.countRecursive() == 2);
}


// template test
TEST_CASE("Function template works with int")
{
    CHECK(maxValue(5, 10) == 10);
}

TEST_CASE("Function template works with double")
{
    CHECK(maxValue(3.5, 2.1) == 3.5);
}

// 2/22 test operator +=, -=
TEST_CASE("operator+= adds item correctly") {

    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("Vegas"), 7
    );

    CHECK(manager.getSize() == 1);
}

TEST_CASE("operator-= removes item correctly") {

    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("Vegas"), 7
    );

    manager -= 0;

    CHECK(manager.getSize() == 0);
}



// 2/22 Operator== Tests 


// Not Equal Objects
TEST_CASE("NightDerived operator== returns false when objects differ") {

    NightComp loc1("Detroit");
    NightDerived a("2026-02-08", 21, MID_NIGHT, loc1, 5);

    NightComp loc2("Chicago");  // different location
    NightDerived b("2026-02-08", 21, MID_NIGHT, loc2, 5);

    CHECK_FALSE(a == b);
}

// Equal Objects
TEST_CASE("NightDerived operator== returns true for identical objects") {

    NightComp loc1("Detroit");
    NightDerived a("2026-02-08", 21, MID_NIGHT, loc1, 5);

    NightComp loc2("Detroit");
    NightDerived b("2026-02-08", 21, MID_NIGHT, loc2, 5);

    CHECK(a == b);
}



// 2/22 Polymorphic Streaming Test (operator<<)


TEST_CASE("operator<< works polymorphically") {

    NightBase* ptr = new NightDerived(
        "2026-02-08", 3, LATE_NIGHT,
        NightComp("Moon"), 9
    );

    std::ostringstream oss;
    oss << *ptr;   // uses overloaded <<

    std::string out = oss.str();

    CHECK(out.find("Moon") != std::string::npos);
    CHECK(out.find("Object count: 9") != std::string::npos);

    delete ptr;
}


//  replacesment test  (test base through derived)

TEST_CASE("Base functionality works through derived class") {
    NightComp loc("Test");
    NightDerived d("2026-02-08", 23, MID_NIGHT, loc, 3);

    CHECK(d.getDate() == "2026-02-08");
    CHECK(d.getHour24() == 23);
    CHECK(d.getPhase() == MID_NIGHT);
}


// NightComp (UNCHANGED)

TEST_CASE("NightComp helper works (isEmpty)") {
    NightComp c;
    CHECK(c.isEmpty() == true);
    c.setLocation("Detroit");
    CHECK(c.isEmpty() == false);
    CHECK(c.getLocation() == "Detroit");
}


// NightOver (UNCHANGED)

TEST_CASE("NightOver ctor sets base + derived + composition") {
    NightComp loc("Hanoi");
    NightOver o("2026-02-08", 20, EARLY_NIGHT, loc, true);

    CHECK(o.getDate() == "2026-02-08");
    CHECK(o.getHour24() == 20);
    CHECK(o.getPhase() == EARLY_NIGHT);

    CHECK(o.getLocationInfo().getLocation() == "Hanoi");
    CHECK(o.getClearSky() == true);
}





// 2/22 operator[] Tests 


TEST_CASE("Manager operator[] returns correct item for valid index") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    CHECK(manager[0] != nullptr);
    CHECK(manager[0]->getDate() == "2026-02-08");
}
// Delete me because "manager will throw"
//TEST_CASE("Manager operator[] returns nullptr for invalid index") {

  //  NightManager manager;

    //CHECK(manager[0] == nullptr);   // empty container
    //CHECK(manager[-1] == nullptr);  // negative index
//}


//  test for manager (existing functionality)

TEST_CASE("Manager adds items and tracks size") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    manager.add(new NightOver(
        "2026-02-09", 22, MID_NIGHT,
        NightComp("NYC"), true
    ));

    CHECK(manager.getSize() == 2);
}

TEST_CASE("Manager remove deletes and shifts correctly") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    manager.add(new NightOver(
        "2026-02-09", 22, MID_NIGHT,
        NightComp("NYC"), true
    ));

    manager.remove(0);

    CHECK(manager.getSize() == 1);
}

TEST_CASE("Manager printAll works polymorphically") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 2, LATE_NIGHT,
        NightComp("Mars"), 7
    ));

    std::ostringstream oss;
    manager.printAll(oss);

    const std::string out = oss.str();

    CHECK(out.find("Mars") != std::string::npos);
    CHECK(out.find("Object count: 7") != std::string::npos);
}


// Virtual print test (polymorphism)


TEST_CASE("Derived print overrides and calls base print") {

    NightBase* ptr = new NightDerived(
        "2026-02-08", 2, LATE_NIGHT,
        NightComp("Wayne State"), 5
    );

    std::ostringstream oss;
    ptr->print(oss);   // polymorphic call

    const std::string out = oss.str();

    CHECK(out.find("Date: 2026-02-08") != std::string::npos);
    CHECK(out.find("Hour24: 2") != std::string::npos);
    CHECK(out.find("Object count: 5") != std::string::npos);

    delete ptr;   // needed
}


//  test FOR manager


// EXTRA DUPLICATE TESTS (commented out to avoid running twice)
/*
TEST_CASE("Manager adds items and tracks size") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    manager.add(new NightOver(
        "2026-02-09", 22, MID_NIGHT,
        NightComp("NYC"), true
    ));

    CHECK(manager.getSize() == 2);
}

TEST_CASE("Manager remove deletes and shifts correctly") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 2
    ));

    manager.add(new NightOver(
        "2026-02-09", 22, MID_NIGHT,
        NightComp("NYC"), true
    ));

    manager.remove(0);

    CHECK(manager.getSize() == 1);
}

TEST_CASE("Manager printAll works polymorphically") {

    NightManager manager;

    manager.add(new NightDerived(
        "2026-02-08", 2, LATE_NIGHT,
        NightComp("Mars"), 7
    ));

    std::ostringstream oss;
    manager.printAll(oss);

    const std::string out = oss.str();
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    CHECK(out.find("Mars") != std::string::npos);
    CHECK(out.find("Object count: 7") != std::string::npos);
}
*/


//3/1

TEST_CASE("NightManager throws on invalid index access")
{
    NightManager manager;

    CHECK_THROWS_AS(manager[0], NightException);
    CHECK_THROWS_AS(manager[-1], NightException);
    CHECK_THROWS_AS(manager[99], NightException);
}

TEST_CASE("NightManager throws on invalid removal")
{
    NightManager manager;

    CHECK_THROWS_AS(manager -= 0, NightException);
}

TEST_CASE("DynamicArray throws on invalid access and removal")
{
    DynamicArray<int> arr;

    CHECK_THROWS_AS(arr[0], NightException);
    CHECK_THROWS_AS(arr.remove(0), NightException);

    arr.add(10);

    CHECK_THROWS_AS(arr[5], NightException);
    CHECK_THROWS_AS(arr.remove(5), NightException);
}


// 3/8 TEST RECURSION verifies recursive counting in NightManager
TEST_CASE("NightManager recursive count works")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 2, LATE_NIGHT,
        NightComp("Mars"), 7
    );

    manager += new NightDerived(
        "2026-02-09", 3, LATE_NIGHT,
        NightComp("Moon"), 8
    );

    CHECK(manager.countRecursive() == 2);
}

// 3/22 Binary Search Test - found sortByHour, manager.binarySearchByHour()
TEST_CASE("Binary search finds correct index after sorting")
{
    NightManager manager;

    manager += new NightDerived("2026-02-08", 22, MID_NIGHT, NightComp("A"), 1);
    manager += new NightDerived("2026-02-08", 18, EARLY_NIGHT, NightComp("B"), 1);
    manager += new NightDerived("2026-02-08", 20, MID_NIGHT, NightComp("C"), 1);

    manager.sortByHour();   // REQUIRED before binary search

    int index = manager.binarySearchByHour(20);

    CHECK(index != -1);
    CHECK(manager[index]->getHour24() == 20);
}

// 3/20 Binary Search Test - not found sortByHour, manager.binarySearchByHour()
TEST_CASE("Binary search returns -1 when value not found")
{
    NightManager manager;

    manager += new NightDerived("2026-02-08", 10, EARLY_NIGHT, NightComp("A"), 1);
    manager += new NightDerived("2026-02-08", 12, MID_NIGHT, NightComp("B"), 1);

    manager.sortByHour();

    int index = manager.binarySearchByHour(25);

    CHECK(index == -1);
}


// Stack Tests

TEST_CASE("NightStack push, pop, peek, and isEmpty work")
{
    NightComp loc1("Stack One");
    NightComp loc2("Stack Two");

    NightDerived first("2026-02-08", 20, EARLY_NIGHT, loc1, 1);
    NightDerived second("2026-02-08", 22, MID_NIGHT, loc2, 2);

    NightStack stack(3);

    CHECK(stack.isEmpty() == true);

    stack.push(&first);
    stack.push(&second);

    CHECK(stack.isEmpty() == false);
    CHECK(stack.getSize() == 2);
    CHECK(stack.peek()->getLocation() == "Stack Two");

    CHECK(stack.pop()->getLocation() == "Stack Two");
    CHECK(stack.pop()->getLocation() == "Stack One");

    CHECK(stack.isEmpty() == true);
}

TEST_CASE("NightStack throws when pushing to a full stack")
{
    NightComp loc1("Full One");
    NightComp loc2("Full Two");
    NightComp loc3("Full Three");

    NightDerived first("2026-02-08", 20, EARLY_NIGHT, loc1, 1);
    NightDerived second("2026-02-08", 21, MID_NIGHT, loc2, 2);
    NightDerived third("2026-02-08", 22, MID_NIGHT, loc3, 3);

    NightStack stack(2);

    stack.push(&first);
    stack.push(&second);

    CHECK_THROWS_AS(stack.push(&third), NightException);
}

TEST_CASE("NightStack throws when popping or peeking from an empty stack")
{
    NightStack stack(2);

    CHECK_THROWS_AS(stack.pop(), NightException);
    CHECK_THROWS_AS(stack.peek(), NightException);
}


// Queue Tests

TEST_CASE("NightQueue enqueue, dequeue, front, and isEmpty work")
{
    NightComp loc1("Queue One");
    NightComp loc2("Queue Two");

    NightDerived first("2026-02-08", 20, EARLY_NIGHT, loc1, 1);
    NightDerived second("2026-02-08", 22, MID_NIGHT, loc2, 2);

    NightQueue queue(3);

    CHECK(queue.isEmpty() == true);

    queue.enqueue(&first);
    queue.enqueue(&second);

    CHECK(queue.isEmpty() == false);
    CHECK(queue.getSize() == 2);
    CHECK(queue.front()->getLocation() == "Queue One");

    CHECK(queue.dequeue()->getLocation() == "Queue One");
    CHECK(queue.front()->getLocation() == "Queue Two");
    CHECK(queue.dequeue()->getLocation() == "Queue Two");

    CHECK(queue.isEmpty() == true);
}

TEST_CASE("NightQueue throws when enqueueing into a full queue")
{
    NightComp loc1("Full Queue One");
    NightComp loc2("Full Queue Two");
    NightComp loc3("Full Queue Three");

    NightDerived first("2026-02-08", 20, EARLY_NIGHT, loc1, 1);
    NightDerived second("2026-02-08", 21, MID_NIGHT, loc2, 2);
    NightDerived third("2026-02-08", 22, MID_NIGHT, loc3, 3);

    NightQueue queue(2);

    queue.enqueue(&first);
    queue.enqueue(&second);

    CHECK_THROWS_AS(queue.enqueue(&third), NightException);
}

TEST_CASE("NightQueue throws when dequeuing or viewing front from an empty queue")
{
    NightQueue queue(2);

    CHECK_THROWS_AS(queue.dequeue(), NightException);
    CHECK_THROWS_AS(queue.front(), NightException);
}

TEST_CASE("NightQueue circular behavior works after dequeue")
{
    NightComp loc1("Circular One");
    NightComp loc2("Circular Two");
    NightComp loc3("Circular Three");
    NightComp loc4("Circular Four");

    NightDerived first("2026-02-08", 20, EARLY_NIGHT, loc1, 1);
    NightDerived second("2026-02-08", 21, MID_NIGHT, loc2, 2);
    NightDerived third("2026-02-08", 22, MID_NIGHT, loc3, 3);
    NightDerived fourth("2026-02-08", 23, MID_NIGHT, loc4, 4);

    NightQueue queue(3);

    queue.enqueue(&first);
    queue.enqueue(&second);
    queue.enqueue(&third);

    CHECK(queue.dequeue()->getLocation() == "Circular One");

    queue.enqueue(&fourth);

    CHECK(queue.dequeue()->getLocation() == "Circular Two");
    CHECK(queue.dequeue()->getLocation() == "Circular Three");
    CHECK(queue.dequeue()->getLocation() == "Circular Four");
    CHECK(queue.isEmpty() == true);
}


// Manager Integration Tests

TEST_CASE("NightManager printNewestFirst uses stack to reverse order")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("First"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("Second"), 2
    );

    std::ostringstream oss;
    manager.printNewestFirst(oss);

    std::string out = oss.str();

    size_t secondPos = out.find("Second");
    size_t firstPos = out.find("First");

    CHECK(secondPos != std::string::npos);
    CHECK(firstPos != std::string::npos);
    CHECK(secondPos < firstPos);
}

TEST_CASE("NightManager printReviewQueue uses queue to preserve original order")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("First"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("Second"), 2
    );

    std::ostringstream oss;
    manager.printReviewQueue(oss);

    std::string out = oss.str();

    size_t firstPos = out.find("First");
    size_t secondPos = out.find("Second");

    CHECK(firstPos != std::string::npos);
    CHECK(secondPos != std::string::npos);
    CHECK(firstPos < secondPos);
}


// STL Map Header Tests

TEST_CASE("NightLocationMap inserts and looks up location indexes")
{
    NightLocationMap map;

    map.insertLocation("Detroit", 0);
    map.insertLocation("NYC", 1);

    CHECK(map.getSize() == 2);
    CHECK(map.lookupLocation("Detroit") == 0);
    CHECK(map.lookupLocation("NYC") == 1);
}

TEST_CASE("NightLocationMap lookup returns -1 when key does not exist")
{
    NightLocationMap map;

    map.insertLocation("Detroit", 0);

    CHECK(map.lookupLocation("Missing") == -1);
}

TEST_CASE("NightLocationMap deletes existing location")
{
    NightLocationMap map;

    map.insertLocation("Detroit", 0);
    map.insertLocation("NYC", 1);

    CHECK(map.deleteLocation("Detroit") == true);
    CHECK(map.lookupLocation("Detroit") == -1);
    CHECK(map.getSize() == 1);
}

TEST_CASE("NightLocationMap delete returns false when key does not exist")
{
    NightLocationMap map;

    map.insertLocation("Detroit", 0);

    CHECK(map.deleteLocation("Missing") == false);
    CHECK(map.getSize() == 1);
}

TEST_CASE("NightLocationMap iterates and prints all key value pairs")
{
    NightLocationMap map;

    map.insertLocation("Detroit", 0);
    map.insertLocation("NYC", 1);

    std::ostringstream oss;
    map.printAll(oss);

    std::string out = oss.str();

    CHECK(out.find("Detroit: 0") != std::string::npos);
    CHECK(out.find("NYC: 1") != std::string::npos);
}


// Manager STL Map Integration Tests

TEST_CASE("NightManager STL map inserts and looks up locations")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("NYC"), 2
    );

    CHECK(manager.getLocationMapSize() == 2);
    CHECK(manager.findByLocation("Detroit") == 0);
    CHECK(manager.findByLocation("NYC") == 1);
}

TEST_CASE("NightManager STL map lookup returns -1 when location does not exist")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 1
    );

    CHECK(manager.findByLocation("Missing") == -1);
}

TEST_CASE("NightManager STL map deletes location through removeByLocation")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("NYC"), 2
    );

    int removedIndex = manager.removeByLocation("Detroit");

    CHECK(removedIndex == 0);
    CHECK(manager.getSize() == 1);
    CHECK(manager.getLocationMapSize() == 1);
    CHECK(manager.findByLocation("Detroit") == -1);
    CHECK(manager.findByLocation("NYC") == 0);
}

TEST_CASE("NightManager STL map delete returns -1 when location does not exist")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 1
    );

    int removedIndex = manager.removeByLocation("Missing");

    CHECK(removedIndex == -1);
    CHECK(manager.getSize() == 1);
    CHECK(manager.getLocationMapSize() == 1);
}

TEST_CASE("NightManager STL map iterates and prints all key value pairs")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Detroit"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("NYC"), 2
    );

    std::ostringstream oss;
    manager.printLocationIndexMap(oss);

    std::string out = oss.str();

    CHECK(out.find("Detroit: 0") != std::string::npos);
    CHECK(out.find("NYC: 1") != std::string::npos);
}

TEST_CASE("NightManager STL map rebuilds indexes after sorting")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("Late"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 18, EARLY_NIGHT,
        NightComp("Early"), 2
    );

    CHECK(manager.findByLocation("Late") == 0);
    CHECK(manager.findByLocation("Early") == 1);

    manager.sortByHour();

    CHECK(manager.findByLocation("Early") == 0);
    CHECK(manager.findByLocation("Late") == 1);
}

TEST_CASE("NightManager STL map keeps first index for duplicate locations")
{
    NightManager manager;

    manager += new NightDerived(
        "2026-02-08", 20, EARLY_NIGHT,
        NightComp("Duplicate"), 1
    );

    manager += new NightDerived(
        "2026-02-08", 22, MID_NIGHT,
        NightComp("Duplicate"), 2
    );

    CHECK(manager.getSize() == 2);
    CHECK(manager.getLocationMapSize() == 1);
    CHECK(manager.findByLocation("Duplicate") == 0);
}


// JSON Loader Tests

TEST_CASE("NightManager loads observations from a JSON file through NightJSONLoader")
{
    const std::string fileName = "test_night_observations.json";

    std::ofstream output(fileName);

    output << R"([
        { "date": "2026-02-08", "hour24": 20, "phase": "EARLY_NIGHT", "location": "Detroit", "objectCount": 2 },
        { "date": "2026-02-08", "hour24": 22, "phase": "MID_NIGHT", "location": "NYC", "objectCount": 4 },
        { "date": "2026-02-09", "hour24": 1, "phase": "LATE_NIGHT", "location": "Moon", "objectCount": 7 },
        { "date": "2026-02-09", "hour24": 3, "phase": "LATE_NIGHT", "location": "Mars", "objectCount": 9 },
        { "date": "2026-02-10", "hour24": 18, "phase": "EARLY_NIGHT", "location": "Wayne State", "objectCount": 5 }
    ])";

    output.close();

    NightManager manager;

    int loadedCount = manager.loadFromJSONFile(fileName);

    CHECK(loadedCount == 5);
    CHECK(manager.getSize() == 5);
    CHECK(manager.getLocationMapSize() == 5);

    CHECK(manager.findByLocation("Detroit") == 0);
    CHECK(manager.findByLocation("NYC") == 1);
    CHECK(manager.findByLocation("Moon") == 2);
    CHECK(manager.findByLocation("Mars") == 3);
    CHECK(manager.findByLocation("Wayne State") == 4);

    std::remove(fileName.c_str());
}

TEST_CASE("NightManager JSON loader returns -1 when file is missing")
{
    NightManager manager;

    int loadedCount = manager.loadFromJSONFile("missing_night_file.json");

    CHECK(loadedCount == -1);
    CHECK(manager.getSize() == 0);
    CHECK(manager.getLocationMapSize() == 0);
}

TEST_CASE("NightManager JSON loader returns -1 when JSON is malformed")
{
    const std::string fileName = "bad_night_observations.json";

    std::ofstream output(fileName);

    output << R"([
        { "date": "2026-02-08", "hour24": 20, "phase": "EARLY_NIGHT", "location": "Detroit", "objectCount": 2 },
    )";

    output.close();

    NightManager manager;

    int loadedCount = manager.loadFromJSONFile(fileName);

    CHECK(loadedCount == -1);
    CHECK(manager.getSize() == 0);
    CHECK(manager.getLocationMapSize() == 0);

    std::remove(fileName.c_str());
}

TEST_CASE("NightManager JSON loaded data works with existing printAll")
{
    const std::string fileName = "print_night_observations.json";

    std::ofstream output(fileName);

    output << R"([
        { "date": "2026-02-08", "hour24": 20, "phase": "EARLY_NIGHT", "location": "Detroit", "objectCount": 2 },
        { "date": "2026-02-08", "hour24": 22, "phase": "MID_NIGHT", "location": "NYC", "objectCount": 4 },
        { "date": "2026-02-09", "hour24": 1, "phase": "LATE_NIGHT", "location": "Moon", "objectCount": 7 },
        { "date": "2026-02-09", "hour24": 3, "phase": "LATE_NIGHT", "location": "Mars", "objectCount": 9 },
        { "date": "2026-02-10", "hour24": 18, "phase": "EARLY_NIGHT", "location": "Wayne State", "objectCount": 5 }
    ])";

    output.close();

    NightManager manager;

    CHECK(manager.loadFromJSONFile(fileName) == 5);

    std::ostringstream oss;
    manager.printAll(oss);

    std::string out = oss.str();

    CHECK(out.find("Detroit") != std::string::npos);
    CHECK(out.find("NYC") != std::string::npos);
    CHECK(out.find("Moon") != std::string::npos);
    CHECK(out.find("Mars") != std::string::npos);
    CHECK(out.find("Wayne State") != std::string::npos);

    std::remove(fileName.c_str());
}