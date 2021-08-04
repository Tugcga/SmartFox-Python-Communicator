#pragma once

#include "SmartFox.h"
#include "Entities/MMOItem.h"

//data container contains all kind of data, which we can get from event:
//- SFSObject
//- User
//- MMOItem
//- array whith Users and MMOItems
class DataContainer
{
public:
	DataContainer();
	~DataContainer();

	void set_event_name(std::string name);

	void set_sfs_object(boost::shared_ptr<SFSObject> data);
	void set_sfs_user(boost::shared_ptr<User> user);
	void set_sfs_mmoitem(boost::shared_ptr<MMOItem> item);
	void set_lists(boost::shared_ptr<std::vector<boost::shared_ptr<User>>> add_users,
					boost::shared_ptr<std::vector<boost::shared_ptr<User>>> remove_users,
					boost::shared_ptr<std::vector<boost::shared_ptr<MMOItem>>> add_mmoitems,
					boost::shared_ptr<std::vector<boost::shared_ptr<MMOItem>>> remove_mmoitems);
	void set_command(string cmd);  // when set external command, also set parameters of this command

	int get_int(string key);
	float get_float(string key);
	double get_double(string key);
	bool get_bool(string key);
	unsigned char get_byte(string key);
	string get_string(string key);
	std::vector<double> get_double_array(string key);
	std::vector<DataContainer> get_sfs_array(string key);

	string get_user_string(string key);
	int get_user_int(string key);
	float get_user_float(string key);
	double get_user_double(string key);
	bool get_user_bool(string key);
	int get_user_id();

	string get_mmoitem_string(string key);
	int get_mmoitem_int(string key);
	float get_mmoitem_float(string key);
	double get_mmoitem_double(string key);

	std::vector<DataContainer> get_list_add_users();
	std::vector<DataContainer> get_list_remove_users();
	std::vector<DataContainer> get_list_add_mmoitems();
	std::vector<DataContainer> get_list_remove_mmoitems();

	void put_byte(string key, unsigned char value);
	void put_float(string key, float value);
	void put_bool(string key, bool value);

	string get_command();

	string get_event_name();

	boost::shared_ptr<SFSObject> get_sfs_object();
	bool get_is_sfs_init();

private:
	boost::shared_ptr<SFSObject> sfs_object;
	bool is_sfs_object_init;

	boost::shared_ptr<User> sfs_user;
	bool is_sfs_user_init;

	boost::shared_ptr<MMOItem> sfs_mmoitem;
	bool is_sfs_mmoitem_init;

	std::vector<DataContainer> data_add_users;
	std::vector<DataContainer> data_remove_users;
	std::vector<DataContainer> data_add_mmoitems;
	std::vector<DataContainer> data_remove_mmoitems;
	bool is_lists_init;

	std::string command;
	bool is_command_init;

	std::string event_name;
};