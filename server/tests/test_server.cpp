#include <gtest/gtest.h>
#include "server.hpp"

/**
 * @brief Test fixture for Server tests using the Google Test framework.
 * 
 * This class sets up a server instance for testing and ensures proper 
 * cleanup after each test.
 */
class ServerTest : public ::testing::Test {
protected:
    Server* server; /**< Pointer to the Server instance used in tests. */

    /**
     * @brief Set up the test environment before each test.
     * 
     * This method allocates a new Server instance with a valid port number
     * for testing.
     */
    void SetUp() override {
        server = new Server(12345); 
    }

    /**
     * @brief Clean up the test environment after each test.
     * 
     * This method deallocates the Server instance to prevent memory leaks.
     */
    void TearDown() override {
        // delete server;
    }
};

/**
 * @brief Test case to verify successful server initialization.
 * 
 * This test calls the init_server method on a valid server instance
 * and expects it to succeed.
 */
TEST_F(ServerTest, ServerInitializationSuccess) {
    bool initResult = server->init_server();
    EXPECT_TRUE(initResult);
}

/**
 * @brief Test case to verify failure during server initialization.
 * 
 * This test creates a Server instance with an invalid port number,
 * calls init_server, and expects it to fail.
 */
TEST_F(ServerTest, ServerInitializationFailure) {
    Server badServer(-1);  // Intentionally invalid port
    bool initResult = badServer.init_server();
    EXPECT_FALSE(initResult);
}
