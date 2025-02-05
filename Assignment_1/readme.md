# Multi-Client Chat Server

## Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Design Decisions](#design-decisions)
4. [Implementation Details](#implementation-details)
5. [Key Functions](#key-functions)
6. [Testing](#testing)
7. [Restrictions and Known Issues](#restrictions-and-known-issues)
8. [Challenges Faced](#challenges-faced)
9. [Contributors](#contributors)
10. [Setup and Running](#setup-and-running)
11. [Declaration](#declaration)

## Overview
This project implements a robust multi-client chat server in C++. It supports multiple concurrent connections, user authentication, private messaging, group chats, and broadcasting capabilities. The server is designed to handle various client requests efficiently and securely.

## Features

### Server Functionality
- Initializes and listens on port 8080
- Accepts and manages multiple concurrent client connections
- Maintains a dynamic list of connected clients with their usernames

### User Authentication
- Implements a secure user authentication system
- Users must provide valid credentials to log in
- Authentication failures result in immediate disconnection

### Client Commands
The server supports the following client commands:
- `/msg <user> <message>`: Send private messages to online clients
- `/create_group <group_name>`: Create a new group chat
- `/join_group <group_name>`: Join an existing group
- `/leave_group <group_name>`: Leave a group
- `/broadcast <message>`: Send a message to all online clients
- `/group_message <group_name> <message>`: Send a message to all online members of a specific group
- `/exit`: Disconnect from the server

## Design Decisions

### Threading Model
- The server employs a multi-threaded architecture
- Each client connection is handled in a separate thread
- This approach ensures efficient resource management and responsiveness

### Data Structures
- Client information is stored using a custom `ClientInfo` struct:
- Online users are tracked using a map (username to socket)
- Groups are managed using a map (group name to set of users)

### User Management
- User credentials are loaded from a `users.txt` file at server startup
- Active online users are maintained in a dynamic map for quick access and updates

### Synchronization
- Mutex locks are used to ensure thread-safe operations on shared resources
- This prevents race conditions when updating client lists or sending messages

### Code Structure
- The project is segmented into several key files:
-  **server.cpp** : Contains the Server class, initializes the server, and binds it to port 8080.
- **response.cpp**: Builds HTTP responses to send to clients.
- **request.cpp**: Parses HTTP requests from clients, extracts methods and variables, and calls appropriate functions according to user
- **client.cpp**: Implements client-related functions and loads user data into the online users map from **users.txt**.
- This modular structure allows for better organization and separation of concerns within the project.

## Implementation Details

### Server Startup Process
1. Server initializes and binds to port 8080
2. User credentials are loaded from `users.txt`
3. The server enters an accept loop to handle incoming client connections
![Alt Text](D:\ANYA\CS425\Assignment_1\pictures\Screenshot 2025-02-05 225117.png)

### Client Login Flow
1. Client establishes a connection to the server
2. Server prompts for username and password
3. Credentials are verified against the loaded user data
4. Upon successful authentication, the client is registered and welcomed
5. Failed authentication results in disconnection

### Message Handling
- Private messages: Implemented via `/msg <user> <message>`
- Broadcasts: Sent using `/broadcast <message>`
- Group messages: Managed through dedicated group functions

### Group Management
- Group creation: `/create_group <group_name>`
- Joining groups: `/join_group <group_name>`
- Leaving groups: `/leave_group <group_name>`
- Group messaging: `/group_message <group_name> <message>`

### Client Disconnection
- Triggered by the `/exit` command
- Server removes the client from all relevant data structures
- The client's connection is closed, and resources are freed

## Key Functions

### Server Core Functions
- `start()`: Initializes the server, binds to the port, and prepares for connections
- `stop()`: Gracefully shuts down the server, closing all connections

### Client Handling Functions
- `handleClient()`: Manages the entire lifecycle of a client session
- `sendMessage()`: Sends a message to a specific client
- `broadcastMessage()`: Distributes a message to all connected clients
- `sendPrivateMessage()`: Handles private messaging between clients

### Group Management Functions
- `createGroup()`: Establishes a new group chat
- `joinGroup()`: Adds a client to an existing group
- `leaveGroup()`: Removes a client from a group
- `sendGroupMessage()`: Distributes a message to all members of a group

### User Management Functions
- `loadUsers()`: Reads and loads user credentials from file
- `addClient()`: Registers a new client in the active clients list
- `removeClient()`: Removes a client and performs necessary cleanup

### Utility Functions
- `welcomeClient()`: Sends a welcome message to newly connected clients
- Various helper functions for message parsing and request handling

## Testing
- Extensive manual testing was conducted to verify all functionalities
- Edge cases were explored to ensure system stability
- Stress testing involved multiple concurrent user logins and interactions
- The team aimed for 100% code coverage in their testing efforts

## Restrictions and Known Issues
- Group names cannot contain spaces
- Multiple logins from the same user account are not supported
- Very large messages may cause system slowdown
- Message display might be affected if received while a user is typing

## Challenges Faced
1. Cross-platform socket library differences (Windows, Linux, Mac)
2. Message parsing and handling of empty messages
3. Server socket reuse issues after closing
4. Mutex-related problems with client reconnection
5. Parsing issues with different line ending formats in `users.txt`

## Contributors
- Anya Rajan (220191) - 40%
- Ananya Singh Baghel (220136) - 40%
- Nandini Akolkar (220692) - 20%

Contribution breakdown:
- Design and implementation: Anya and Ananya
- Testing: Anya and Ananya
- README documentation: Nandini

## Setup and Running
1. Clone the repository
2. Navigate to the `Assignment_1` directory
3. Compile the project: `make all`
4. Start the server: `./server_grp`
5. Start a client: `./client_grp`
 - Enter username and password from `users.txt`
 - Use "127.0.0.1" if client and server are on the same machine

## Declaration
This project is original work completed by the contributors listed above. No plagiarism was involved in the development of this software.
