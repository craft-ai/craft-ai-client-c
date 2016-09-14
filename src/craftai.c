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
#include <yajl/yajl_gen.h>
#include <yajl/yajl_parse.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum craft_client_status {
    CRAFTAI_TEARDOWNED = 0,
    CRAFTAI_INITIALIZED
} craft_client_status_t;

typedef enum craft_client_http_verb {
    craft_client_http_verb_get = 0,
    craft_client_http_verb_post,
    craft_client_http_verb_delete
} craft_client_http_verb_t;

#define CRAFT_PARSER_MAX_STACK_SIZE 16
#define CRAFT_MAX_OWNER_SIZE 256
#define CRAFT_MAX_TOKEN_SIZE 1024
#define CRAFT_MAX_URL_SIZE 512
#define CRAFT_MAX_BUFFER_SIZE 2048

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
    {
        CURLcode curl_init_status = curl_global_init(CURL_GLOBAL_ALL);
        if (curl_init_status != CURLE_OK) {
          return CRAFTAI_CURL_ERROR;
        }
    }

    /* Read initial configuration from environment */
    {
        char* envOwner = getenv("CRAFT_OWNER");
        if (envOwner) {
          strcpy(client.owner, envOwner);
        }
    }
    {
        char* envToken = getenv("CRAFT_TOKEN");
        if (envToken) {
          strcpy(client.token, envToken);
        }
    }
    {
        char* envUrl = getenv("CRAFT_URL");
        if (envUrl) {
          strcpy(client.url, envUrl);
        }
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

typedef struct craft_buffer {
    char data[CRAFT_MAX_BUFFER_SIZE];
    size_t len;
} craft_buffer_t;

size_t craft_buffer_write(craft_buffer_t* buffer, char* str, size_t len) {
    if (buffer->len + len + 1 > CRAFT_MAX_BUFFER_SIZE) {
      return 0;
    }
    strncpy(buffer->data + buffer->len, str, len);
    buffer->len += len;
    buffer->data[buffer->len] = '\0';
    return len;
}

void craft_buffer_yajl_print_callback(void* ctx, char* str, size_t len) {
    craft_buffer_write((craft_buffer_t*)ctx, str, len);
}

size_t craft_buffer_curl_write_callback(char* ptr, size_t size, size_t nmemb, void* ctx) {
    return craft_buffer_write((craft_buffer_t*)ctx, ptr, size * nmemb);
}

yajl_gen_status craft_gen_model(yajl_gen gen, craft_model_t* model) {
  size_t i;
  yajl_gen_status s = yajl_gen_status_ok;
  s = s ||  yajl_gen_map_open(gen);
  s = s ||    yajl_gen_string(gen, (unsigned char *)"context", 7);
  s = s ||    yajl_gen_map_open(gen);
  for(i = 0 ; i < model->properties_count ; ++i) {
      craft_property_definition_t* property_definition = &model->properties[i];
      s = s ||  yajl_gen_string(gen, (unsigned char *)property_definition->key, strlen(property_definition->key));
      s = s ||  yajl_gen_map_open(gen);
      s = s ||    yajl_gen_string(gen, (unsigned char *)"type", 4);
      switch (property_definition->type) {
      case TIME_OF_DAY:
          s = s ||yajl_gen_string(gen, (unsigned char *)"time_of_day", 11);
          s = s ||yajl_gen_string(gen, (unsigned char *)"is_generated", 12);
          s = s ||yajl_gen_bool(gen, property_definition->is_generated);
          break;
      case DAY_OF_WEEK:
          s = s ||yajl_gen_string(gen, (unsigned char *)"day_of_week", 11);
          s = s ||yajl_gen_string(gen, (unsigned char *)"is_generated", 12);
          s = s ||yajl_gen_bool(gen, property_definition->is_generated);
          break;
      case CONTINUOUS:
          s = s ||yajl_gen_string(gen, (unsigned char *)"continuous", 10);
          break;
      case ENUM:
      default:
          s = s ||yajl_gen_string(gen, (unsigned char *)"enum", 4);
      }
      s = s ||  yajl_gen_map_close(gen);
  }
  s = s ||    yajl_gen_map_close(gen);
  s = s ||    yajl_gen_string(gen, (unsigned char *)"output", 6);
  s = s ||    yajl_gen_array_open(gen);
  for(i = 0 ; i < model->outputs_count ; ++i) {
      char* output = model->outputs[i];
      s = s ||  yajl_gen_string(gen, (unsigned char *)output, strlen(output));
  }
  s = s ||    yajl_gen_array_close(gen);
  s = s ||  yajl_gen_map_close(gen);
  return s;
}

craft_status_t craft_serialize_model(craft_model_t* model, craft_buffer_t* buffer) {
    yajl_gen gen = yajl_gen_alloc(NULL);
    yajl_gen_status s;
    yajl_gen_config(gen, yajl_gen_print_callback, craft_buffer_yajl_print_callback, buffer);
    s = craft_gen_model(gen, model);
    yajl_gen_free(gen);
    if (s == yajl_gen_status_ok) {
        return CRAFTAI_OK;
    }
    else {
        return CRAFTAI_INTERNAL_ERROR;
    }
}

craft_status_t craft_serialize_new_agent(craft_model_t* model, char* agent_id, craft_buffer_t* buffer) {
    yajl_gen gen = yajl_gen_alloc(NULL);
    yajl_gen_status s = yajl_gen_status_ok;

    yajl_gen_config(gen, yajl_gen_print_callback, craft_buffer_yajl_print_callback, buffer);

    s = s ||  yajl_gen_map_open(gen);
    if (agent_id != 0 && strlen(agent_id) > 0) {
      s = s ||  yajl_gen_string(gen, (unsigned char *)"id", 2);
      s = s ||  yajl_gen_string(gen, (unsigned char *)agent_id, strlen(agent_id));
    }
    s = s ||    yajl_gen_string(gen, (unsigned char *)"model", 5);
    s = s ||    craft_gen_model(gen, model);
    s = s ||  yajl_gen_map_close(gen);
    yajl_gen_free(gen);
    if (s == yajl_gen_status_ok) {
        return CRAFTAI_OK;
    }
    else {
        return CRAFTAI_INTERNAL_ERROR;
    }
}

typedef struct craft_agent {
    char id[CRAFT_MAX_ID_SIZE];
    char modelId[CRAFT_MAX_ID_SIZE];
} craft_agent_t;

typedef enum craft_agent_parser_status {
    CRAFT_AGENT_PARSER_STATUS_ROOT = 0,
    CRAFT_AGENT_PARSER_STATUS_ID,
    CRAFT_AGENT_PARSER_STATUS_MODEL_ID
} craft_agent_parser_status_t;

typedef struct craft_agent_parser_ctx {
    craft_agent_t* agent;
    craft_agent_parser_status_t stack[CRAFT_PARSER_MAX_STACK_SIZE];
    size_t stackSize;
} craft_agent_parser_ctx_t;

int craft_agent_parser_start_map(void* ctx) {
    craft_agent_parser_ctx_t* agent_parser_ctx = (craft_agent_parser_ctx_t*)ctx;
    if (agent_parser_ctx->stackSize != 0) {
        return 0;
    }
    agent_parser_ctx->stack[agent_parser_ctx->stackSize] = CRAFT_AGENT_PARSER_STATUS_ROOT;
    ++agent_parser_ctx->stackSize;
    return 1;
}

int craft_agent_parser_end_map(void* ctx) {
    craft_agent_parser_ctx_t* agent_parser_ctx = (craft_agent_parser_ctx_t*)ctx;
    if (agent_parser_ctx->stackSize != 1 ||
        agent_parser_ctx->stack[0] != CRAFT_AGENT_PARSER_STATUS_ROOT) {
        return 0;
    }
    --agent_parser_ctx->stackSize;
    return 1;
}

int craft_agent_parser_map_key(void* ctx, const unsigned char* key, size_t stringLen) {
    craft_agent_parser_ctx_t* agent_parser_ctx = (craft_agent_parser_ctx_t*)ctx;
    if (agent_parser_ctx->stackSize != 1 ||
        agent_parser_ctx->stack[0] != CRAFT_AGENT_PARSER_STATUS_ROOT) {
        return 0;
    }
    if (stringLen == 2 && strncmp("id", (char*)key, 2) == 0) {
        agent_parser_ctx->stack[agent_parser_ctx->stackSize] = CRAFT_AGENT_PARSER_STATUS_ID;
        ++agent_parser_ctx->stackSize;
        return 1;
    }
    if (stringLen == 5 && strncmp("model", (char*)key, 5) == 0) {
        agent_parser_ctx->stack[agent_parser_ctx->stackSize] = CRAFT_AGENT_PARSER_STATUS_MODEL_ID;
        ++agent_parser_ctx->stackSize;
        return 1;
    }
    return 0;
}

int craft_agent_parser_string(void* ctx, const unsigned char* stringVal, size_t stringLen) {
    craft_agent_parser_ctx_t* agent_parser_ctx = (craft_agent_parser_ctx_t*)ctx;
    if (agent_parser_ctx->stackSize != 2) {
        return 0;
    }
    switch (agent_parser_ctx->stack[1]) {
    case CRAFT_AGENT_PARSER_STATUS_ID:
        strncpy(agent_parser_ctx->agent->id, (char*)stringVal, stringLen);
        --agent_parser_ctx->stackSize;
        return 1;
    case CRAFT_AGENT_PARSER_STATUS_MODEL_ID:
        strncpy(agent_parser_ctx->agent->modelId, (char*)stringVal, stringLen);
        --agent_parser_ctx->stackSize;
        return 1;
    default:
        return 0;
    }
}

yajl_callbacks craft_agent_parser_callbacks = {
    0, /* yajl_null */
    0, /* yajl_boolean */
    0, /* yajl_integer */
    0, /* yajl_double */
    0, /* yajl_number */
    craft_agent_parser_string, /* yajl_string */
    craft_agent_parser_start_map, /* yajl_start_map */
    craft_agent_parser_map_key, /* yajl_map_key */
    craft_agent_parser_end_map, /* yajl_end_map */
    0, /* yajl_start_array */
    0 /* yajl_end_array */
};

craft_status_t craft_request(craft_client_http_verb_t verb, char* url, craft_buffer_t* request, craft_buffer_t* response) {
    long response_code;
    struct curl_slist* hdr_list = 0;

    /* get a curl handle */
    CURL* curl = curl_easy_init();
    if (!curl) {
        return CRAFTAI_CURL_ERROR;
    }

    /* Let's deal with the query URL */
    printf("--REQUEST--\n");
    printf("  - url: %s\n", url);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Set the request body */
    if (request) {
        printf("  - body: %s\n\n", request->data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request->data);
    }

    /* Handle the retrieval of response body */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &craft_buffer_curl_write_callback);

    /* Add headers */
    {
        CURLcode curl_hdr_status;
        hdr_list = curl_slist_append(hdr_list, "Content-Type: application/json");
        if (strlen(client.token) > 0) {
            char authorization_header[CRAFT_MAX_TOKEN_SIZE + 23];
            sprintf(authorization_header, "Authorization: Bearer %s", client.token);
            hdr_list = curl_slist_append(hdr_list, authorization_header);
        }

        curl_hdr_status = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdr_list);

        if(curl_hdr_status != CURLE_OK) {
            curl_easy_cleanup(curl);
            curl_slist_free_all(hdr_list);
            return CRAFTAI_CURL_ERROR;
        }
    }

    /* Let's set the verb */
    {
        CURLcode curl_verb_status;
        switch (verb) {
        case craft_client_http_verb_post:
            curl_verb_status = curl_easy_setopt(curl, CURLOPT_POST, 1);
            break;
        case craft_client_http_verb_delete:
            curl_verb_status = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        case craft_client_http_verb_get:
        default:
            curl_verb_status = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        }
        if(curl_verb_status != CURLE_OK) {
            curl_easy_cleanup(curl);
            curl_slist_free_all(hdr_list);
            return CRAFTAI_CURL_ERROR;
        }
    }

    /* Perform the request, res will get the return code */
    {
        CURLcode curl_res_status = curl_easy_perform(curl);
        /* Check for errors */
        if(curl_res_status != CURLE_OK) {
            curl_easy_cleanup(curl);
            curl_slist_free_all(hdr_list);
            return CRAFTAI_CURL_ERROR;
        }
        /* Retrieve the response status code */
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_cleanup(curl);
        curl_slist_free_all(hdr_list);
    }

    printf("--RESPONSE--\n");
    printf("  - status: %ld\n", response_code);
    printf("  - body: %s\n\n", response->data);

    /* Check for errors */
    switch (response_code) {
    case 200:
    case 201:
        return CRAFTAI_OK;
    case 400:
        return CRAFTAI_BAD_REQUEST_ERROR;
    case 401:
        return CRAFTAI_CREDENTIALS_ERROR;
    case 404:
        return CRAFTAI_NOT_FOUND_ERROR;
    case 500:
    default:
        return CRAFTAI_INTERNAL_ERROR;
    }
}

craft_status_t craft_create_agent(craft_model_t* model, char* agent_id) {
    char url[CRAFT_MAX_URL_SIZE];
    craft_buffer_t request_body;
    craft_buffer_t response_body;

    /* Let's deal with the query URL */
    sprintf(url, "%s/api/%s/agents", client.url, client.owner);

    /* Generate the request body */
    memset(&request_body, 0, sizeof(craft_buffer_t));
    craft_serialize_new_agent(model, agent_id, &request_body);

    /* Initialize the response buffer */

    memset(&response_body, 0, sizeof(craft_buffer_t));

    /* Perform the request ! */
    return craft_request(craft_client_http_verb_post, url, &request_body, &response_body);
}

craft_status_t craft_get_agent(char* agent_id, craft_model_t* model, time_t* first_ts, time_t* last_ts) {
    craft_status_t status = CRAFTAI_OK;

    return status;
}

craft_status_t craft_delete_agent(char* agent_id) {
    char url[CRAFT_MAX_URL_SIZE];
    craft_buffer_t response_body;

    /* Let's deal with the query URL */
    sprintf(url, "%s/api/%s/agents/%s", client.url, client.owner, agent_id);

    /* Initialize the response buffer */
    memset(&response_body, 0, sizeof(craft_buffer_t));

    /* Perform the request ! */
    return craft_request(craft_client_http_verb_delete, url, 0, &response_body);
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
