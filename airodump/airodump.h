#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>


// caution: little endian

#pragma pack(push, 1)
typedef struct
{
	uint8_t Header_revision;
	uint8_t Header_pad;
	uint16_t Header_length;
	uint64_t Present_flags;
	uint8_t Flags;
	uint8_t Data_Rate;
	uint16_t Channel_frequency;
	uint16_t Channel_flags;
	uint8_t Antenna_signal;
	uint8_t Reserved;
	uint16_t RX_flags;
	uint8_t Antenna_signal1;
	uint8_t Antenna;
} Radiotap;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	union
	{
		struct
		{
			uint8_t Version : 2;
			uint8_t Type : 2;
			uint8_t SubType : 4;

		};
		uint16_t FrameControlField; 
	};
	uint16_t Duration;
	union
	{
		uint8_t Receiver_address[6];
		uint8_t Destination_address[6];
	};
	union
	{
		uint8_t Transmitter_address[6];
		uint8_t Source_address[6];
	};
	uint8_t BSS_Id[6];
	uint8_t Fragment_number : 4;
	uint16_t Sequence_number : 12;
} BeaconFrame;
#pragma pack(pop)

#pragma pack(push, 4)
typedef struct
{
	uint64_t Timestamp;
	uint16_t Beacon_Interval;
	uint16_t Capabilities_Information;
} FixedParam;
#pragma pack(pop)

#pragma pack(push, 2)
typedef struct
{
	uint8_t Tag_Number;
	uint8_t Tag_length;
	const u_char *data;
} Tag;
#pragma pack(pop)

typedef struct APlist
{
	uint8_t BSSID[6];
	int PWR;
	uint32_t Beacons;
	uint8_t CH;
	u_char ESSID[0x100];

	struct APlist *next;
} APlist;


bool is_beacon(BeaconFrame *bf);
void get_ESSID(u_char *, u_char *);
void get_CH(u_char *, uint8_t *);
APlist *create_AP(uint8_t *, int, uint32_t, uint8_t, u_char *);
void init_AP(APlist **);
void check_dup(APlist *, const u_char *);
void print_dump(APlist *);

