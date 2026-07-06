#define TIME_CORRECTION -5.9 // seconds to advance/retard per hour
// -5.9 set @12pm 3/14  -9s/27h = -0.33s/hr [-6.23]
// -6 set @8am 3/14 too slow
// -5.85 set @9am 3/12 +35s/48hr = +0.72s/hr [-6.57]
// -5.70 set @10am 3/11 +10s/23hr = +0.43s/hr [-6.13]
// -5.70 set @10:30am 3/10  +18s fast/22hr = +0.82s/hr [-6.52]
// -6.02 set @8:25am 3/9 slow ~2-3 minutes
// -5.40 set @10am 3/8; 14s fast / 22.5hr = +0.62s /hr [-6.02]
// -5.32 set @10am 3/6; reset @ 12:01 3/7  result: 15s fast / 22 hr = +0.68s/hr  [-6]
// -5.25 set @8:10am  result: 10am + 1d = 5s fast / 26hrs = +0.20s/hr [-5.45]
// -5 set @10pm 3/4 (9s fast @10hrs) +0.90s/hr [-5.9]
// -7 set @8pm 3/4 (15s slow @ 2 hrs) -7.5s/hr [0.5]
// -13 set @2pm 3/3  (22s slow @3hrs)  -7s/hr   [-4]
// -17 set @9am 3/3 RESULT: 1:56pm slow by 63s/5hr = -12.6s/hr   [-4.4]
// -18 set @ 8:35pm 3/2   RESULT: slow 3m15 = 195s/12.5hr = -15.6s/hr [-2.4]
// -19.35 set @8:12am 3/2 result: slow 75s/5.5h = -13.6s/hr 
// -19.69 set @ 10:47pm 3/1 result: slow 75s/9.3h = -8s/hour


void correctTime() {
  // every hour, update the clock to account for drift
  int correction = (int) (TIME_CORRECTION / 6); // every 10 minutes
  unsigned long newTime = rtc.getEpoch() + correction;
  rtc.setTime(newTime);
}
