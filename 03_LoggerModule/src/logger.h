//
// Created by mathews on 3/22/22.
// Taken from WiPS project
// Credits : M.Engel, T.Braun, C.Kramer, D.Christmann
//

#ifndef INC_03_LOGGERMODULE_LOGGER_H
#define INC_03_LOGGERMODULE_LOGGER_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <mutex>

#define STR1(x) #x
#define TOString(x)  STR1(x)

namespace LOG_COLOR{
    const std::string COLOR_RESET  = "\033[0m";
    const std::string BLACK  = "\033[30m";
    const std::string RED  = "\033[31m" ;
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE  = "\033[34m";

    const std::string MAGENTA = "\033[35m";
    const std::string CYAN =   "\033[36m";
    const std::string WHITE =   "\033[37m" ;     /* White */
    const std::string BOLDBLACK =  "\033[1m\033[30m"  ;    /* Bold Black */
    const std::string BOLDRED    = "\033[1m\033[31m"  ;    /* Bold Red */
    const std::string BOLDGREEN  = "\033[1m\033[32m" ;     /* Bold Green */
    const std::string BOLDYELLOW = "\033[1m\033[33m" ;     /* Bold Yellow */
    const std::string BOLDBLUE   = "\033[1m\033[34m" ;     /* Bold Blue */
    const std::string BOLDMAGENTA = "\033[1m\033[35m";      /* Bold Magenta */
    const std::string BOLDCYAN   = "\033[1m\033[36m";      /* Bold Cyan */
    const std::string BOLDWHITE  = "\033[1m\033[37m";      /* Bold White */

}


// Defintion of BUG-Macro
#define BUG(str) \
			do { 														\
				LOG(LOG_LEVEL_ERROR, str);								\
				LOG(LOG_LEVEL_ERROR, "CRITICAL ERROR EXECTION STOPPED !");\
				while(true); 									  		\
			}while(false)

/**
 *  \ingroup logging
 *
 *  Logging severity classes and levels, see ns3
 */
enum LogLevel {
  LOG_NONE                  = 0x00000000, // no logging
  LOG_LEVEL_ERROR           = 0x00000001, // serious error messages only
  LOG_LEVEL_WARN            = 0x00000002, // warning messages
  LOG_LEVEL_DEBUG           = 0x00000003, // rare ad-hoc debug messages
  LOG_LEVEL_INFO            = 0x00000004, // informational messages (e.g., banners)
  LOG_LEVEL_FUNCTION        = 0x00000005, // function tracing
  LOG_LEVEL_LOGIC           = 0x00000006, // control flow tracing within functions 0x00000009 -> f
  LOG_LEVEL_ALL             = 0x0fffffff, // print everything

  LOG_LEVEL_PREFIX_FUNC    = 0x80000000, // prefix all trace prints with function
  LOG_LEVEL_PREFIX_TIME    = 0x40000000, // prefix all trace prints with simulation time
  LOG_LEVEL_PREFIX_NODE    = 0x20000000, // prefix all trace prints with simulation node
  LOG_LEVEL_PREFIX_LEVEL   = 0x10000000, // prefix all trace prints with log level (severity)
  LOG_LEVEL_PREFIX_ALL     = 0xf0000000  // all prefixes
};

/**
 * \ingroup logging
 * \param name a string
 *
 * Define a Log component with a specific name.
 *
 * This macro should be used at the top of every file in which you want
 * to use the LOG macro. This macro defines a new
 * "log component" which can be later selectively enabled
 * or disabled with the BIPSLogComponentEnable and
 * BIPSLogComponentDisable functions.
 *
 * To create a log component Foo, at the top of foo.cc:
 *
 * \code
 *   LOG_COMPONENT_DEFINE ("Foo")
 *     ;
 * \endcode
 *
 * Note the closing ';' is not on the same line;  this prevents
 * Doxygen from spuriously warning that the macro invocation is undocumented.
 */
#define LOG_COMPONENT_DEFINE(name)                           \
  static LogComponent logComponent{name}



class LogComponent{
private:
    // global list of all registered log components
    static std::unordered_map<std::string,LogComponent*>* logComponents;
    static std::ostream* out;

    // name and loglevel
    std::string name;
    int32_t  m_levels;

    std::string getTime() const;

    static std::mutex mutex_lck;

    // Internal logging functions without locks (usage for reporting errors within the logging system)
    void logNoLock(const std::string& msg);
    void logNoLock(int32_t level, const std::string& context, const std::string& msg);

public:
    LogComponent(const std::string& name);


    void log(int32_t level, const std::string& context, const std::string& msg);
    void logFunction(int32_t level, const std::string& context, const std::string param);
    void log(const std::string& msg);



    static std::string GetLevelLabel(const enum LogLevel level);
    static std::string GetLevelColor(const enum LogLevel level);
    static void setStream(std::ostream& outStream);
    static void enable(const std::string& name, const enum LogLevel level, bool reset);
    void Enable (const enum LogLevel level, bool reset);
    bool IsEnabled(int32_t level) const;

    static void printComponents();
    std::string getName() const;
    std::string getMaxLogLevelLabel() const;

    // no copy or move semantic, remove also operators
    LogComponent(const LogComponent&) = delete;
    LogComponent(LogComponent&&) = delete;

    LogComponent& operator=(const LogComponent&) = delete;
    LogComponent& operator=(LogComponent&&) = delete;

    ~LogComponent();
};



// Forward to object internal log functions
#define LOG(level, msg)											\
		  do {                                                          \
		          std::string context = __PRETTY_FUNCTION__;            \
		          std::stringstream msgStream{};						\
				  msgStream << msg;										\
		          logComponent.log(level, context, msgStream.str());  		\
		    } while (false)




#define LOG_PARAM(level, context, param)   		  			\
	do  {															\
	          std::string logParamContext = __PRETTY_FUNCTION__;            \
	          logComponent.logFunction(level, logParamContext, param);	  		\
	    } while (false)



/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Use \ref LOG to output a message of level LOG_LEVEL_ERROR.
 * Use \ref LOG to output a message of level LOG_LEVEL_WARN.
 * Use \ref LOG to output a message of level LOG_LEVEL_DEBUG.
 * Use \ref LOG to output a message of level LOG_LEVEL_INFO.
 * Use \ref LOG to output a message of level LOG_LEVEL_FUNCTION.
 * Use \ref LOG to output a message of level LOG_LEVEL_LOGIC.

 */
#define LOG_ERROR(msg) \
        LOG(LOG_LEVEL_ERROR, msg)

#define LOG_WARN(msg) \
		LOG(LOG_LEVEL_WARN, msg)

#define LOG_DEBUG(msg) \
		LOG(LOG_LEVEL_DEBUG, msg)

#define LOG_INFO(msg) \
		LOG(LOG_LEVEL_INFO, msg)

#define LOG_LOGIC(msg) \
		LOG(LOG_LEVEL_LOGIC, msg)

#define LOG_UNCOND(msg) \
		logComponent.log(msg)

#define LOG_FUNCTION_NOARGS()                              \
  do {                                                          \
      if (logComponent.IsEnabled (LOG_LEVEL_FUNCTION)) {                  \
          std::string context = __PRETTY_FUNCTION__;            \
          LOG_PARAM(LOG_LEVEL_FUNCTION, context, "");	  	  	\
        }                                                       \
    }  while (false)


#define LOG_FUNCTION(parameters)                           \
  do {                                                          \
      if (logComponent.IsEnabled (LOG_LEVEL_FUNCTION)) {                  \
    	  std::string logFunContext = __PRETTY_FUNCTION__;            \
          std::stringstream parameterStream{};             \
          std::ostream\
          parameterStream << parameters;		\
          LOG_PARAM(LOG_LEVEL_FUNCTION, logFunContext, parameterStream.str()); \
		}                                                       \
    } while (false)




/**
 * \param name a log component name
 * \param level a logging level
 * \ingroup logging
 *
 * Enable the logging output associated with that log component.
 * The logging output can be later disabled with a call
 * to LogComponentDisable.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='name=level'
 */
void LogComponentEnable (const std::string& name, enum LogLevel level);

/**
 * \param name a log component name
 * \param level a logging level
 * \param reset whether or not to overwrite the current level
 * \ingroup logging
 */
void LogComponentEnable (const std::string& name, enum LogLevel level, bool reset);

/**
 * \param name a log component name
 * \ingroup logging
 *
 * Disable all logging output of the given log component.
 *
 */
void LogComponentDisable (const std::string& name);





/**
 * Stolen from ns3
 */
/*class BipsParameterLogger : public std::ostream
{
  int m_itemNumber;
  std::ostream &m_os;
public:
  BipsParameterLogger (std::ostream &os);
};*/




#endif //INC_03_LOGGERMODULE_LOGGER_H
