#ifdef RUN_TESTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

#include "TheNight.h"

// ------------------------------------------------------------
// A) Calculations — at least 4 tests
// ------------------------------------------------------------
TEST_CASE("A) computeAverageHour - normal + edge + guard") {
    AstronomyTracker t;

    AstronomyTracker::Observation arr[3];
    arr[0] = { "2026-01-18", 20, "City Park", t.computePhase(20) };
    arr[1] = { "2026-01-19", 22, "City Park", t.computePhase(22) };
    arr[2] = { "2026-01-20",  0, "City Park", t.computePhase(0) };

    CHECK(t.computeAverageHour(arr, 3) == doctest::Approx((20 + 22 + 0) / 3.0));
    CHECK(t.computeAverageHour(arr, 1) == doctest::Approx(20.0));
    CHECK(t.computeAverageHour(arr, 0) == doctest::Approx(0.0));
    CHECK(t.computeAverageHour(arr, -5) == doctest::Approx(0.0));
}

// ------------------------------------------------------------
// B) Enum decision logic
// ------------------------------------------------------------
TEST_CASE("B) computePhase - enum logic") {
    AstronomyTracker t;

    CHECK(t.computePhase(20) == AstronomyTracker::EARLY_NIGHT);
    CHECK(t.computePhase(23) == AstronomyTracker::MID_NIGHT);
    CHECK(t.computePhase(3)  == AstronomyTracker::LATE_NIGHT);
    CHECK(t.computePhase(12) == AstronomyTracker::DAYTIME);
}

// ------------------------------------------------------------
// C) Struct/array processing — at least 3 tests
// ------------------------------------------------------------
TEST_CASE("C) countPhase + mostCommonPhase - array/struct processing") {
    AstronomyTracker t;

    AstronomyTracker::Observation arr[4];
    arr[0] = { "d1", 20, "Loc", t.computePhase(20) }; // EARLY
    arr[1] = { "d2", 23, "Loc", t.computePhase(23) }; // MID
    arr[2] = { "d3", 23, "Loc", t.computePhase(23) }; // MID
    arr[3] = { "d4",  3, "Loc", t.computePhase(3)  }; // LATE

    CHECK(t.countPhase(arr, 4, AstronomyTracker::EARLY_NIGHT) == 1);
    CHECK(t.countPhase(arr, 4, AstronomyTracker::MID_NIGHT)   == 2);
    CHECK(t.countPhase(arr, 4, AstronomyTracker::LATE_NIGHT)  == 1);

    CHECK(t.mostCommonPhase(1, 2, 1) == AstronomyTracker::MID_NIGHT);
}
