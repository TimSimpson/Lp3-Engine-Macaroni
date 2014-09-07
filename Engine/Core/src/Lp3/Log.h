#ifndef FILE_LP3_LOG_H
#define FILE_LP3_LOG_H

#include <Config_Lp3_Core.h>
#ifdef BOOST_HAS_PRAGMA_ONCE
    #pragma once
#endif

#include <Lp3/LE.h>


#ifdef LP3_COMPILE_LOG_DISABLE

    #define LP3_LOG_CRITICAL(m, ...) /* m */
    #define LP3_LOG_DEBUG(m, ...) /* m */
    #define LP3_LOG_ERROR(m, ...) /* m */
    #define LP3_LOG_INFO(m, ...) /* m */
    #define LP3_LOG_WARNING(m, ...) /* m */
    #define LP3_LOG_SET(name, value) /* name, value */

#else
    #include <boost/format.hpp>

    namespace Lp3 { namespace Log {
        enum class LogLevel
        {
            Critical,
            Debug,
            Error,
            Info,
            Warning
        };

        template<typename HeadType, typename... TailTypes>
        void feedToFormat(boost::format & format, const HeadType & head,
                          const TailTypes... tail)
        {
            format % head;
            feedToFormat(format, tail...);
        }

        template<typename LastType>
        void feedToFormat(boost::format & format, const LastType & arg)
        {
            format % arg;
        }

        MACARONI_LIB_DECL_Lp3___Lp3_46_Engine_46_Core___DEV___lib
        void WriteLog(const char * const filename, const int lineNumber,
                      const LogLevel & level, const char * const message);


        template<typename... ParameterTypes>
        void WriteLog(const char * const filename, const int lineNumber,
                      const LogLevel & level, const char * const formatString,
                      const ParameterTypes... args)
        {
            auto format = boost::format(formatString);
            feedToFormat(format, args...);
            const auto message = str(format);
            WriteLog(filename, lineNumber, level, message.c_str());
        }

    } }


    #include <Lp3/Engine/Log/ErrOutPipe.h>
    #include <boost/format.hpp>

    #define LP3_LOG(l, m, ...) \
        Lp3::Log::WriteLog(__FILE__, __LINE__, l, m, ##__VA_ARGS__);
    #define LP3_LOG_CRITICAL(m, ...) LP3_LOG(Lp3::Log::LogLevel::Critical, m,  ##__VA_ARGS__);
    #define LP3_LOG_DEBUG(m, ...) LP3_LOG(Lp3::Log::LogLevel::Debug, m,  ##__VA_ARGS__);
    #define LP3_LOG_INFO(m, ...) LP3_LOG(Lp3::Log::LogLevel::Info, m,  ##__VA_ARGS__);
    #define LP3_LOG_ERROR(m, ...) LP3_LOG(Lp3::Log::LogLevel::Error, m,  ##__VA_ARGS__);
    #define LP3_LOG_WARNING(m, ...) LP3_LOG(Lp3::Log::LogLevel::Warning, m,  ##__VA_ARGS__);

    #define LP3_LOG_SET(name, value) \
    {   \
        LP3_LOG_DEBUG("%s@%s", name, value); \
    }


#endif

#endif