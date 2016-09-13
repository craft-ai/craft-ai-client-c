/** Copyright (c) 2016, craft ai All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of craft ai nor the names of
 *    its contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "craftai.h"

#include <curl/curl.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum craft_client_status {
    CRAFTAI_TEARDOWNED = 0,
    CRAFTAI_INITIALIZED
} craft_client_status_t;

#define CRAFT_MAX_OWNER_SIZE 256
#define CRAFT_MAX_TOKEN_SIZE 1024
#define CRAFT_MAX_URL_SIZE 512

typedef struct craft_client {
    craft_client_status_t status;
    /* Configuration */
    char owner[CRAFT_MAX_OWNER_SIZE];
    char token[CRAFT_MAX_TOKEN_SIZE];
    char url[CRAFT_MAX_URL_SIZE];
} craft_client_t;

static craft_client_t default_client = { CRAFTAI_TEARDOWNED, "", "", "https://beta.craft.ai" };
static craft_client_t client = { CRAFTAI_TEARDOWNED, "", "", "https://beta.craft.ai" };

craft_status_t craft_init() {
    if (client.status != CRAFTAI_TEARDOWNED) {
      return CRAFTAI_ALREADY_INITIALIZED;
    }

    /* Initializing curl with support for HTTPS */
    CURLcode curl_init_status = curl_global_init(CURL_GLOBAL_ALL);
    if (curl_init_status != CURLE_OK) {
      return CRAFTAI_CURL_ERROR;
    }

    client.status = CRAFTAI_INITIALIZED;
    
    return CRAFTAI_OK;
}

craft_status_t craft_teardown() {
    /* Cleaning up curl */
    curl_global_cleanup();

    /* Resetting the client itself */
    client = default_client;

    return CRAFTAI_OK;
}

/**
 * Set the value of a given option in the client configuration.
 */
craft_status_t craft_config(craft_option_t option, ...) {
  va_list argp;

  va_start(argp, option);
  switch (option) {
    case CRAFTAI_TOKEN:
      strcpy(client.token, va_arg(argp, char*));
      return CRAFTAI_OK;
    case CRAFTAI_OWNER:
      strcpy(client.owner, va_arg(argp, char*));
      return CRAFTAI_OK;
    case CRAFTAI_URL:
      strcpy(client.url, va_arg(argp, char*));
      return CRAFTAI_OK;
    default:
      return CRAFTAI_UNKNOWN_CONFIG_OPTION;
  }
}

craft_status_t craft_create_agent(craft_model_t* model, char** agent_id) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_get_agent(char* agent_id, craft_model_t* model, time_t* first_ts, time_t* last_ts) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_delete_agent(char* agent_id) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_add_operations(char* agent_id, craft_context_t* operations_list, size_t nb_operations) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_list_operations(char* agent_id, craft_context_t* operations_list, size_t* nb_operations) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_retrieve_state(char* agent_id, time_t timestamp, craft_context_t* state) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}
