/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright 2022 NXP */
#ifndef _ISOCHRON_SEND_H
#define _ISOCHRON_SEND_H

#include <pthread.h>
#include <linux/if_packet.h>
#include <linux/limits.h>
#include <linux/un.h>

#include "log.h"
#include "ptpmon.h"
#include "sysmon.h"

#define BUF_SIZ		10000

struct isochron_send {
	volatile bool send_tid_should_stop;
	volatile bool send_tid_stopped;
	unsigned char dest_mac[ETH_ALEN];
	unsigned char src_mac[ETH_ALEN];
	char if_name[IFNAMSIZ];
	char uds_remote[UNIX_PATH_MAX];
	__u8 sendbuf[BUF_SIZ];
	struct ptpmon *ptpmon;
	struct sysmon *sysmon;
	struct mnl_socket *rtnl;
	enum port_state last_local_port_state;
	enum port_state last_remote_port_state;
	struct cmsghdr *cmsg;
	struct iovec iov;
	struct msghdr msg;
	char msg_control[CMSG_SPACE(sizeof(__s64))];
	struct ip_address stats_srv;
	union {
		struct sockaddr_ll l2;
		struct sockaddr_in udp4;
		struct sockaddr_in6 udp6;
	} sockaddr;
	size_t sockaddr_size;
	struct isochron_log log;
	unsigned long timestamped;
	unsigned long iterations;
	clockid_t clkid;
	__s64 session_start;
	__s64 advance_time;
	__s64 shift_time;
	__s64 cycle_time;
	__s64 base_time;
	__s64 window_size;
	long priority;
	long tx_len;
	int data_fd;
	int stats_fd;
	long vid;
	bool do_ts;
	bool quiet;
	long etype;
	bool omit_sync;
	bool omit_remote_sync;
	bool trace_mark;
	int trace_mark_fd;
	char tracebuf[BUF_SIZ];
	long stats_port;
	bool taprio;
	bool txtime;
	bool deadline;
	bool do_vlan;
	int l2_header_len;
	int l4_header_len;
	bool sched_fifo;
	bool sched_rr;
	long sched_priority;
	long utc_tai_offset;
	struct ip_address ip_destination;
	bool l2;
	bool l4;
	long data_port;
	long domain_number;
	long transport_specific;
	long sync_threshold;
	long num_readings;
	char output_file[PATH_MAX];
	pthread_t send_tid;
	pthread_t tx_timestamp_tid;
	int send_tid_rc;
	int tx_timestamp_tid_rc;
	unsigned long cpumask;
};

int isochron_send_parse_args(int argc, char **argv, struct isochron_send *prog);
int isochron_send_init(struct isochron_send *prog);

#endif