// Smartfox_Communicator.cpp : Defines the entry point for the console application.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SFSClient.h"

namespace py = pybind11;

PYBIND11_MODULE(SFSCommunicator, m)
{
	py::class_<SFSClient>(m, "SFSClient")
		.def(py::init<>())
		.def("update", &SFSClient::update)
		.def("connect", &SFSClient::connect)
		.def("disconnect", &SFSClient::disconnect)
		.def("login", &SFSClient::login)
		.def("send_command", &SFSClient::send_command)
		.def("get_my_id", &SFSClient::get_my_id)
		.def("get_event_datas", &SFSClient::get_event_datas);

	py::class_<DataContainer>(m, "DataContainer")
		.def(py::init<>())
		.def("get_int", &DataContainer::get_int)
		.def("get_float", &DataContainer::get_float)
		.def("get_double", &DataContainer::get_double)
		.def("get_bool", &DataContainer::get_bool)
		.def("get_string", &DataContainer::get_string)
		.def("get_byte", &DataContainer::get_byte)
		.def("get_double_array", &DataContainer::get_double_array)
		.def("get_sfs_array", &DataContainer::get_sfs_array)
		.def("get_user_string", &DataContainer::get_user_string)
		.def("get_user_int", &DataContainer::get_user_int)
		.def("get_user_float", &DataContainer::get_user_float)
		.def("get_user_double", &DataContainer::get_user_double)
		.def("get_user_bool", &DataContainer::get_user_bool)
		.def("get_user_id", &DataContainer::get_user_id)
		.def("get_list_add_users", &DataContainer::get_list_add_users)
		.def("get_list_remove_users", &DataContainer::get_list_remove_users)
		.def("get_list_add_mmoitems", &DataContainer::get_list_add_mmoitems)
		.def("get_list_remove_mmoitems", &DataContainer::get_list_remove_mmoitems)
		.def("get_mmoitem_string", &DataContainer::get_mmoitem_string)
		.def("get_mmoitem_int", &DataContainer::get_mmoitem_int)
		.def("get_mmoitem_float", &DataContainer::get_mmoitem_float)
		.def("get_mmoitem_double", &DataContainer::get_mmoitem_double)
		.def("get_event_name", &DataContainer::get_event_name)
		.def("get_command", &DataContainer::get_command)
		.def("put_byte", &DataContainer::put_byte)
		.def("put_float", &DataContainer::put_float)
		.def("put_bool", &DataContainer::put_bool);
}

