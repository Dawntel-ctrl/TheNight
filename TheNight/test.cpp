#define RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "NightBase.h"
#include "NightComp.h"
#include "NightOver.h"
#include "NightDerived.h"
#include <sstream>

TEST_CASE("NightBase default ctor initializes fields") {
    NightBase b;
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

TEST_CASE("NightComp helper works (isEmpty)") {
    NightComp c;
    CHECK(c.isEmpty() == true);
    c.setLocation("Detroit");
    CHECK(c.isEmpty() == false);
    CHECK(c.getLocation() == "Detroit");
}

TEST_CASE("NightOver ctor sets base + derived + composition") {
    NightComp loc("Hanoi");
    NightOver o("2026-02-08", 20, EARLY_NIGHT, loc, true);

    CHECK(o.getDate() == "2026-02-08");
    CHECK(o.getHour24() == 20);
    CHECK(o.getPhase() == EARLY_NIGHT);

    CHECK(o.getLocationInfo().getLocation() == "Hanoi");
    CHECK(o.getClearSky() == true);
}

TEST_CASE("Derived print overrides and calls base print") {
    NightComp loc("Wayne State");
    NightDerived d("2026-02-08", 2, LATE_NIGHT, loc, 5);

    std::ostringstream oss;
    d.print(oss);
    const std::string out = oss.str();

    CHECK(out.find("Date: 2026-02-08") != std::string::npos);
    CHECK(out.find("Hour24: 2") != std::string::npos);
    CHECK(out.find("Object count: 5") != std::string::npos);
}
