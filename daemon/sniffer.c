#include "sniffer.h"

static FILE *f;

struct sniff_ip {
        u_char ipVhl;      /* version << 4 | header length >> 2 */
        u_char ipTos;      /* type of service */
        u_short ipLen;     /* total length */
        u_short ipId;      /* identification */
        u_short ipOff;     /* fragment offset field */
    #define IP_RF 0x8000        /* reserved fragment flag */
    #define IP_DF 0x4000        /* dont fragment flag */
    #define IP_MF 0x2000        /* more fragments flag */
    #define IP_OFFMASK 0x1fff   /* mask for fragmenting bits */
        u_char ipTtl;      /* time to live */
        u_char ipP;        /* protocol */
        u_short ipSum;     /* checksum */
        struct in_addr ipSrc;
        struct in_addr ipDst; /* source and dest address */
};

void pHandler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
);


void storeIP(char* ip);


void* sniff(void* args) {
    char* device = (char*) args;
    char errorBuffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    int timeoutLimit = 1000; /* In milliseconds */
    f = fopen("data.txt", "a");
    fprintf(f, "%s", device);

    device = pcap_lookupdev(errorBuffer);
    if (device == NULL) {
        printf("Error finding device: %s\n", errorBuffer);
        return (void*)1;
    }

    /* Open device for live capture */
    handle = pcap_open_live(
            device,
            BUFSIZ,
            0,
            timeoutLimit,
            errorBuffer
        );
    if (handle == NULL) {
         fprintf(stderr, "Could not open device %s: %s\n", device, errorBuffer);
         return (void*)2;
     }
     
    pcap_loop(handle, 0, pHandler, NULL);

    return (void*)0;
}


void pHandler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
)
{
    struct sniff_ip *ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    storeIP(inet_ntoa(ip->ipSrc));
    return;
}


void storeIP(char* ip) {
    fprintf(f, "%d\n", (int)ip);
}
