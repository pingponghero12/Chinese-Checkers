// binding.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "client.hpp"

namespace py = pybind11;

/**
 * @brief Creates the Python module and defines the bindings
 * @param m The pybind11 module object to which the bindings will be added
 * @details This function creates a Python module named 'client_module' and
 *          exposes the Client class and its methods to Python. The following
 *          functionality is exposed:
 *          - Constructor taking server address and port
 *          - Connection management (connect, disconnect)
 *          - Message handling (send_message, set_message_callback)
 *          - Background receiving functionality (start_receiving)
 */
PYBIND11_MODULE(client_module, m) {
    py::class_<Client>(m, "Client")
        /**
         * @brief Exposes the Client constructor
         * @param address Server address as string
         * @param port Server port as integer
         */
        .def(py::init<const std::string&, int>())
        
        /**
         * @brief Exposes the connect_to_server method
         * @return Boolean indicating connection success
         */
        .def("connect", &Client::connect_to_server)
        
        /**
         * @brief Exposes the send_message method
         * @param message String message to send to server
         */
        .def("send_message", &Client::send_message)
        
        /**
         * @brief Exposes the disconnect method
         */
        .def("disconnect", &Client::disconnect)
        
        /**
         * @brief Exposes the start_receiving method for background message processing
         */
        .def("start_receiving", &Client::start_receiving)
        
        /**
         * @brief Exposes the set_message_callback method
         * @param callback Python callable to handle received messages
         */
        .def("set_message_callback", &Client::set_message_callback);
}