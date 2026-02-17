#define RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"


#include "NightManager.h"
#include "NightBase.h"
#include "NightComp.h"
#include "NightOver.h"
#include "NightDerived.h"

#include <sstream>


//  NightBase is now abstract


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

    CHECK(out.find("Mars") != std::string::npos);
    CHECK(out.find("Object count: 7") != std::string::npos);
}
