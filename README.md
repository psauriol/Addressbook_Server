Addressbook_Server
==================

This server is used to communicate with a simple client via sockets.

Listens for a connection request then takes commands from the client to query a simple text database.
The database currently consists of email owner pairs.

The server can accept these commands from the client:
Insert - adds an entry to the database
Quit - disconnects the client from the server
Find - searches the database for an email and returns the name of the owner

Possible future addtions:
Command Delete - removes an entry from the database
Command Modify - modifies an entry from the database
