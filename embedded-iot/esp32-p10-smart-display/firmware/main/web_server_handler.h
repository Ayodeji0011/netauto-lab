#pragma once

#include "shared_data.h"
#include <WebServer.h>

void initWebServer();
void webServerTask(void *pvParameters);
