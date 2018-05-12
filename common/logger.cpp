
#include "logger.h"

#include <stdarg.h>
#include <sys/time.h>


namespace Tackle
{

    namespace
    {

        #define TIME_FORMAT_SPEC "%02u:%02u:%02u.%03u"
        #define USER_TEXT_LENGTH "1024"

        struct Time
        {
            unsigned short year;
            unsigned short month;
            unsigned short day;
            unsigned short hour;
            unsigned short minute;
            unsigned short second;
            unsigned short millisec;
        };

        size_t const MAX_LOG_MESSAGE_SIZE = 1024;
        char const* const level_names[LoggingLevel_Count] = { "[F]", "[E]", "[W]", "[I]", "[D]" };
        char global_prefix[64] = ">>> [" TIME_FORMAT_SPEC "] [%#x]: %." USER_TEXT_LENGTH "s\n";
        LogOutputType output_type = LogOutputType_Printf;
        FILE* log_output = NULL;


        void internal_static_init()
        {}


        size_t vstring_format(char * buf, size_t buf_size, const char * format, va_list va)
        {
            int const printed_size = vsnprintf( buf, buf_size, format, va );
            return static_cast<size_t>( printed_size );
        }


        template <class LHS, class RHS>
        const LHS& cast_assign( LHS& lhs, const RHS& rhs )
        {
            return lhs = static_cast<LHS>(rhs);
        }


        static void get_current_time( Time& cur_time )
        {
            timeval timeval;
            gettimeofday( &timeval, NULL );

            struct tm* const t = localtime( &timeval.tv_sec );

            cast_assign( cur_time.year,     t->tm_year + 1900 );
            cast_assign( cur_time.month,    t->tm_mon + 1 );
            cast_assign( cur_time.day,      t->tm_mday );
            cast_assign( cur_time.hour,     t->tm_hour );
            cast_assign( cur_time.minute,   t->tm_min );
            cast_assign( cur_time.second,   t->tm_sec );
            cast_assign( cur_time.millisec,	timeval.tv_usec / 1000 );
        }


        static inline const uint32_t get_thread_id()
        {
            return (uintptr_t)pthread_self();
        }


        void default_logger_proc( const char* text, LoggingLevel level )
        {
            Time t;
            get_current_time( t );
            uint32_t const thread_id  = get_thread_id();

            if (output_type == LogOutputType_Printf || output_type == LogOutputType_PrintAndFile)
            {
                fprintf( stdout, global_prefix, t.hour, t.minute, t.second, t.millisec, thread_id, text );
                fflush( stdout );
            }

            if ((output_type == LogOutputType_File || output_type == LogOutputType_PrintAndFile) && log_output != NULL)
            {
                fprintf( log_output, global_prefix, t.hour, t.minute, t.second, t.millisec, thread_id, text );
                fflush( log_output );
            }
        }
    }


    LoggerBase::LoggerBase( char const* name, Logger* logger )
        : _name( name )
        , _logger( logger )
    {

    }


    LoggerBase::~LoggerBase()
    {

    }


    Logger::Logger( LoggingLevel lvl, char const* prefix )
        : _lvl( lvl )
        , _prefix( prefix )
        , _output_proc( &default_logger_proc )
    {

    }


    int Logger::print( LoggingLevel level, char const* format, ... )
    {
        int retval = 0;
        if (level <= _lvl)
        {
            va_list params;
            va_start( params, format );
            retval = vprint( level, format, params );
            va_end( params );
        }
        return retval;
    }


    int Logger::vprint( LoggingLevel level, char const* format, va_list params )
    {
        char message[MAX_LOG_MESSAGE_SIZE + 2];
        int prefix_size = 0;
        bool to_log = level <= _lvl;
        if (to_log && level != LoggingLevel_None)
            prefix_size = sprintf( message, "%s %s: ", level_names[level - 1], _prefix.c_str() );
        int printed_size = vstring_format( message + prefix_size, MAX_LOG_MESSAGE_SIZE - prefix_size, format, params );

        printed_size += prefix_size;
        if ((size_t) printed_size > MAX_LOG_MESSAGE_SIZE)
            printed_size = MAX_LOG_MESSAGE_SIZE;

        message[printed_size] = 0;

        for (char* ch = message + printed_size; ch-- > message;)
            if (*ch >= 0 && *ch < 32 && !isspace( *ch ))
                *ch = '?';

        if (to_log)
            _output_proc( message, level );
    }


    Logger &DefaultLoggerOutputer::get_logger()
    {
        static Logger __g_log( LoggingLevel_Count, "Global" );
        internal_static_init();
        return __g_log;
    }

} // namespace Tackle
