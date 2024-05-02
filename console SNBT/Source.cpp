#include "main.h"

std::string parameter{ "" };
std::string choosenProject{ "" };

//commands
struct abstract_command {
    virtual void execute() = 0;
    virtual ~abstract_command() = default;
};

struct makeProject : abstract_command {
    void execute() override {
        makeProjectFunc();
    }
};

struct info : abstract_command {
    void execute() override {
        dispalyInfo();
    }
};

struct chooseProject : abstract_command {
    void execute() override {
        chooseProjectFunc();
    }
};

struct listProject : abstract_command {
    void execute() override {
        listProjectFunc();
    }
};

struct closeProject : abstract_command {
    void execute() override {
        closeProjectFunc();
    }
};

struct unknown : abstract_command {
    void execute() override {
        std::cerr << "unknown command\n";
    }
};

struct init : abstract_command {
    void execute() override {
        initFunc();
    }
};

struct build : abstract_command {
    void execute() override {
        buildFunc();
    }
};

struct addAPI : abstract_command {
    void execute() override {
       addAPIFunc();
    }
};

struct translate : abstract_command {
    void execute() override {
        translateFunc();
    }
};

int main() {

    setlocale(LC_ALL, "ru");

    std::unordered_map<std::string, abstract_command*> commands{
        {"info",new info},
        {"makeProject", new makeProject},
        {"chooseProject", new chooseProject},
        {"list", new listProject},
        {"closeProject", new closeProject},
        {"init", new init},
        {"build", new build},
        {"addAPI", new addAPI},
        {"translate", new translate},
        {"unknown", new unknown}
    };

    dispalyInfo();

    std::string cmd = "";

    for (;;) {

        std::cin >> cmd;
        std::getline(std::cin, parameter);

        if (cmd == "exit")
            return 0;

        if (commands.find(cmd) == commands.end())
            cmd = "unknown";

        commands[cmd]->execute();
    }


    return 0;
}
