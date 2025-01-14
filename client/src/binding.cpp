// binding.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "client.hpp"

namespace py = pybind11;

PYBIND11_MODULE(client_module, m) {
    py::class_<Client>(m, "Client")
        .def(py::init<const std::string&, int>())
        .def("connect", &Client::connect_to_server)
        .def("send_message", &Client::send_message)
        .def("disconnect", &Client::disconnect)
        .def("start_receiving", &Client::start_receiving)
        .def("set_message_callback", &Client::set_message_callback);
}
