#ifndef _SEATD_SEAT_H
#define _SEATD_SEAT_H

#include "list.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

struct client;

enum seat_device_type {
	SEAT_DEVICE_TYPE_NORMAL,
	SEAT_DEVICE_TYPE_EVDEV,
	SEAT_DEVICE_TYPE_DRM,
};

struct seat_device {
	int device_id;
	int fd;
	int ref_cnt;
	bool active;
	char *path;
	enum seat_device_type type;
};

struct seat {
	char *seat_name;
	struct list clients;
	struct client *active_client;
	struct client *next_client;

	bool vt_bound;
	bool vt_pending_ack;
	int next_vt;
};

struct seat *seat_create(const char *name, bool vt_bound);
void seat_destroy(struct seat *seat);

int seat_add_client(struct seat *seat, struct client *client);
int seat_remove_client(struct seat *seat, struct client *client);
int seat_open_client(struct seat *seat, struct client *client);
int seat_close_client(struct seat *seat, struct client *client);

struct seat_device *seat_open_device(struct client *client, const char *path);
int seat_close_device(struct client *client, struct seat_device *seat_device);
struct seat_device *seat_find_device(struct client *client, int device_id);

int seat_set_next_session(struct seat *seat, int session);
int seat_activate(struct seat *seat);
int seat_prepare_vt_switch(struct seat *seat);

#endif
