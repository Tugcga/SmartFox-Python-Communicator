#pragma once

#include <iostream>
#include "DataContainer.h"
#include "Requests/LoginRequest.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/PublicMessageRequest.h"
#include <map>

class SFSClient
{
public:
	SFSClient();
	~SFSClient();

	void connect(string host, int port, string zone);
	void disconnect();
	void login(string name, string password, string zone, DataContainer data);
	std::vector<DataContainer> update();  // return the number of event datas in the stack
	int get_my_id();  // return id of the current connection
	void send_command(const char* command, DataContainer data);
	void send_public_message(const char* message);
	void set_room(boost::shared_ptr<Room> _room);

	static void on_connection(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_connection_lost(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_login(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_room_join(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_user_variable_update(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_mmoitem_variable_update(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_proximity_list_update(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_extension_response(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_admin_message(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_public_message(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	static void on_private_message(unsigned long long context, boost::shared_ptr<BaseEvent> evt);
	
private:
	boost::shared_ptr<Sfs2X::SmartFox> sfs;
	boost::shared_ptr<Room> room;
	std::vector<DataContainer> event_datas;
	bool init_callbacks;
};