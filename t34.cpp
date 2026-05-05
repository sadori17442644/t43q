#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <memory>
#include <unordered_map>
#include <fstream>

class Command {
public:
    virtual void execute() = 0;
    virtual void undo() {} 
    virtual std::string getName() const { return "Unknown Command"; }
    virtual ~Command() = default;
};

class HelloCommand : public Command {
public:
    void execute() override {
        std::cout << "Hello, World!\n";
    }
    std::string getName() const override { return "HelloCommand"; }
};

class Light {
public:
    void turnOn() {
        std::cout << "Light is ON\n";
    }
    void turnOff() {
        std::cout << "Light is OFF\n";
    }
};

class TurnOnCommand : public Command {
private:
    Light& light;
public:
    TurnOnCommand(Light& l) : light(l) {}
    void execute() override {
        light.turnOn();
    }
    std::string getName() const override { return "TurnOnCommand"; }
};

class TurnOffCommand : public Command {
private:
    Light& light;
public:
    TurnOffCommand(Light& l) : light(l) {}
    void execute() override {
        light.turnOff();
    }
    void undo() override {
        light.turnOn(); 
    }
    std::string getName() const override { return "TurnOffCommand"; }
};

class Button {
private:
    Command* command;
public:
    Button() : command(nullptr) {}
    void setCommand(Command* cmd) {
        command = cmd;
    }
    void press() {
        if (command) {
            command->execute();
        }
    }
};


class Player {
private:
    int x, y;
public:
    Player() : x(0), y(0) {}
    void moveUp() { y++; std::cout << "Player moved up to (" << x << ", " << y << ")\n"; }
    void moveDown() { y--; std::cout << "Player moved down to (" << x << ", " << y << ")\n"; }
    void moveLeft() { x--; std::cout << "Player moved left to (" << x << ", " << y << ")\n"; }
    void moveRight() { x++; std::cout << "Player moved right to (" << x << ", " << y << ")\n"; }

    void undoMoveUp() { y--; }
    void undoMoveDown() { y++; }
    void undoMoveLeft() { x++; }
    void undoMoveRight() { x--; }
};

class MoveUpCommand : public Command {
private:
    Player& player;
public:
    MoveUpCommand(Player& p) : player(p) {}
    void execute() override { player.moveUp(); }
    void undo() override { player.undoMoveUp(); }
    std::string getName() const override { return "MoveUpCommand"; }
};

class MoveDownCommand : public Command {
private:
    Player& player;
public:
    MoveDownCommand(Player& p) : player(p) {}
    void execute() override { player.moveDown(); }
    void undo() override { player.undoMoveDown(); }
    std::string getName() const override { return "MoveDownCommand"; }
};

class MoveLeftCommand : public Command {
private:
    Player& player;
public:
    MoveLeftCommand(Player& p) : player(p) {}
    void execute() override { player.moveLeft(); }
    void undo() override { player.undoMoveLeft(); }
    std::string getName() const override { return "MoveLeftCommand"; }
};

class MoveRightCommand : public Command {
private:
    Player& player;
public:
    MoveRightCommand(Player& p) : player(p) {}
    void execute() override { player.moveRight(); }
    void undo() override { player.undoMoveRight(); }
    std::string getName() const override { return "MoveRightCommand"; }
};

class AttackCommand : public Command {
public:
    void execute() override { std::cout << "Hero attacks!\n"; }
    std::string getName() const override { return "AttackCommand"; }
};

class HealCommand : public Command {
public:
    void execute() override { std::cout << "Hero heals!\n"; }
    std::string getName() const override { return "HealCommand"; }
};

class DefendCommand : public Command {
public:
    void execute() override { std::cout << "Hero defends!\n"; }
    std::string getName() const override { return "DefendCommand"; }
};

class SmartHomeCommand : public Command {
private:
    std::string action;
public:
    SmartHomeCommand(const std::string& act) : action(act) {}
    void execute() override { std::cout << action << "\n"; }
    std::string getName() const override { return action; }
};

class FileCommand : public Command {
private:
    std::string operation;
public:
    FileCommand(const std::string& op) : operation(op) {}
    void execute() override { std::cout << "File operation: " << operation << "\n"; }
    std::string getName() const override { return operation; }
};

void executeQueue(std::queue<Command*>& commandQueue) {
    while (!commandQueue.empty()) {
        Command* cmd = commandQueue.front();
        cmd->execute();
        commandQueue.pop();
    }
}

class CommandManager {
private:
    std::stack<Command*> history;
    std::stack<Command*> redoStack;
public:
    void executeCommand(Command* cmd) {
        std::cout << "[LOG] Executing: " << cmd->getName() << "\n";
        cmd->execute();
        history.push(cmd);
        while (!redoStack.empty()) redoStack.pop(); // Очищаем redo при новой команде
    }

    void undoLastCommand() {
        if (!history.empty()) {
            Command* cmd = history.top();
            std::cout << "[LOG] Undoing: " << cmd->getName() << "\n";
            cmd->undo();
            redoStack.push(cmd);
            history.pop();
        } else {
            std::cout << "Nothing to undo.\n";
        }
    }

    void redoLastCommand() {
        if (!redoStack.empty()) {
            Command* cmd = redoStack.top();
            std::cout << "[LOG] Redoing: " << cmd->getName() << "\n";
            cmd->execute();
            history.push(cmd);
            redoStack.pop();
        } else {
            std::cout << "Nothing to redo.\n";
        }
    }
};


class MacroCommand : public Command {
private:
    std::vector<Command*> commands;
public:
    void addCommand(Command* cmd) {
        commands.push_back(cmd);
    }
    void execute() override {
        for (auto cmd : commands) {
            cmd->execute();
        }
    }
    void undo() override {
        for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
            (*it)->undo();
        }
    }
    std::string getName() const override { return "MacroCommand"; }
};

void demoUniquePtrCommands() {
void demoUniquePtrCommands() {
    std::unique_ptr<Command> helloCmd = std::make_unique<HelloCommand>();
    std::unique_ptr<Command> attackCmd = std::make_unique<AttackCommand>();

    helloCmd->execute();
    attackCmd->execute();
}

class CommandFactory {
public:
    static std::unique_ptr<Command> createCommand(const std::string& type, Player* player = nullptr, Light* light = nullptr) {
        if (type == "attack") {
            return std::make_unique<AttackCommand>();
        } else if (type == "heal") {
            return std::make_unique<HealCommand>();
        } else if (type == "defend") {
            return std::make_unique<DefendCommand>();
        } else if (type == "moveUp" && player) {
            return std::make_unique<MoveUpCommand>(*player);
        } else if (type == "moveDown" && player) {
            return std::make_unique<MoveDownCommand>(*player);
        } else if (type == "moveLeft" && player) {
            return std::make_unique<MoveLeftCommand>(*player);
        } else if (type == "moveRight" && player) {
            return std::make_unique<MoveRightCommand>(*player);
        } else if (type == "turnOn" && light) {
            return std::make_unique<TurnOnCommand>(*light);
        } else if (type == "turnOff" && light) {
            return std::make_unique<TurnOffCommand>(*light);
        }
        return nullptr;
    }
};

void miniProject() {
    Player player;
    Light light;
    CommandManager manager;

    char input;
    std::cout << "Mini-Game: Control the hero!\n";
    std::cout << "Commands: w (up), s (down), a (left), d (right), u (undo), r (redo), q (quit)\n";

    while (true) {
        std::cout << "\nEnter command: ";
        std::cin >> input;

        std::unique_ptr<Command> cmd;

        switch (input) {
            case 'w':
                cmd = CommandFactory::createCommand("moveUp", &player);
                manager.executeCommand(cmd.get());
                break;
            case 's':
                cmd = CommandFactory::createCommand("moveDown", &player);
                manager.executeCommand(cmd.get());
                break;
            case 'a':
                cmd = CommandFactory::createCommand("moveLeft", &player);
                manager.executeCommand(cmd.get());
                break;
            case 'd':
                cmd = CommandFactory::createCommand("moveRight", &player);
                manager.executeCommand(cmd.get());
                break;
            case 'u':
                manager.undoLastCommand();
                break;
            case 'r':
                manager.redoLastCommand();
                break;
            case 'q':
                std::cout << "Game over.\n";
                return;
            default:
                std::cout << "Unknown command!\n";
        }
    }
}

int main() {
    std::cout << "=== Block 1: Basic Understanding ===\n";
    HelloCommand helloCmd;
    helloCmd.execute();

    Light light;
    TurnOnCommand turnOnCmd(light);
    TurnOffCommand turnOffCmd(light);

    Button button;
    button.setCommand(&turnOnCmd);
    button.press(); 

    button.setCommand(&turnOffCmd);
    button.press(); 

    Button buttons[3];
    buttons[0].setCommand(&helloCmd);
    buttons[1].setCommand(&turnOnCmd);
    buttons[2].setCommand(&turnOffCmd);

    for (int i = 0; i < 3; ++i) {
        buttons[i].press();
    }

    std::cout << "\n=== Block 2: Practical Cases ===\n";
    Player player;

    MoveUpCommand moveUpCmd(player);
    MoveDownCommand moveDownCmd(player);
    MoveLeftCommand moveLeftCmd(player);
    MoveRightCommand moveRightCmd(player);

    moveUpCmd.execute();
    moveRightCmd.execute();

    AttackCommand attackCmd;
    HealCommand healCmd;
    DefendCommand defendCmd;

    attackCmd.execute();
    healCmd.execute();

    SmartHomeCommand turnOnLight("Light turned ON");
    SmartHomeCommand turnOnAC("AC turned ON");
    turnOnLight.execute();
    turnOnAC.execute();

    FileCommand createFile("Create file");
    FileCommand deleteFile("Delete file");
    createFile.execute();
    deleteFile.execute();

    std::queue<Command*> commandQueue;
    commandQueue.push(&moveUpCmd);
    commandQueue.push(&attackCmd);
    commandQueue.push(&createFile);
    executeQueue(commandQueue);

    std::cout << "\n=== Block 3: Undo/Redo ===\n";
    CommandManager manager;

    manager.executeCommand(&moveUpCmd);
    manager.executeCommand(&moveRightCmd);
    manager.executeCommand(&attackCmd);
    manager.undoLastCommand(); 
    manager.redoLastCommand();  

    manager.executeCommand(&moveLeftCmd);
    manager.executeCommand(&moveDownCmd);
    manager.undoLastCommand();
    manager.undoLastCommand();

    std::cout << "\n=== Block 4: Advanced Level ===\n";
    MacroCommand macroCmd;
    macroCmd.addCommand(&helloCmd);
    macroCmd.addCommand(&attackCmd);
    macroCmd.execute();
    macroCmd.undo();

    demoUniquePtrCommands();

    auto factoryCmd = CommandFactory::createCommand("heal");
    if (factoryCmd) {
        factoryCmd->execute();
    }

    std::cout << "\n=== Mini-Project: Console Game ===\n";
    miniProject();

    return 0;
}
