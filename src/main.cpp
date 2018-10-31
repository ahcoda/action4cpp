#include <stdio.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <cctype>
#include <utility>
#include <iomanip>
#include "vender/argh.h"
#include "vender/easylogging++.h"
#include "vender/args_converter.h"

#include "replxx.hxx"
using namespace std;
INITIALIZE_EASYLOGGINGPP;

#define ELPP_LOGGING_FLAGS_FROM_ARG
void initLogger(int argc, char **argv)
{
    START_EASYLOGGINGPP(argc, argv);

    el::Configurations conf("../default-logger.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    /// 防止Fatal级别日志中断程序
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    /// 选择划分级别的日志
    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
    /// 设置级别门阀值，修改参数可以控制日志输出
    el::Loggers::setLoggingLevel(el::Level::Global);
}

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
using Replxx = replxx::Replxx;

// prototypes
Replxx::completions_t hook_completion(std::string const &context, int index, void *user_data);
Replxx::hints_t hook_hint(std::string const &context, int index, Replxx::Color &color, void *user_data);
void hook_color(std::string const &str, Replxx::colors_t &colors, void *user_data);

Replxx::completions_t hook_completion(std::string const &context, int index, void *user_data)
{
    auto *examples = static_cast<std::vector<std::string> *>(user_data);
    Replxx::completions_t completions;

    std::string prefix{context.substr(index)};
    for (auto const &e : *examples)
    {
        if (e.compare(0, prefix.size(), prefix) == 0)
        {
            completions.emplace_back(e.c_str());
        }
    }

    return completions;
}

Replxx::hints_t hook_hint(std::string const &context, int index, Replxx::Color &color, void *user_data)
{
    auto *examples = static_cast<std::vector<std::string> *>(user_data);
    Replxx::hints_t hints;

    // only show hint if prefix is at least 'n' chars long
    // or if prefix begins with a specific character

    std::string prefix{context.substr(index)};
    if (prefix.size() >= 2 || (!prefix.empty() && prefix.at(0) == '.'))
    {
        for (auto const &e : *examples)
        {
            if (e.compare(0, prefix.size(), prefix) == 0)
            {
                hints.emplace_back(e.substr(prefix.size()).c_str());
            }
        }
    }

    // set hint color to green if single match found
    if (hints.size() == 1)
    {
        color = Replxx::Color::GREEN;
    }

    return hints;
}

int real_len(std::string const &s)
{
    int len(0);
    unsigned char m4(128 + 64 + 32 + 16);
    unsigned char m3(128 + 64 + 32);
    unsigned char m2(128 + 64);
    for (int i(0); i < static_cast<int>(s.length()); ++i, ++len)
    {
        char c(s[i]);
        if ((c & m4) == m4)
        {
            i += 3;
        }
        else if ((c & m3) == m3)
        {
            i += 2;
        }
        else if ((c & m2) == m2)
        {
            i += 1;
        }
    }
    return (len);
}

void hook_color(std::string const &context, Replxx::colors_t &colors, void *user_data)
{
    auto *regex_color = static_cast<std::vector<std::pair<std::string, Replxx::Color>> *>(user_data);

    // highlight matching regex sequences
    for (auto const &e : *regex_color)
    {
        size_t pos{0};
        std::string str = context;
        std::smatch match;

        while (std::regex_search(str, match, std::regex(e.first)))
        {
            std::string c{match[0]};
            pos += real_len(match.prefix());
            int len(real_len(c));

            for (int i = 0; i < len; ++i)
            {
                colors.at(pos + i) = e.second;
            }

            pos += len;
            str = match.suffix();
        }
    }
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
        return;
    }

    std::cout << "未能识别的命令" << std::endl;
}

void runRepl()
{
    // words to be completed
    std::vector<std::string>
        examples{
            ".help",
            ".history",
            ".quit",
            ".exit",
            ".clear",
            ".prompt ",
            "hello",
            "world",
            "db",
            "data",
            "drive",
            "show",
            "print",
            "put",
            "color_black",
            "color_red",
            "color_green",
            "color_brown",
            "color_blue",
            "color_magenta",
            "color_cyan",
            "color_lightgray",
            "color_gray",
            "color_brightred",
            "color_brightgreen",
            "color_yellow",
            "color_brightblue",
            "color_brightmagenta",
            "color_brightcyan",
            "color_white",
            "color_normal",
        };

    // highlight specific words
    // a regex string, and a color
    // the order matters, the last match will take precedence
    using cl = Replxx::Color;
    std::vector<std::pair<std::string, cl>> regex_color{
        // single chars
        {"\\`", cl::BRIGHTCYAN},
        {"\\'", cl::BRIGHTBLUE},
        {"\\\"", cl::BRIGHTBLUE},
        {"\\-", cl::BRIGHTBLUE},
        {"\\+", cl::BRIGHTBLUE},
        {"\\=", cl::BRIGHTBLUE},
        {"\\/", cl::BRIGHTBLUE},
        {"\\*", cl::BRIGHTBLUE},
        {"\\^", cl::BRIGHTBLUE},
        {"\\.", cl::BRIGHTMAGENTA},
        {"\\(", cl::BRIGHTMAGENTA},
        {"\\)", cl::BRIGHTMAGENTA},
        {"\\[", cl::BRIGHTMAGENTA},
        {"\\]", cl::BRIGHTMAGENTA},
        {"\\{", cl::BRIGHTMAGENTA},
        {"\\}", cl::BRIGHTMAGENTA},

        // color keywords
        {"color_black", cl::BLACK},
        {"color_red", cl::RED},
        {"color_green", cl::GREEN},
        {"color_brown", cl::BROWN},
        {"color_blue", cl::BLUE},
        {"color_magenta", cl::MAGENTA},
        {"color_cyan", cl::CYAN},
        {"color_lightgray", cl::LIGHTGRAY},
        {"color_gray", cl::GRAY},
        {"color_brightred", cl::BRIGHTRED},
        {"color_brightgreen", cl::BRIGHTGREEN},
        {"color_yellow", cl::YELLOW},
        {"color_brightblue", cl::BRIGHTBLUE},
        {"color_brightmagenta", cl::BRIGHTMAGENTA},
        {"color_brightcyan", cl::BRIGHTCYAN},
        {"color_white", cl::WHITE},
        {"color_normal", cl::NORMAL},

        // commands
        {"\\.help", cl::BRIGHTMAGENTA},
        {"\\.history", cl::BRIGHTMAGENTA},
        {"\\.quit", cl::BRIGHTMAGENTA},
        {"\\.exit", cl::BRIGHTMAGENTA},
        {"\\.clear", cl::BRIGHTMAGENTA},
        {"\\.prompt", cl::BRIGHTMAGENTA},

        // numbers
        {"[\\-|+]{0,1}[0-9]+", cl::YELLOW},                    // integers
        {"[\\-|+]{0,1}[0-9]*\\.[0-9]+", cl::YELLOW},           // decimals
        {"[\\-|+]{0,1}[0-9]+e[\\-|+]{0,1}[0-9]+", cl::YELLOW}, // scientific notation

        // strings
        {"\".*?\"", cl::BRIGHTGREEN}, // double quotes
        {"\'.*?\'", cl::BRIGHTGREEN}, // single quotes
    };

    // init the repl
    Replxx rx;
    rx.install_window_change_handler();

    // the path to the history file
    std::string history_file{"./replxx_history.txt"};

    // load the history file if it exists
    rx.history_load(history_file);

    // set the max history size
    rx.set_max_history_size(12);

    // set the max number of hint rows to show
    rx.set_max_hint_rows(8);

    // set the callbacks
    rx.set_completion_callback(hook_completion, static_cast<void *>(&examples));
    rx.set_highlighter_callback(hook_color, static_cast<void *>(&regex_color));
    rx.set_hint_callback(hook_hint, static_cast<void *>(&examples));

    // display initial welcome message
    std::cout
        << "Welcome to Replxx\n"
        << "Press 'tab' to view autocompletions\n"
        << "Type '.help' for help\n"
        << "Type '.quit' or '.exit' to exit\n\n";

    // set the repl prompt
    std::string prompt{"\x1b[1;32mreplxx\x1b[0m> "};

    // main repl loop
    for (;;)
    {
        // display the prompt and retrieve input from the user
        char const *cinput{nullptr};

        do
        {
            cinput = rx.input(prompt);
        } while ((cinput == nullptr) && (errno == EAGAIN));

        if (cinput == nullptr)
        {
            break;
        }

        // change cinput into a std::string
        // easier to manipulate
        std::string input{cinput};

        if (input.empty())
        {
            // user hit enter on an empty line

            continue;
        }
        else if (input.compare(0, 5, ".quit") == 0 || input.compare(0, 5, ".exit") == 0)
        {
            // exit the repl

            rx.history_add(input);
            break;
        }
        else if (input.compare(0, 5, ".help") == 0)
        {
            // display the help output
            std::cout
                << ".help\n\tdisplays the help output\n"
                << ".quit\n\texit the repl\n"
                << ".exit\n\texit the repl\n"
                << ".clear\n\tclears the screen\n"
                << ".history\n\tdisplays the history output\n"
                << ".prompt <str>\n\tset the repl prompt to <str>\n";

            rx.history_add(input);
            continue;
        }
        else if (input.compare(0, 7, ".prompt") == 0)
        {
            // set the repl prompt text
            auto pos = input.find(" ");
            if (pos == std::string::npos)
            {
                std::cout << "Error: '.prompt' missing argument\n";
            }
            else
            {
                prompt = input.substr(pos + 1) + " ";
            }

            rx.history_add(input);
            continue;
        }
        else if (input.compare(0, 8, ".history") == 0)
        {
            // display the current history
            for (size_t i = 0, sz = rx.history_size(); i < sz; ++i)
            {
                std::cout << std::setw(4) << i << ": " << rx.history_line(i) << "\n";
            }

            rx.history_add(input);
            continue;
        }
        else if (input.compare(0, 6, ".clear") == 0)
        {
            // clear the screen
            rx.clear_screen();

            rx.history_add(input);
            continue;
        }
        else
        {
            // default action
            // echo the input
            // std::cout << input << "\n";

            args_converter cv = args_converter(input);

            int _argc = cv.get_argc();
            char **_argv = cv.get_argv();
            parse(_argc, _argv);
            // std::cout << _argc << std::endl;

            // for (int i = 0; i < _argc; i++)
            // {
            // 	std::cout << _argv[i] << std::endl;
            // }

            rx.history_add(input);
            continue;
        }
    }

    // save the history
    rx.history_save(history_file);

    std::cout << "\nExiting Replxx\n";
}

/**
 * TODO "入口函数"
 */
int main(int argc, char **argv)
{
    initLogger(argc, argv);
    // testLogger();
    runRepl();

    return 0;
}
