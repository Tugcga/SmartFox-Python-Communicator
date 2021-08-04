#include "SFSClient.h"

SFSClient::SFSClient()
{
	sfs = boost::shared_ptr<Sfs2X::SmartFox>(new Sfs2X::SmartFox(false));
	room = boost::shared_ptr<Room>();
	event_datas.resize(0);
	init_callbacks = false;
}

SFSClient::~SFSClient()
{
	disconnect();
}

void SFSClient::connect(string host, int port, string zone)
{
	if (init_callbacks)
	{
		delete sfs.get();
		sfs = boost::shared_ptr<Sfs2X::SmartFox>(new Sfs2X::SmartFox(false));
		init_callbacks = false;
	}

	if (!init_callbacks)
	{
		sfs->ThreadSafeMode(false);

		sfs->AddEventListener(SFSEvent::CONNECTION, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_connection, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::CONNECTION_LOST, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_connection_lost, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::LOGIN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_login, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::ROOM_JOIN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_room_join, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::USER_VARIABLES_UPDATE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_user_variable_update, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::MMOITEM_VARIABLES_UPDATE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_mmoitem_variable_update, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::PROXIMITY_LIST_UPDATE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_proximity_list_update, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::EXTENSION_RESPONSE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_extension_response, (unsigned long long)this)));
		sfs->AddEventListener(SFSEvent::ADMIN_MESSAGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSClient::on_admin_message, (unsigned long long)this)));

		init_callbacks = true;
	}

	boost::shared_ptr<ConfigData> cfgData = sfs->Config();
	cfgData->Host(host);
	cfgData->Port(port);
	cfgData->Zone(zone);
	cfgData->ForceIPv6(false);

	// Connect to SmartFoxServer
	sfs->Connect(cfgData);
}

void SFSClient::disconnect()
{
	sfs->Disconnect();
}

size_t SFSClient::update()
{
	sfs->ProcessEvents();
	return event_datas.size();
}

int SFSClient::get_my_id()
{
	if (sfs->IsConnected())
	{
		return sfs->MySelf()->Id();
	}
	else
	{
		return -1;
	}
}

void SFSClient::login(string name, string password, string zone, DataContainer data)
{
	boost::shared_ptr<IRequest> request(new LoginRequest(name, password, zone, data.get_sfs_object()));
	sfs->Send(request);
}

std::vector<DataContainer> SFSClient::get_event_datas()
{
	// create copy
	std::vector<DataContainer> to_return(0);
	for (size_t i = 0; i < event_datas.size(); i++)
	{
		to_return.push_back(event_datas[i]);
	}
	//clear original buffer
	event_datas.clear();
	event_datas.resize(0);
	return to_return;
}

void SFSClient::send_command(const char* command, DataContainer data)
{
	if (data.get_is_sfs_init())
	{
		boost::shared_ptr<IRequest> request(new ExtensionRequest(command, data.get_sfs_object(), room));
		sfs->Send(request);
	}
}

void SFSClient::on_connection(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();
	DataContainer containter;

	boost::shared_ptr<void> prm_success_ptr = (*event_params)["success"];
	boost::shared_ptr<bool> prm_success = (boost::static_pointer_cast<bool>)(prm_success_ptr);
	boost::shared_ptr<SFSObject> data(new SFSObject());
	data->PutBool("success", *prm_success);
	containter.set_sfs_object(data);

	containter.set_event_name("connection");
	client->event_datas.push_back(containter);
}

void SFSClient::on_connection_lost(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	DataContainer containter;

	containter.set_event_name("connection_lost");
	client->event_datas.push_back(containter);
}

void SFSClient::on_login(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();
	DataContainer containter;

	boost::shared_ptr<void> prm_user_ptr = (*event_params)["user"];
	boost::shared_ptr<User> prm_user = (boost::static_pointer_cast<User>)(prm_user_ptr);
	containter.set_sfs_user(prm_user);

	boost::shared_ptr<void> prm_data_ptr = (*event_params)["data"];
	boost::shared_ptr<SFSObject> prm_data = (boost::static_pointer_cast<SFSObject>)(prm_data_ptr);
	containter.set_sfs_object(prm_data);

	containter.set_event_name("login");
	client->event_datas.push_back(containter);
}

void SFSClient::set_room(boost::shared_ptr<Room> _room)
{
	room = _room;
}

void SFSClient::on_room_join(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();
	boost::shared_ptr<void> prm_room_ptr = (*event_params)["room"];
	boost::shared_ptr<Room> prm_room = ((boost::static_pointer_cast<Room>))(prm_room_ptr);

	client->set_room(prm_room);

	DataContainer containter;
	containter.set_event_name("join_room");
	client->event_datas.push_back(containter);
}

void SFSClient::on_user_variable_update(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();

	boost::shared_ptr<void> prm_user_ptr = (*event_params)["user"];
	boost::shared_ptr<User> prm_user = (boost::static_pointer_cast<User>)(prm_user_ptr);
	DataContainer containter;
	containter.set_sfs_user(prm_user);
	containter.set_event_name("user_update");
	client->event_datas.push_back(containter);
}

void SFSClient::on_mmoitem_variable_update(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();

	boost::shared_ptr<void> prm_item_ptr = (*event_params)["mmoItem"];
	boost::shared_ptr<MMOItem> prm_item = (boost::static_pointer_cast<MMOItem>)(prm_item_ptr);

	DataContainer containter;
	containter.set_sfs_mmoitem(prm_item);
	containter.set_event_name("mmoitem_update");
	client->event_datas.push_back(containter);
}

void SFSClient::on_proximity_list_update(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	//proximity_update
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();
	boost::shared_ptr<void> prm_remove_users_ptr = (*event_params)["removedUsers"];
	boost::shared_ptr<void> prm_add_users_ptr = (*event_params)["addedUsers"];
	boost::shared_ptr<void> prm_remove_items_ptr = (*event_params)["removedItems"];
	boost::shared_ptr<void> prm_add_items_ptr = (*event_params)["addedItems"];

	boost::shared_ptr<std::vector<boost::shared_ptr<User>>> prm_remove_users = (boost::static_pointer_cast<std::vector<boost::shared_ptr<User>>>)(prm_remove_users_ptr);
	boost::shared_ptr<std::vector<boost::shared_ptr<User>>> prm_add_users = (boost::static_pointer_cast<std::vector<boost::shared_ptr<User>>>)(prm_add_users_ptr);

	boost::shared_ptr<std::vector<boost::shared_ptr<MMOItem>>> prm_remove_items = (boost::static_pointer_cast<std::vector<boost::shared_ptr<MMOItem>>>)(prm_remove_items_ptr);
	boost::shared_ptr<std::vector<boost::shared_ptr<MMOItem>>> prm_add_items = (boost::static_pointer_cast<std::vector<boost::shared_ptr<MMOItem>>>)(prm_add_items_ptr);
	
	DataContainer containter;
	containter.set_lists(prm_add_users, prm_remove_users, prm_add_items, prm_remove_items);
	containter.set_event_name("proximity_update");
	client->event_datas.push_back(containter);
}

void SFSClient::on_extension_response(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();
	DataContainer containter;

	boost::shared_ptr<void> prm_cmd_ptr = (*event_params)["cmd"];
	boost::shared_ptr<string> prm_cmd = ((boost::static_pointer_cast<string>)(prm_cmd_ptr));
	containter.set_command(prm_cmd->c_str());

	boost::shared_ptr<void> prm_params_ptr = (*event_params)["params"];
	boost::shared_ptr<SFSObject> prm_params = ((boost::static_pointer_cast<SFSObject>)(prm_params_ptr));
	containter.set_sfs_object(prm_params);

	containter.set_event_name("extension");
	client->event_datas.push_back(containter);
}

void SFSClient::on_admin_message(unsigned long long context, boost::shared_ptr<BaseEvent> evt)
{
	SFSClient* client = (SFSClient*)context;
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> event_params = evt->Params();
	DataContainer containter;

	boost::shared_ptr<void> prm_sender_ptr = (*event_params)["sender"];
	boost::shared_ptr<User> prm_sender = (boost::static_pointer_cast<User>)(prm_sender_ptr);
	containter.set_sfs_user(prm_sender);

	boost::shared_ptr<void> prm_message_ptr = (*event_params)["message"];
	boost::shared_ptr<string> prm_message = (boost::static_pointer_cast<string>)(prm_message_ptr);
	boost::shared_ptr<SFSObject> data(new SFSObject());
	data->PutUtfString("message", prm_message->c_str());
	containter.set_sfs_object(data);

	containter.set_event_name("admin_message");
	client->event_datas.push_back(containter);
}
