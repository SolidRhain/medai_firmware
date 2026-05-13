#ifndef CERT_MANAGER_H
#define CERT_MANAGER_H

#include <WiFiClientSecure.h>

bool loadCertificates(WiFiClientSecure& secureClient);

#endif
