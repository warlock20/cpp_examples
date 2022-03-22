//
// Created by mathews on 3/22/22.
//

#include "logger.h"

LOG_COMPONENT_DEFINE("main");

int main (int argc, char * argv[]){

    LogComponentEnable("main",LOG_LEVEL_ALL);
    LOG_INFO("Test message");
    LOG_ERROR("Test message");
    LOG_LOGIC("Test message");
    LOG_DEBUG("Test message");
    LOG_WARN("Test message");


    return 0;
}