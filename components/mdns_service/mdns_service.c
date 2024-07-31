#include <stdio.h>
#include "mdns_service.h"

void start_mdns_service()
{
    //initialize mDNS service
    esp_err_t err = mdns_init();
    if (err) {
        printf("MDNS Init failed: %d\n", err);
        return;
    }

    //set hostname
    mdns_hostname_set("esp-hero");
    //set default instance
    mdns_instance_name_set("ESP smart light controller");
}
