# What is it

This is minimal Python bindings of the SmartFox c++ client API. It does not contains all possible functionality of the client application, but only one that used in [Two World project](http://twoworlds.azurewebsites.net/).

# Features

Accept the following events:
* `CONNECTION`
* `CONNECTION_LOST`
* `LOGIN`
* `ROOM_JOIN`
* `USER_VARIABLES_UPDATE`
* `MMOITEM_VARIABLES_UPDATE`
* `PROXIMITY_LIST_UPDATE`
* `EXTENSION_RESPONSE`
* `ADMIN_MESSAGE`
* `PUBLIC_MESSAGE`
* `PRIVATE_MESSAGE`

All data from events packed into `DataContainer` - special class that can contains the following items:
* `SFSObject`
* `User`
* `MMOItem`

# How to use

Import and client class instance

```python
import SFSCommunicator as sfs

client = sfs.SFSClient()
```

Then in any non-blocking loop call update

```python
datas_list = client.update()
```

`datas_list` is a list of `DataContainer`s with data, received from events. To obtain event title, for which each container `d` is created

```python
e_name = d.get_event_name()
```

`e_name` is one of the following strings:
* `connection` for `CONNECTION` event
* `connection_lost` for `CONNECTION_LOST` event
* `login` for `LOGIN` event
* `join_room` for `ROOM_JOIN` event
* `user_update` for `USER_VARIABLES_UPDATE` event
* `mmoitem_update` for `MMOITEM_VARIABLES_UPDATE` event
* `proximity_update` for `PROXIMITY_LIST_UPDATE` event
* `extension` for `EXTENSION_RESPONSE` event
* `admin_message` for `ADMIN_MESSAGE` event
* `public_message` for `PUBLIC_MESSAGE` event
* `private_message` for `PRIVATE_MESSAGE` event

For different event container contains different set of data. For example, if container corresponds to `EXTENSION_RESPONSE` event, then we can get the name of the external command by
```python
cmd = d.get_command()
```

Then we can get data from corresponding `SFSObject` by using methods
* `d.get_int(key)`
* `d.get_byte(key)`
* `d.get_float(key)`
* `d.get_fouble(key)`
* `d.get_bool(key)`
* `d.get_string(key)`
* `d.get_double_array(key)`
* `d.get_sfs_array(key)`

If the data event contains user (as, for example, in `USER_VARIABLES_UPDATE`), then we can obtain user variables by using methods
* `d.get_user_string(key)`
* `d.get_user_int(key)`
* `d.get_user_float(key)`
* `d.get_user_double(key)`
* `d.get_user_bool(key)`

Also we can use `d.get_user_id()` to obtain Id of the stored user.

Similar for `MMOItems`
* `d.get_mmoitem_string(key)`
* `d.get_mmoitem_int(key)`
* `d.get_mmoitem_float(key)`
* `d.get_mmoitem_double(key)`

For `PROXIMITY_LIST_UPDATE` the container stores lists for users and mmo-items. We can get it by using
```python
add_user_list = d.get_list_add_users()
remove_user_list = d.get_list_remove_users()
add_mmoitem_list = d.get_list_add_mmoitems()
remove_mmoitem_list = d.get_list_remove_mmoitems()
```

# Usage example

This Python module were used in [TwoWorlds bot](https://github.com/Tugcga/TwoWorlds-bot).