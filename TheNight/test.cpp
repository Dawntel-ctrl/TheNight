#define RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


#include "NightManager.h"
#include "NightBase.h"
#include "NightComp.h"
#include "NightOver.h"
#include "NightDerived.h"
#include "NightException.h"
#include <sstream>
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


//  test FOR manager (existing functionality)

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