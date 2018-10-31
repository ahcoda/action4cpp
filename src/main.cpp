#include <stdio.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <cctype>
#include "repl/repl.h"
#include "logger/logger.h"
#include "vender/argh.h"

using namespace std;

void testLogger()
{
    LOG(INFO) << "My first info log using default logger";
    LOG(TRACE) << "***** trace log  *****";
    LOG(DEBUG) << "***** debug log  *****";
    LOG(ERROR) << "***** error log  *****";
    LOG(WARNING) << "***** warning log  *****";
    LOG(INFO) << "***** info log  *****";
    int s = 100;
    cout << "x = " << s << endl;

    /// Debug模式日志记录
    DLOG(TRACE);
    DLOG(DEBUG);
    DLOG(FATAL);
    DLOG(ERROR);
    DLOG(WARNING);
    DLOG(INFO);
    DVLOG(0);
}

/**
 *  解析自定义命令
 */
void parse(int argc, char **argv)
{
    auto cmd = argh::parser(argc, argv);

    std::string code = cmd[0];

    if (code.compare("show") == 0)
    {
        std::cout << "show" << std::endl;
        bool isOK = cmd[{"-v", "--verbose"}];
        if (isOK)
        {
            std::cout << "verbose" << std::endl;
        }
    }
    else if (code.compare("log") == 0)
    {
        testLogger();
    }
    else
    {
        std::cout << "未能识别的命令" << std::endl;
    }
}

/**
 * TODO "入口函数"
 */
int main(int argc, char **argv)
{
    init_logger(argc, argv);

    run_cli_repl();

    return 0;
}
