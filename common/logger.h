#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__


#include <string>


namespace Tackle
{

    enum LoggingLevel
    {
        LoggingLevel_None = 0,
        LoggingLevel_Fatal,
        LoggingLevel_Error,
        LoggingLevel_Warning,
        LoggingLevel_Info,
        LoggingLevel_Debug,
        LoggingLevel_Count
    };


    enum LogOutputType
    {
        LogOutputType_Empty,
        LogOutputType_Printf,
        LogOutputType_File,
        LogOutputType_PrintAndFile
    };


    typedef void (*LogOutputerProc)( char const* text, LoggingLevel level );


    class Logger
    {
    public:
        Logger( LoggingLevel lvl, char const* prefix );

        template <typename... ArgT>
        void operator()( LoggingLevel level, char const* str, ArgT... arg )
        {
            print( level, str, arg... );
        }

    private:
        int print( LoggingLevel level, char const* format, ... );
        int vprint( LoggingLevel level, char const* format, va_list params );

        LoggingLevel        _lvl;
        std::string const   _prefix;
        LogOutputerProc     _output_proc;
    };


    template <typename Tr, LoggingLevel level>
    struct LoggerWrapper
    {
        LoggerWrapper()
        {
        }

        template <typename... ArgT>
        LoggerWrapper( const char* str, ArgT... arg )
        {
            Tr::get_logger()( level, str, arg... );
        }
    };


    template <typename T, LoggingLevel level>
    struct TemplatedLoggerOutputer
    {
        static Logger& get_logger();
    };

    template <typename T, LoggingLevel level>
    /*static*/ Logger& TemplatedLoggerOutputer<T, level>::get_logger()
    {
        static Logger logger( level, T::logger_name );
        return logger;
    }


    class LoggerBase
    {
    public:
        Logger*  get_logger() { return _logger; }

    protected:
        LoggerBase( char const* name, Logger* logger );
        ~LoggerBase();

    private:
        std::string const   _name;
        Logger*             _logger;
    };


    template<typename T, LoggingLevel static_level>
    struct LoggerImpl : LoggerBase
    {
        typedef TemplatedLoggerOutputer<T, static_level> outputer_type;

        #ifdef Debug
        #error "'Debug' macro should not be used!"
        #endif
        typedef LoggerWrapper<outputer_type, LoggingLevel_Debug  > Debug;
        typedef LoggerWrapper<outputer_type, LoggingLevel_Info   > Info;
        typedef LoggerWrapper<outputer_type, LoggingLevel_Warning> Warning;
        typedef LoggerWrapper<outputer_type, LoggingLevel_Error  > Error;
        typedef LoggerWrapper<outputer_type, LoggingLevel_Fatal  > Fatal;

        LoggerImpl()  : LoggerBase( T::logger_name, &outputer_type::get_logger() ) {}
    };


    struct DefaultLoggerOutputer
    {
        static Logger& get_logger();
    };

} // namespace Tackle


namespace OwnLogger
{
    #ifdef Debug
    #define "'Debug' macro should not be used!"
    #endif
    typedef Tackle::LoggerWrapper<Tackle::DefaultLoggerOutputer, Tackle::LoggingLevel_Fatal  > Fatal;
    typedef Tackle::LoggerWrapper<Tackle::DefaultLoggerOutputer, Tackle::LoggingLevel_Error  > Error;
    typedef Tackle::LoggerWrapper<Tackle::DefaultLoggerOutputer, Tackle::LoggingLevel_Warning> Warning;
    typedef Tackle::LoggerWrapper<Tackle::DefaultLoggerOutputer, Tackle::LoggingLevel_Info   > Info;
    typedef Tackle::LoggerWrapper<Tackle::DefaultLoggerOutputer, Tackle::LoggingLevel_Debug  > Debug;
}


#define TACKLE_DECLARE_DEFAULT_LOGGER( Subsystem ) \
    TACKLE_DECLARE_LOGGER( Subsystem, ::Tackle::LoggingLevel_Count )

#define TACKLE_DECLARE_INHERITABLE_LOGGER( Subsystem, Level ) \
    protected: \
        TACKLE_DECLARE_LOGGER_BASE( Subsystem, Level )

#define TACKLE_DECLARE_LOGGER( Subsystem, Level ) \
    private: \
        TACKLE_DECLARE_LOGGER_BASE( Subsystem, Level )

#define TACKLE_DECLARE_LOGGER_BASE( Subsystem, Level ) \
        typedef ::Tackle::LoggerImpl<Subsystem, Level> OwnLogger; \
        static OwnLogger _logger; \
        inline static ::Tackle::Logger& get_logger() \
        { \
            ::Tackle::Logger* real_logger = _logger.get_logger(); \
            assert( real_logger != NULL ); \
            return *real_logger; \
        } \
    public: \
        static const char* const logger_name; \

#define TACKLE_IMPLEMENT_LOGGER( Subsystem ) \
    TACKLE_IMPLEMENT_NAMED_LOGGER( Subsystem, #Subsystem )

#define TACKLE_IMPLEMENT_NAMED_LOGGER( Subsystem, Name ) \
    Subsystem::OwnLogger Subsystem::_logger; \
    const char* const Subsystem::logger_name = Name; \



#define LogOutPrd( level, ... ) typename OwnLogger::level( __VA_ARGS__ )
#if defined( DEBUG )
    #define LogOut( level, ... ) LogOutPrd( level, __VA_ARGS__ )
#else
    #define LogOut( level, ... )
#endif


#endif // __COMMON_LOGGER_H__
