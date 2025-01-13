// binding.cpp
#include <pybind11/pybind11.h>
#include "client.hpp"

namespace py = pybind11;

PYBIND11_MODULE(client_module, m) {
    py::class_<Client>(m, "Client")
        .def(py::init<const std::string&, int>())
        .def("connect", &Client::connect_to_server)
        .def("send_message", &Client::send_message)
        .def("disconnect", &Client::disconnect);
}
