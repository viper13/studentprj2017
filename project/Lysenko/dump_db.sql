DROP TABLE IF EXISTS users_by_chats;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS chats;
DROP TABLE IF EXISTS users;

CREATE TABLE users
(
    id SERIAL PRIMARY KEY,
    name varchar(100) NOT NULL,
    online boolean NOT NULL
);

CREATE TABLE chats
(
    id SERIAL PRIMARY KEY,
    name varchar(100) NOT NULL
);

CREATE TABLE messages
(
    id SERIAL PRIMARY KEY,
    chat_id integer NOT NULL,
    user_id integer NOT NULL,
    message text NOT NULL,
    FOREIGN KEY (chat_id) REFERENCES chats(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE users_by_chats
(
    chat_id integer NOT NULL,
    user_id integer NOT NULL,
    PRIMARY KEY (chat_id, user_id),
    FOREIGN KEY (chat_id) REFERENCES chats(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
);