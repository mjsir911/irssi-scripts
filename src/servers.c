#include "servers.h"
#include "core.h"

#include <irssi/src/common.h>
#include <irssi/src/core/signals.h>
#include <irssi/src/core/chat-protocols.h>

#include <irssi/src/core/servers.h>

#include "utils.h"

#include "channels.h"

static SERVER_CONNECT_REC *create(void) {
	debug();
	return g_new0(SERVER_CONNECT_REC, 1);
}

static SERVER_REC *init(SERVER_CONNECT_REC *connrec) {
	debug();
	SERVER_REC *server = g_new0(SERVER_REC, 1);
	server->chat_type = PROTOCOL;
	server->connrec = connrec;

	/*
	 * Is there a reason to assign these functions when connected and not on
	 * init?
	 */
	server->channels_join = join;
	server->ischannel = (int (*)(SERVER_REC *, const char *)) is_channel;
	server->send_message = send_message;

	connrec->address = "gateway.discord.gg";
	connrec->port = 443; // TODO: make this default a better way

	server_connect_ref(SERVER_CONNECT(connrec));
	server_connect_init(server);
	return server;
}

static void sig_connected(SERVER_REC *server) {
	debug();
	server->connected = true; // XXX \\ Why do I have to do this tho
}

static void connect(SERVER_REC *server) {
	debug();
	printf("%d", server_start_connect(server));
	printf("%d", server->connected);
	return;
}

static void destroy(SERVER_CONNECT_REC *conn) {
	debug();
	return;
}

void servers_protocol_init(CHAT_PROTOCOL_REC *rec) {
	rec->create_server_connect = create;
	rec->server_init_connect = init;
	rec->server_connect = connect;
	rec->destroy_server_connect = destroy;

	signal_add_first("server connected", (SIGNAL_FUNC) sig_connected);
}
