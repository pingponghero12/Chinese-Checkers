#include <gtest/gtest.h>
#include <string>
#include <map>
#include <vector>
#include "server_controller.hpp"
#include "server.hpp"

class MockServer : public Server {
public:
    MockServer(int port = 12345) : Server(port) {}

    bool init_server() override { return true; }
    void start_server() override {}

    void send_message(const std::string& message, const int& client_id) override {
        sentMessages[client_id].push_back(message);
    }

    std::map<int, std::vector<std::string>> sentMessages;

};

class ServerControllerTest : public ::testing::Test {
protected:
    // We use a heap-allocated MockServer and ServerController to reset state each test.
    MockServer* mockServer;
    ServerController* controller;

    void SetUp() override {
        mockServer = new MockServer();
        controller = new ServerController(mockServer);

        controller->update_player_status(10, -1);
        controller->update_player_status(20, -1);
    }

    void TearDown() override {
        delete controller;
        delete mockServer;
    }
};

TEST_F(ServerControllerTest, ParseCall_EmptyMessage_NoAction) {
    controller->parse_call("", 10);  
    auto it = mockServer->sentMessages.find(10);
    EXPECT_TRUE(it == mockServer->sentMessages.end());
}

TEST_F(ServerControllerTest, ParseCall_InvalidCommand_SendsError) {
    controller->parse_call("999 ", 10);

    auto it = mockServer->sentMessages.find(10);
    ASSERT_TRUE(it != mockServer->sentMessages.end());

    bool foundError = false;
    for (const auto& msg : it->second) {
        if (msg.find("Command not found") != std::string::npos) {
            foundError = true;
            break;
        }
    }
    EXPECT_TRUE(foundError);
}

TEST_F(ServerControllerTest, ParseCall_EmptyString_HandledGracefully) {
    controller->parse_call("", 10);
}

TEST_F(ServerControllerTest, ParseCall_NonNumericInput_HandledGracefully) {
    controller->parse_call("abc", 10);
}

TEST_F(ServerControllerTest, ParseCall_ValidCommandFormat) {
    controller->parse_call("1 2 3", 10);  // Valid command with arguments
}

TEST_F(ServerControllerTest, ParseCall_MainCommandWhileInGame) {
    controller->update_player_status(10, 1);

    controller->parse_call("1 2", 10);

    auto it = mockServer->sentMessages.find(10);
    ASSERT_TRUE(it != mockServer->sentMessages.end());
    bool foundInGameMsg = false;
    for (const auto& msg : it->second) {
        if (msg.find("You are currently in game") != std::string::npos) {
            foundInGameMsg = true;
            break;
        }
    }
    EXPECT_TRUE(foundInGameMsg);
}

TEST_F(ServerControllerTest, ParseCall_NonMainCommandNotInGame) {
    controller->parse_call("4 1 2 3 4", 20);

    auto it = mockServer->sentMessages.find(20);
    ASSERT_TRUE(it != mockServer->sentMessages.end());
    bool foundNotInGameMsg = false;
    for (const auto& msg : it->second) {
        if (msg.find("You are not in game") != std::string::npos) {
            foundNotInGameMsg = true;
            break;
        }
    }
    EXPECT_TRUE(foundNotInGameMsg);
}

TEST_F(ServerControllerTest, UpdateAndGetPlayerStatus) {
    EXPECT_EQ(controller->get_player_status(10), -1);

    controller->update_player_status(10, 999);
    EXPECT_EQ(controller->get_player_status(10), 999);
}
