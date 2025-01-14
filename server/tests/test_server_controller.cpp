#include <gtest/gtest.h>
#include <string>
#include <map>
#include <vector>
#include "server_controller.hpp"
#include "server.hpp"

/**
 * @brief Mock implementation of the Server class for testing purposes.
 * 
 * This class overrides the Server methods to allow for easier testing by
 * capturing messages sent to clients without establishing actual network connections.
 */
class MockServer : public Server {
public:
    MockServer(int port = 12345) : Server(port) {}

    /**
     * @brief Override the init_server method for testing.
     * 
     * This mock implementation always returns true to bypass actual server initialization.
     * 
     * @return Always returns true.
     */
    bool init_server() override { return true; }

    /**
     * @brief Override the start_server method for testing.
     * 
     * This mock implementation does nothing to avoid starting a real server.
     */
    void start_server() override {}

    /**
     * @brief Send a message to a mock client.
     * 
     * This method captures messages sent to a specific client by appending them
     * to the sentMessages map for verification in tests.
     *
     * @param message The message to be sent.
     * @param client_id The ID of the client receiving the message.
     */
    void send_message(const std::string& message, const int& client_id) override {
        sentMessages[client_id].push_back(message);
    }

    // Map to track messages sent to each client for testing.
    std::map<int, std::vector<std::string>> sentMessages;
};

/**
 * @brief Test fixture for ServerController tests using Google Test framework.
 * 
 * This class sets up the necessary mock server and controller instances for each test.
 */
class ServerControllerTest : public ::testing::Test {
protected:
    // We use a heap-allocated MockServer and ServerController to reset state each test.
    MockServer* mockServer; /**< Pointer to the mock server instance used in tests. */
    ServerController* controller; /**< Pointer to the ServerController instance. */

    /**
     * @brief Set up the test environment before each test.
     * 
     * This method allocates instances of MockServer and ServerController and
     * initializes player statuses for testing.
     */
    void SetUp() override {
        mockServer = new MockServer();
        controller = new ServerController(mockServer);

        controller->update_player_status(10, -1);
        controller->update_player_status(20, -1);
    }

    /**
     * @brief Clean up the test environment after each test.
     * 
     * This method deallocates the instances of ServerController and MockServer.
     */
    void TearDown() override {
        delete controller;
        delete mockServer;
    }
};

/**
 * @brief Test case to verify that an empty message results in no action.
 */
TEST_F(ServerControllerTest, ParseCall_EmptyMessage_NoAction) {
    controller->parse_call("", 10);  
    auto it = mockServer->sentMessages.find(10);
    EXPECT_TRUE(it == mockServer->sentMessages.end());
}

/**
 * @brief Test case to verify that an invalid command sends an error message.
 */
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

/**
 * @brief Test case to verify that an empty string is handled gracefully.
 */
TEST_F(ServerControllerTest, ParseCall_EmptyString_HandledGracefully) {
    controller->parse_call("", 10);
}

/**
 * @brief Test case to verify that non-numeric input is handled gracefully.
 */
TEST_F(ServerControllerTest, ParseCall_NonNumericInput_HandledGracefully) {
    controller->parse_call("abc", 10);
}

/**
 * @brief Test case for verifying a valid command format.
 */
TEST_F(ServerControllerTest, ParseCall_ValidCommandFormat) {
    controller->parse_call("1 2 3", 10);  // Valid command with arguments
}

/**
 * @brief Test case to verify that a main command while in a game sends the correct message.
 */
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

/**
 * @brief Test case to verify that sending a non-main command while not in the game
 * sends the correct message.
 */
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

/**
 * @brief Test case to verify the functionality of updating and getting player status.
 */
TEST_F(ServerControllerTest, UpdateAndGetPlayerStatus) {
    EXPECT_EQ(controller->get_player_status(10), -1);

    controller->update_player_status(10, 999);
    EXPECT_EQ(controller->get_player_status(10), 999);
}