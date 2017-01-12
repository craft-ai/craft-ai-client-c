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


/************************************************************************
 * This library contains some documentation which is specific to this C *
 * implementation of a craft ai SDK, but you should always refer to     *
 * beta.craft.ai/doc **first**!                                         *
 ************************************************************************/

#ifndef CRAFTAI_H
#define CRAFTAI_H

#ifdef _cplusplus
extern "C" {
#endif /* _cplusplus */

#include <time.h>

/* Boolean type definitions */
typedef enum bool { false, true } bool;

#define CRAFT_MAX_ID_SIZE 512
#define CRAFT_MAX_PROPERTIES_SIZE 128

typedef enum craft_property_type {
    ENUM,
    CONTINUOUS,
    TIME_OF_DAY,
    DAY_OF_WEEK,
    TIMEZONE
} craft_property_type_t;

typedef struct craft_property_definition {
    char* key;
    craft_property_type_t type;
    bool is_generated;
} craft_property_definition_t;

typedef struct craft_property {
    char* key;
    craft_property_type_t type;
    union {
      float number_value;
      char* string_value;
    } value;
} craft_property_t;

typedef struct craft_model {
    craft_property_definition_t properties[CRAFT_MAX_PROPERTIES_SIZE];
    size_t properties_count;
    char* outputs[CRAFT_MAX_PROPERTIES_SIZE];
    size_t outputs_count;
    unsigned int time_quantum;
} craft_model_t;

typedef struct craft_context {
    time_t timestamp;
    craft_property_t* properties;
    size_t properties_count;
} craft_context_t;

typedef struct craft_decision {
    craft_context_t context;

    craft_property_t output;
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

    CRAFTAI_ALREADY_INITIALIZED,

    CRAFTAI_CURL_ERROR,

    CRAFTAI_UNKNOWN_CONFIG_OPTION,

    CRAFTAI_UNKNOWN_ERROR
} craft_status_t;

typedef enum craft_option {
    CRAFTAI_TOKEN = 0, /* Token, can be changed at any time - char* */
    CRAFTAI_OWNER, /* Owner, can be changed at any time - char* */
    CRAFTAI_URL /* (Advanced) API Url, can be changed at any time - char* */
} craft_option_t;

/**
 * Initializes the craft ai C client, should be called **before** any other function!
 *
 * @return Status_code:
 *  - CRAFTAI_OK if everything went fine
 *  - CRAFTAI_ALREADY_INITIALIZED if the client was already initialized
 *  - CRAFTAI_CURL_ERROR if cURL (used for http requests) initialization failed
 */
craft_status_t craft_init();

/**
 * Tears down the craft ai C client.
 *
 * @return Status_code:
 *  - CRAFTAI_OK if everything went fine
 */
craft_status_t craft_teardown();

/**
 * Set the value of a given option in the client configuration.

 * @return Status_code:
 *  - CRAFTAI_OK if everything went fine
 *  - CRAFTAI_UNKNOWN_CONFIG_OPTION if the given option is unknown
 */
craft_status_t craft_config(craft_option_t option, ...);

/**
 * Retrieve the value of a given option from the client configuration.
 */
craft_status_t craft_get_config(craft_option_t option, ...);

/**
 * @brief      Creates a new agent with the given model and agent_id.
 *
 *             If agent_id is null, craft ai will generate an ID for you.
 *
 * @param      model     The model upon which craft ai bases its learning for
 *                       the given agent
 * @param      agent_id  The agent identifier. Should stay the same if you gave
 *                       one and will contain the newly generated one otherwise. You
 *                       should not expect agent_id to point to the same adress
 *                       space as the one given, since it might be freed and
 *                       changed.
 *
 * @return     Status code:
 *              - CRAFTAI_OK if everything went fine
 *              - CRAFTAI_*_ERROR otherwise (should be from the list of request
 *                errors)
 */
craft_status_t craft_create_agent(craft_model_t* model, char* agent_id);
/**
 * @brief      Retrieves the agent's previously defined model.
 *
 * @param      agent_id  The agent identifier
 * @param      model     A model structure which should be allocated and will be
 *                       initialized from craft ai's response.
 * @param      first_ts  The timestamp at which the first context operation was
 *                       posted.
 * @param      last_ts   The timestamp at which the last context operation was
 *                       posted.
 *
 * @return     Status code:
 *              - CRAFTAI_OK if everything went fine
 *              - CRAFTAI_*_ERROR otherwise (should be from the list of request
 *                errors)
 */
craft_status_t craft_get_agent(char* agent_id, craft_model_t* model, time_t* first_ts, time_t* last_ts);
/**
 * @brief      Deletes the agent with the give agent's id.
 *
 * @param      agent_id  The agent identifier
 *
 * @return     Status code:
 *              - CRAFTAI_OK if everything went fine
 *              - CRAFTAI_*_ERROR otherwise (should be from the list of request
 *                errors)
 */
craft_status_t craft_delete_agent(char* agent_id);


/**
 * @brief      Adds context operations to the agent's model, so that craft ai
 *             can learn from it.
 *
 * @param      agent_id         The agent identifier
 * @param      operations_list  The operations list
 * @param[in]  nb_operations    The number of operations in the list
 *
 * @return     Status code:
 *              - CRAFTAI_OK if everything went fine
 *              - CRAFTAI_*_ERROR otherwise (should be from the list of request
 *                errors)
 */
craft_status_t craft_add_operations(char* agent_id, craft_context_t* operations_list, size_t nb_operations);
/**
 * @brief      Gives out the list of operations that have been previously added
 *             to the agent.
 *
 * @param      agent_id         The agent identifier
 * @param      operations_list  The list of operations that were previously added
 * @param      nb_operations    The number of operations contained in the list
 *
 * @return     Status code:
 *              - CRAFTAI_OK if everything went fine
 *              - CRAFTAI_*_ERROR otherwise (should be from the list of request
 *                errors)
 */
craft_status_t craft_list_operations(char* agent_id, craft_context_t* operations_list, size_t* nb_operations);
/**
 * @brief      Retrieves the agent's context at a given timestamp.
 *
 * @param      agent_id   The agent identifier
 * @param[in]  timestamp  The timestamp at which the context is retrieved.
 * @param      state      The context state at the given timestamp as given by craft ai's API.
 *
 * @return     Status code:
 *              - CRAFTAI_OK if everything went fine
 *              - CRAFTAI_*_ERROR otherwise (should be from the list of request
 *                errors)
 */
craft_status_t craft_retrieve_state(char* agent_id, time_t timestamp, craft_context_t* state);

#ifdef _cplusplus
}
#endif /* _cplusplus */

#endif /* CRAFTAI_H */
