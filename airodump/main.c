#include "airodump.h"

char *interface;

void usage()
{
	printf("syntax: airodump <interface>\n");
	printf("sample: airodump mon0\n");
	return ;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		usage();
		return 1;
	}
	interface = argv[1];
	char errbuf[PCAP_ERRBUF_SIZE];
	
	pcap_t *pcap = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
	if (pcap == NULL)
	{
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", interface, errbuf);
		return 1;
	}
	
	APlist *aphdr = NULL;
	init_AP(&aphdr);
	if (!aphdr)
		return 1;

	while (true)
	{
		struct pcap_pkthdr *header;
		const u_char *packet;
		int res = pcap_next_ex(pcap, &header, &packet);

		if (res == 0)
			continue;
		if (res == PCAP_ERROR | res == PCAP_ERROR_BREAK)
		{
			printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
			break;
		}
		
		if (!is_beacon((BeaconFrame *)(packet + sizeof(Radiotap))))
			continue;
		check_dup(aphdr, packet);
		print_dump(aphdr);
	}
	pcap_close(pcap);
	
	return 0;
}
