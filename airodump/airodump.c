#include "airodump.h"

bool is_beacon(BeaconFrame *bf)
{
	if (bf->Type == 0 && bf->SubType == 8) //Manage && Beacon
		return true;
	return false;
}

void get_ESSID(u_char *tag, u_char *ESSID)
{
	Tag *cur = (Tag *)tag;

	while (cur->Tag_Number != 0x00)
		cur = tag + 2 + cur->Tag_length;

	memcpy(ESSID, cur->data, cur->Tag_length);
}

void get_CH(u_char *tag, uint8_t *CH)
{
	Tag *cur = (Tag *)tag;

	while (cur->Tag_Number != 0x03)
		cur = tag + 2 + cur->Tag_length;
	*CH = *(cur->data);
}

APlist *create_AP(uint8_t *BSSID, int PWR, uint32_t Beacons, uint8_t CH, u_char *ESSID)
{
	APlist *new_ap = (APlist *)malloc(sizeof(APlist));
	if (new_ap == NULL)
		return (APlist *)NULL;
	if (BSSID != NULL)
		memcpy(new_ap->BSSID, BSSID, 6);
	new_ap->PWR = PWR;
	new_ap->Beacons = Beacons;
	new_ap->CH = CH;

	if (ESSID != NULL)
		strcpy(new_ap->ESSID, ESSID);

	new_ap->next = NULL;

	return new_ap;
}

void init_AP(APlist **aphdrp)
{
	*aphdrp = create_AP(NULL, 0, 0, 0, NULL); 

	return ;
}

void check_dup(APlist *aphdr, const u_char *packet)
{
	Radiotap *rth = (Radiotap *)packet;
	BeaconFrame *bf = (BeaconFrame *)(packet + sizeof(Radiotap));
	FixedParam *fx = (FixedParam *)(packet + sizeof(Radiotap) + sizeof(BeaconFrame));
	Tag *tag = (Tag *)(packet + sizeof(Radiotap) + sizeof(BeaconFrame) + sizeof(FixedParam));

//	printf("\n\n");
//	for (int i=0; i<sizeof(Radiotap); i++)
//	{
//		printf("%02x ", ((u_char *)rth)[i]);
//	}
//	printf("\n");
//	for (int i=0; i<sizeof(BeaconFrame); i++)
//	{
//		printf("%02x ", ((u_char *)bf)[i]);
//	}
//	printf("\n");
//	for (int i=0; i<sizeof(FixedParam); i++)
//	{
//		printf("%02x ", ((u_char *)fx)[i]);
//	}
//	printf("\n");
//	for (int i=0; i<0x10; i++)
//	{
//		printf("%02x ", ((u_char *)tag)[i]);
//	}
//	printf("\n\n");

	u_char ESSID[0x100] = {0, };
	uint8_t CH;

	//get_ESSID((u_char *)tag, ESSID);
	//get_CH((u_char *)tag, &CH);

	bool is_dup = false;
	for (APlist *p=aphdr->next; p != NULL; p=p->next)
	{
		if (!memcmp(p->BSSID, bf->BSS_Id, 6))
		{
			p->Beacons++;
			p->PWR = rth->Antenna_signal;
			is_dup = true;
		}
	}

	if (!is_dup)
	{
		APlist *new_ap = create_AP(bf->BSS_Id, rth->Antenna_signal, 1, CH, ESSID);
		new_ap->next = aphdr->next;
		aphdr->next = new_ap;
	}
}

void print_dump(APlist *aphdr)
{
	printf("\n%-17s\t%3s\t%7s\t%2s\t%s\n\n", "BSSID", "PWR", "Beacons", "CH", "ESSID");
	for (APlist *p=aphdr->next; p!=NULL; p=p->next)
	{
		for (int i=0; i<6; i++)
		{
			printf("%02x", p->BSSID[i]);
			if (i != 5)
				printf(":");
		}
		printf("\t%3d\t%7d\t%2d\t%s\n", p->PWR, p->Beacons, p->CH, p->ESSID);
	}

	return ;
}
