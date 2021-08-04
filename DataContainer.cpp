#include "DataContainer.h"

DataContainer::DataContainer()
{
	is_sfs_object_init = false;
	is_sfs_user_init = false;
	is_sfs_mmoitem_init = false;
	is_command_init = false;
	is_lists_init = false;

	event_name = "undefined";
}

void DataContainer::set_event_name(std::string name)
{
	event_name = name;
}

void DataContainer::set_sfs_object(boost::shared_ptr<SFSObject> data)
{
	sfs_object = data;
	is_sfs_object_init = true;
}

void DataContainer::set_sfs_user(boost::shared_ptr<User> user)
{
	sfs_user = user;
	is_sfs_user_init = true;
}

void DataContainer::set_sfs_mmoitem(boost::shared_ptr<MMOItem> item)
{
	sfs_mmoitem = item;
	is_sfs_mmoitem_init = true;
}

void DataContainer::set_lists(boost::shared_ptr<std::vector<boost::shared_ptr<User>>> add_users, 
	boost::shared_ptr<std::vector<boost::shared_ptr<User>>> remove_users, 
	boost::shared_ptr<std::vector<boost::shared_ptr<MMOItem>>> add_mmoitems,
	boost::shared_ptr<std::vector<boost::shared_ptr<MMOItem>>> remove_mmoitems)
{
	is_lists_init = true;

	data_add_users.resize(0);
	for (size_t i = 0; i < add_users->size(); i++)
	{
		DataContainer data;
		data.set_sfs_user(add_users->at(i));
		data_add_users.push_back(data);
	}

	data_remove_users.resize(0);
	for (size_t i = 0; i < remove_users->size(); i++)
	{
		DataContainer data;
		data.set_sfs_user(remove_users->at(i));
		data_remove_users.push_back(data);
	}

	data_add_mmoitems.resize(0);
	for (size_t i = 0; i < add_mmoitems->size(); i++)
	{
		DataContainer data;
		data.set_sfs_mmoitem(add_mmoitems->at(i));
		data_add_mmoitems.push_back(data);
	}

	data_remove_mmoitems.resize(0);
	for (size_t i = 0; i < remove_mmoitems->size(); i++)
	{
		DataContainer data;
		data.set_sfs_mmoitem(remove_mmoitems->at(i));
		data_remove_mmoitems.push_back(data);
	}
}

void DataContainer::set_command(string cmd)
{
	command = cmd;
	is_command_init = true;
}

int DataContainer::get_int(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<long> val_ptr = sfs_object->GetInt(key);
		
		return (int)(*val_ptr);
	}
	else
	{
		return 0;
	}
}

float DataContainer::get_float(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<float> val_ptr = sfs_object->GetFloat(key);

		return (float)(*val_ptr);
	}
	else
	{
		return 0.0f;
	}
}

double DataContainer::get_double(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<double> val_ptr = sfs_object->GetDouble(key);

		return (double)(*val_ptr);
	}
	else
	{
		return 0.0;
	}
}

bool DataContainer::get_bool(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<bool> val_ptr = sfs_object->GetBool(key);

		return (bool)(*val_ptr);
	}
	else
	{
		return false;
	}
}

string DataContainer::get_string(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<string> val_ptr = sfs_object->GetUtfString(key);

		return (string)(*val_ptr);
	}
	else
	{
		return "";
	}
}

unsigned char DataContainer::get_byte(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<unsigned char> val_ptr = sfs_object->GetByte(key);

		return (unsigned char)(*val_ptr);
	}
	else
	{
		return 0;
	}
}

std::vector<double> DataContainer::get_double_array(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<std::vector<double>> val_ptr = sfs_object->GetDoubleArray(key);
		return *val_ptr;
	}
	else
	{
		std::vector<double> to_return(0);
		return to_return;
	}
}

std::vector<DataContainer> DataContainer::get_sfs_array(string key)
{
	if (is_sfs_object_init)
	{
		boost::shared_ptr<ISFSArray> val_ptr = sfs_object->GetSFSArray(key);
		std::vector<DataContainer> to_return(0);
		for (size_t i = 0; i < val_ptr->Size(); i++)
		{
			boost::shared_ptr<ISFSObject> obj_ptr = val_ptr->GetSFSObject(i);
			DataContainer new_container;
			new_container.set_sfs_object(boost::static_pointer_cast<SFSObject>(obj_ptr));

			to_return.push_back(new_container);
		}
		return to_return;
	}
	else
	{
		std::vector<DataContainer> to_return(0);
		return to_return;
	}
}

string DataContainer::get_user_string(string key)
{
	if (is_sfs_user_init)
	{
		boost::shared_ptr<string> val_ptr = sfs_user->GetVariable(key)->GetStringValue();
		return val_ptr->c_str();
	}
	else
	{
		return "";
	}
}

int DataContainer::get_user_int(string key)
{
	if (is_sfs_user_init)
	{
		boost::shared_ptr<long> val_ptr = sfs_user->GetVariable(key)->GetIntValue();
		return (int)(*val_ptr);
	}
	else
	{
		return -1;
	}
}

float DataContainer::get_user_float(string key)
{
	if (is_sfs_user_init)
	{
		boost::shared_ptr<double> val_ptr = sfs_user->GetVariable(key)->GetDoubleValue();
		return (float)(*val_ptr);
	}
	else
	{
		return 0.0f;
	}
}

double DataContainer::get_user_double(string key)
{
	if (is_sfs_user_init)
	{
		boost::shared_ptr<double> val_ptr = sfs_user->GetVariable(key)->GetDoubleValue();
		return (double)(*val_ptr);
	}
	else
	{
		return 0.0;
	}
}

bool DataContainer::get_user_bool(string key)
{
	if (is_sfs_user_init)
	{
		boost::shared_ptr<bool> val_ptr = sfs_user->GetVariable(key)->GetBoolValue();
		return (bool)(*val_ptr);
	}
	else
	{
		return false;
	}
}

int DataContainer::get_user_id()
{
	if (is_sfs_user_init)
	{
		return sfs_user->Id();
	}
	else
	{
		return -1;
	}
}

string DataContainer::get_mmoitem_string(string key)
{
	if (is_sfs_mmoitem_init)
	{
		boost::shared_ptr<string> val_ptr = sfs_mmoitem->GetVariable(key)->GetStringValue();
		return val_ptr->c_str();
	}
	else
	{
		return "";
	}
}

int DataContainer::get_mmoitem_int(string key)
{
	if (is_sfs_mmoitem_init)
	{
		boost::shared_ptr<long> val_ptr = sfs_mmoitem->GetVariable(key)->GetIntValue();
		return (int)(*val_ptr);
	}
	else
	{
		return -1;
	}
}

float DataContainer::get_mmoitem_float(string key)
{
	if (is_sfs_mmoitem_init)
	{
		boost::shared_ptr<double> val_ptr = sfs_mmoitem->GetVariable(key)->GetDoubleValue();
		return (float)(*val_ptr);
	}
	else
	{
		return 0.0f;
	}
}

double DataContainer::get_mmoitem_double(string key)
{
	if (is_sfs_mmoitem_init)
	{
		boost::shared_ptr<double> val_ptr = sfs_mmoitem->GetVariable(key)->GetDoubleValue();
		return (double)(*val_ptr);
	}
	else
	{
		return 0.0;
	}
}

std::vector<DataContainer> DataContainer::get_list_add_users()
{
	if (is_lists_init)
	{
		return data_add_users;
	}
	else
	{
		std::vector<DataContainer> to_return(0);
		return to_return;
	}
}

std::vector<DataContainer> DataContainer::get_list_remove_users()
{
	if (is_lists_init)
	{
		return data_remove_users;
	}
	else
	{
		std::vector<DataContainer> to_return(0);
		return to_return;
	}
}

std::vector<DataContainer> DataContainer::get_list_add_mmoitems()
{
	if (is_lists_init)
	{
		return data_add_mmoitems;
	}
	else
	{
		std::vector<DataContainer> to_return(0);
		return to_return;
	}
}

std::vector<DataContainer> DataContainer::get_list_remove_mmoitems()
{
	if (is_lists_init)
	{
		return data_remove_mmoitems;
	}
	else
	{
		std::vector<DataContainer> to_return(0);
		return to_return;
	}
}

string DataContainer::get_command()
{
	if (is_command_init)
	{
		return command;
	}
	else
	{
		return "";
	}
}

string DataContainer::get_event_name()
{
	return event_name;
}

boost::shared_ptr<SFSObject> DataContainer::get_sfs_object()
{
	return sfs_object;
}

bool DataContainer::get_is_sfs_init()
{
	return is_sfs_object_init;
}

void DataContainer::put_byte(string key, unsigned char value)
{
	if (!is_sfs_object_init)
	{
		sfs_object = boost::shared_ptr<SFSObject>(new SFSObject());
		is_sfs_object_init = true;
	}
	sfs_object->PutByte(key, value);
}

void DataContainer::put_float(string key, float value)
{
	if (!is_sfs_object_init)
	{
		sfs_object = boost::shared_ptr<SFSObject>(new SFSObject());
		is_sfs_object_init = true;
	}
	sfs_object->PutFloat(key, value);
}

void DataContainer::put_bool(string key, bool value)
{
	if (!is_sfs_object_init)
	{
		sfs_object = boost::shared_ptr<SFSObject>(new SFSObject());
		is_sfs_object_init = true;
	}
	sfs_object->PutBool(key, value);
}

DataContainer::~DataContainer()
{
	
}