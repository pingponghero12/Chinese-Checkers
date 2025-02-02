#ifndef __ABSTRACTCOMMAND__
#define __ABSTRACTCOMMAND__

#include <vector>
/**
 * @brief Abstract class for commands
 * Executing a command is done by calling the execute function
 */
class AbstractCommand {
public:
    virtual ~AbstractCommand() = default;
    virtual void execute(const std::vector<int>& args, int client_id) = 0;
};

#endif // __ABSTRACTCOMMAND__

