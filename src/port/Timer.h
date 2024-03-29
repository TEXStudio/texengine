#ifndef LIBNTIME_TIMER_HPP_INCLUDED
#define LIBNTIME_TIMER_HPP_INCLUDED

/*
    Note:
    If you run into problems while using this library under windows,
    try compiling it with the -no-undefined and --enable-runtime-pseudo-reloc
    linker options.
*/

#if defined(_WIN32)
    #define WIN64_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
    #include <windows.h>
#else
    #error "Unspecified or unsupported platform. Consult the README file."

#endif /* _WIN32 */

namespace TEXEngine {
	namespace Port {

class Timer
{
    public:
        Timer();
        ~Timer();

        void start();   // starts the timer
		void resume();	// resumes the timer
        void stop();    // stops the timer

        double get_time_in_sec();    // returns elapsed time in seconds
        double get_time_in_mlsec();  // returns elapsed time in milli-seconds
        double get_time_in_mcsec();  // returns elapsed time in micro-seconds

    private:
        double m_stime; // starting time in micro-seconds
        double m_etime; // ending time in micro-seconds
        bool m_stopped; // stop flag

	#if defined(_WIN32)
        LARGE_INTEGER m_freq; // ticks per second
        LARGE_INTEGER m_scount; // start count
        LARGE_INTEGER m_ecount; // end count
	#endif /* _WIN32  */
};

	} /* namespace Port */
} /* namespace TEXEngine */

#endif /* LIBNTIME_TIMER_HPP_INCLUDED */