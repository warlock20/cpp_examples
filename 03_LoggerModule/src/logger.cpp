//
// Created by mathews on 3/22/22.
// Taken from WiPS project
// Credits : M.Engel, T.Braun, C.Kramer, D.Christmann
//

#include "logger.h"
#include <iostream>
#include <string>

//#ifdef ENABLE_LOGGING

using namespace LOG_COLOR;

// only for internal usage
#define LOG_NOLOCK(level, msg)										\
		  do {                                                          \
		          std::string context = __PRETTY_FUNCTION__;            \
		          std::stringstream msgStream{};						\
				  msgStream << msg;										\
		          logComponent.logNoLock(level, context, msgStream.str());  \
		    } while (false)


std::unordered_map<std::string, LogComponent*>* LogComponent::logComponents = nullptr;
//std::ostream& out = std::clog;
std::ostream* LogComponent::out = &std::cout;
std::mutex LogComponent::mutex_lck{};

LOG_COMPONENT_DEFINE("MasterLogComponent");


void LogComponentEnable (const std::string& name, const enum LogLevel level){
	LogComponent::enable(name, level, false);
}

void LogComponentEnable (const std::string& name, const enum LogLevel level, bool reset){
    LogComponent::enable(name, level, reset);
}

void LogComponentDisable (const std::string& name){
	LogComponent::enable(name, LOG_NONE, true);
}


LogComponent::LogComponent(const std::string& name) : name{name}, m_levels{LOG_LEVEL_WARN}{
	std::lock_guard<std::mutex> lock{LogComponent::mutex_lck};

	// only create central log object once
	if(!LogComponent::logComponents){
		LogComponent::logComponents = new std::unordered_map<std::string, LogComponent*>{ };
	}

	if(LogComponent::logComponents->find(name) == LogComponent::logComponents->end()){
		(*LogComponent::logComponents)[name] = this;
	}else{
		LOG_NOLOCK(LOG_LEVEL_ERROR,"Component " + name + " already registered!");
	}
}

LogComponent::~LogComponent(){
	out->flush();
}

void LogComponent::setStream(std::ostream& outStream){
	LogComponent::out = &outStream;
}

void LogComponent::enable(const std::string& name, const enum LogLevel level, bool reset){
	std::lock_guard<std::mutex> lock{LogComponent::mutex_lck};

	if(LogComponent::logComponents->find(name) == LogComponent::logComponents->end())
		LOG_NOLOCK(LOG_LEVEL_ERROR,"Component " + name + " not registered!");
	else
		(*LogComponent::logComponents)[name]->Enable(level, reset);
}

void LogComponent::Enable (const enum LogLevel level, bool reset) {
	if(reset)
		this->m_levels = level;
	else
		this->m_levels |= level;
}

bool LogComponent::IsEnabled(int32_t level) const{
	  //  LogComponentEnableEnvVar ();
	  return (level & m_levels) ? true : false;
}

std::string LogComponent::getTime() const{
	std::stringstream tmp{};
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    auto time = (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
	tmp << time;
	return tmp.str();
}

void LogComponent::log(int32_t level, const std::string& context, const std::string& msg){
	if(IsEnabled(level)){
		std::lock_guard<std::mutex> lock{LogComponent::mutex_lck};
		(*LogComponent::out) << BOLDWHITE << "[" << getTime() << ", " << GetLevelColor((enum LogLevel) level)<< LogComponent::GetLevelLabel((enum LogLevel) level) << BOLDWHITE <<  "] " << name << ": " << COLOR_RESET << context << ": " << msg << std::endl;
	}
}

void LogComponent::log(const std::string& msg){
	std::lock_guard<std::mutex> lock(LogComponent::mutex_lck);
	(*LogComponent::out) << "[" << getTime() <<  "]: " << name << ": " << msg << std::endl;
}

void LogComponent::logNoLock(const std::string& msg){
	(*LogComponent::out) << "[" << getTime() <<  "]: " << name << ": " << msg << std::endl;
}

void LogComponent::logNoLock(int32_t level, const std::string& context, const std::string& msg){
	if(IsEnabled(level)){
		(*LogComponent::out) << "[" << getTime() << ", "<< LogComponent::GetLevelLabel((enum LogLevel) level) <<  "] " << name << ": " << context << ": " << msg << std::endl;
	}
}

void LogComponent::logFunction(int32_t level, const std::string& context, const std::string param){
	if(IsEnabled(level)){
		std::lock_guard<std::mutex> lock{LogComponent::mutex_lck};
		(*LogComponent::out) << BOLDWHITE << "[" << getTime() << ", " << GetLevelColor((enum LogLevel) level)<< LogComponent::GetLevelLabel((enum LogLevel) level) << BOLDWHITE << "] " << name << ": " << COLOR_RESET << context << "( " << param << ")" << std::endl;
	}
}

void LogComponent::printComponents() {
	std::lock_guard<std::mutex> lock{LogComponent::mutex_lck};
	for(const auto& pt : *LogComponent::logComponents){
		(*LogComponent::out) << pt.second->getName() << ": " << pt.second->getMaxLogLevelLabel() << std::endl;
	}
	*LogComponent::out << std::endl;
}

std::string LogComponent::getName() const{
	return name;
}

std::string LogComponent::getMaxLogLevelLabel() const{
	static enum LogLevel ll[] = { LOG_LEVEL_LOGIC, LOG_LEVEL_FUNCTION, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_WARN, LOG_LEVEL_ERROR};

	// returns highest enabled loglevel
	for(const auto& it : ll){
		if(this->IsEnabled(it))
			return GetLevelLabel(it);
	}
	return "unknown";
}

std::string LogComponent::GetLevelLabel(const enum LogLevel level){
  	 switch (level){
  	 	 case LOG_LEVEL_ERROR:	    return "ERROR";
  	 	 case LOG_LEVEL_WARN: 	    return "WARN";
  	 	 case LOG_LEVEL_DEBUG: 	    return "DEBUG";
  	 	 case LOG_LEVEL_INFO:		return "INFO";
  	 	 case LOG_LEVEL_FUNCTION:   return "FUNCT";
  	 	 case LOG_LEVEL_LOGIC: 	    return "LOGIC";
  	 	 default:	                return "unkown";
  	 }
}

std::string LogComponent::GetLevelColor(const enum LogLevel level){
    switch (level){
        case LOG_LEVEL_ERROR:	return BOLDRED;
        case LOG_LEVEL_WARN: 	return BOLDYELLOW;
        case LOG_LEVEL_FUNCTION: return BOLDBLACK;
        default:	return BOLDWHITE;
    }
}

/*BipsParameterLogger::BipsParameterLogger (std::ostream &os)
  : std::basic_ostream<char> (os.rdbuf ()),  //!< \bugid{1792}
    m_itemNumber (0),
    m_os (os){
}
*/
// return default log stream
std::ostream* __attribute__((weak)) getLogStream (){
	return &std::clog;
}

//#endif