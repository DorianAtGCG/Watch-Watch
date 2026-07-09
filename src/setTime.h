#define TIME_CORRECTION -0.85 // seconds to advance/retard per hour

// Drift correction approach, recorded 2026-07-07:
// GPS/base-station sync is the source of truth. TIME_CORRECTION only needs to keep the
// watch from drifting noticeably between charging/sync sessions; it does not need to make
// the watch accurate as a long-term free-running clock.
//
// The watch drift has looked erratic enough that single short observations are not reliable.
// Do not chase one reading by immediately changing TIME_CORRECTION; that just fits random noise.
// For future calibration, measure intervals that look like real use between base-station syncs.
// If possible, collect several intervals before adjusting. Record true time, displayed watch time,
// offset seconds, active TIME_CORRECTION, elapsed time since the last GPS/base sync, and whether
// the watch was reset, uploaded, slept oddly, or manually woken.
//
// Compute drift from offset-to-offset:
//   observed drift = (new offset seconds - previous offset seconds) / elapsed hours
//   new factor = old factor - observed drift
//
// Positive offset means the watch is fast. Positive observed drift means it gained time and the
// factor should move more negative. Negative observed drift means it lost time and the factor should
// move more positive. If real-use observations keep changing sign or vary by multiple seconds per
// hour, keep the correction conservative and rely on the next GPS/base sync to clean up the residual
// error. The goal is "not noticeable before the next sync", not perfect oscillator calibration.

// Drift calibration history.
// Formula: new factor = old factor - observed drift. Positive drift means the watch was fast.
// Date       Time     Old factor  Observed drift             Factor change  New factor  Notes
// 2026-07-08 19:46    -1.14       -10.08s/34.32h = -0.29s/hr +0.29         -0.85       Serial request: 39.33s slow, from 29.25s slow at previous point.
// 2026-07-07 09:27    -4.60       -40.25s/11.62h = -3.46s/hr +3.46         -1.14       From +11s fast at 2026-07-06 21:50 to 29.25s slow now.
// 2026-07-07 09:19    -4.60       +11s/5.29h = +2.08s/hr    -2.08          -6.68       Superseded: this was calculated without a fresh observation.
//
// Legacy notes, preserved verbatim enough to keep the original adjustment trail:
// -4.60 set @9:50pm 7/6  +11s/5.29h = +2.08s/hr [-4.60]
// -2.52 set @4:33pm 7/6  -4.68s/1.39h = -3.38s/hr [-2.52]
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
  float correctionSeconds = (TIME_CORRECTION * SLEEP_TIME / 3600.0) + RTC_CORRECTION_REMAINDER;
  int correction = (int) correctionSeconds;
  RTC_CORRECTION_REMAINDER = correctionSeconds - correction;

  if (correction == 0)
    return;

  unsigned long newTime = rtc.getLocalEpoch() + correction;
  rtc.setTime(newTime);
}
