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

#ifndef HEADER_CRAFTAI_H
#define HEADER_CRAFTAI_H

#include <curl/curl.h>
#include "vendor/jsonsl.h"

#ifdef _cplusplus
extern "C"
#endif /** _cplusplus */

typedef struct craft_settings {
    char *owner;
    char *token;
} craft_settings_t;

typedef enum craft_type {
    ENUM,
    CONTINUOUS
} craft_type_t;

typedef struct craft_context {
    time_t timestamp;
    jsonsl_t diff;
} craft_context_t;

typedef struct craft_model {
    craft_context_t *context;
    char **outputs;
    int time_quantum;
} craft_model_t;

typedef struct craft_tree {
    char *version;
    craft_model_t *model;
    jsonsl_t tree;
} craft_tree_t;

typedef struct craft_decision {
    craft_context_t context;

    jsonsl_t *predicates_list;
    float confidence;

    char *output_name;
    craft_type_t output_type;
    void *output_value;
} craft_decision_t;

typedef enum craft_status {
    CRAFTAI_OK = 0,

    /* From request errors*/
    CRAFTAI_NETWORK_ERROR,
    CRAFTAI_CREDENTIALS_ERROR,
    CRAFTAI_BAD_REQUEST_ERROR,
    CRAFTAI_INTERNAL_ERROR,
    CRAFTAI_NOT_FOUND_ERROR,

    /* From local craftai features */
    CRAFTAI_DECISION_ERROR,
    CRAFTAI_TIME_ERROR,

    CRAFTAI_UNKNOWN_ERROR
} craft_status_t;


craft_status_t craft_set_config(craft_settings_t *config);

craft_status_t craft_create_agent(craft_model_t *model, char *agent_id);
craft_status_t craft_get_agent(char *agent_id);
craft_status_t craft_delete_agent(char *agent_id);

craft_status_t craft_add_operations(char *agent_id, craft_context_t *operations_list, int nb_operations);
craft_status_t craft_list_operations(char *agent_id, craft_context_t *operations_list, int nb_operations);
craft_status_t craft_retrieve_state(char *agent_id, time_t timestamp, craft_context_t *state);

craft_status_t craft_compute_decision_tree(char *agent_id, time_t timestamp, craft_tree_t *tree);
craft_status_t craft_decide(craft_tree_t *tree, craft_context_t *context, craft_decision_t *decision);

#endif