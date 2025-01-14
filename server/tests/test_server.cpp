#include <gtest/gtest.h>
#include "server.hpp"

class ServerTest : public ::testing::Test {
protected:
    Server* server;

    void SetUp() override {
        server = new Server(12345); 
    }

    void TearDown() override {
        delete server;
    }
};

TEST_F(ServerTest, ServerInitializationSuccess) {
    bool initResult = server->init_server();
    EXPECT_TRUE(initResult);
}

TEST_F(ServerTest, ServerInitializationFailure) {
    Server badServer(-1);  // Intentionally invalid port
    bool initResult = badServer.init_server();
    EXPECT_FALSE(initResult);
}
